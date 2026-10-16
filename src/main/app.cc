/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
*************************************************************************/
#include "app.hh"
//C system files
//C++ system files
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
//Other libraries' .h files
//Your project's .h files
#include "main/app_registry.hh"
#include "main/app_world.hh"
#include "main/config.hh"
#include "main/options.hh"
#include "projects/project_dictionary.hh"
#include "utils/graphic_contexts/graphic_context.hh"
#include "utils/graphic_contexts/graphic_context_factory.hh"
#include "utils/timer.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
bool _ProjectsLoaded = false;

// Shaders, textures and the default config are next to the executable
std::filesystem::path ExecutableDirectory() {
#ifdef __linux__
  std::error_code error;
  const auto executable = std::filesystem::read_symlink("/proc/self/exe", error);
  if (!error) {
    return executable.parent_path();
  }
  std::cerr << __func__ << " -> " << error.message() << std::endl;
#endif
  return std::filesystem::current_path();
}

int Fail(ErrCode a_eError, const std::string& a_sReason) {
  std::cerr << "Run -> " << a_sReason << std::endl;
  return a_eError;
}

void RunSystems(std::function<void(AppWorld&)> AppSystem::*a_pHook, AppWorld& a_rWorld) {
  for (const auto& system : AppSystems()) {
    if (const auto& hook = system.second.*a_pHook) {
      hook(a_rWorld);
    }
  }
}

// Every project starts from the same render state, and leaves no input callbacks behind
void StartProject(const Project& a_Project, const std::shared_ptr<GraphicContext>& a_pContext, AppWorld& a_rWorld) {
  a_pContext->ResetRenderState();
  a_Project.init(a_pContext);
  RunSystems(&AppSystem::onProjectStarted, a_rWorld);
}

void StopProject(const Project& a_Project, AppWorld& a_rWorld) {
  RunSystems(&AppSystem::onProjectStopped, a_rWorld);
  a_Project.terminate();
  a_rWorld.context.SetInputCallbacks({});
}
}

void LoadProjects() {
  if (!_ProjectsLoaded) {
    project_dict::Init();
    _ProjectsLoaded = true;
  }
}

int Run(int argc, const char* argv[]) {
  // The project argument is checked against the registered ones
  LoadProjects();
  Options options;
  if (const std::optional<int> exitCode = ParseOptions(argc, argv, options)) {
    return *exitCode;
  }

  // Loaded before moving to the executable's folder: its relative paths are the user's
  const std::filesystem::path executableDirectory = ExecutableDirectory();
  std::optional<Config> config = LoadConfig(
    options.configPath.empty() ? (executableDirectory / "config.json").string() : options.configPath);
  if (!config) {
    return Fail(CONFIG_ERROR, "Invalid config");
  }
  std::filesystem::current_path(executableDirectory);

  const std::string projectName = options.project.empty() ? config->defaultProject : options.project;
  const Project* project = project_dict::LookUp(projectName);
  if (project == nullptr) {
    return Fail(UNRECOGNIZED_PROJECT, "Unrecognized project " + projectName);
  }
  const std::shared_ptr<GraphicContext> context = CreateGraphicContext(config->backend);
  if (context == nullptr) {
    return Fail(CONFIG_ERROR, "No graphic context");
  }

  // The app owns the window and the frame; the project and the systems plug into it
  context->Init(config->window);
  AppWorld world{ std::move(config->components), *context, projectName };
  StartProject(*project, context, world);

  timer::Chrono& chrono = timer::Chrono::GetInstance();
  while (!world.exitRequested && !context->PollWindowClosedEvent()) {
    const double dt = chrono.GetTimeElapsedInSeconds();
    context->NewFrame();
    RunSystems(&AppSystem::update, world);
    project->update(dt);
    context->Update();
    chrono.Update();

    // Projects are switched between frames
    if (world.nextProjectRequested) {
      world.nextProjectRequested = false;
      StopProject(*project, world);
      world.project = project_dict::NextProjectName(world.project);
      project = project_dict::LookUp(world.project);
      StartProject(*project, context, world);
      chrono.Reset(); // Loading the project isn't part of the next frame
    }
  }

  StopProject(*project, world);
  context->Terminate();
  return NO_ERROR;
}
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
