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
//Other libraries' .h files
//Your project's .h files
#include "projects/project_dictionary.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
bool    _ProjectsLoaded = false;
Status  _AppStatus;

// Shaders and textures are loaded relative to the executable's folder
void MoveToExecutableDirectory() {
#ifdef __linux__
  std::error_code error;
  const auto executable = std::filesystem::read_symlink("/proc/self/exe", error);
  if (!error) {
    std::filesystem::current_path(executable.parent_path(), error);
  }
  if (error) {
    std::cerr << __func__ << " -> " << error.message() << std::endl;
  }
#endif
}
}

void LoadProjects() {
  if (!_ProjectsLoaded) {
    project_dict::Init();
    _ProjectsLoaded = true;
  }
}

ErrCode Launch(const Options& a_Options) {
  // Load the projects beforehand, in case the user forgot to
  LoadProjects();
  GraphicContext::SetOptions(a_Options.context);
  MoveToExecutableDirectory();

  // Check if the project in input is recognized in the dictionary
  ProjectPipeline *wPipeline = project_dict::LookUp(a_Options.project);
  if (wPipeline == nullptr) {
    _AppStatus._Error = UNRECOGNIZED_PROJECT;
    _AppStatus._Running = false;
    std::cerr << __func__ << " -> Unrecognized project error." << std::endl;
  }
  else
  {
    _AppStatus._Error = NO_ERROR;
    // Execute the pipeline of the project sequentially
    for (auto stage : *wPipeline) {
      stage();
    }
  }
  return _AppStatus._Error;
}

ErrCode GetAppStatusError() { return _AppStatus._Error; }
void SetAppStatusError(ErrCode a_unErrorCode) { _AppStatus._Error = a_unErrorCode; }
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

