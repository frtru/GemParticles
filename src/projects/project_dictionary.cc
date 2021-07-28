/*************************************************************************
 * Copyright (c) 2016 François Trudel
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
#include "project_dictionary.hh"

#include "projects/attractors/attractor_project.hh"
#include "projects/gpu_particles/gpu_particles_project.hh"
#include "projects/light/light_project.hh"
#include "projects/lit_particles/lit_particles_project.hh"
#include "projects/rain/rain_project.hh"
#include "projects/skybox/skybox_project.hh"

#include <map>
#include <mutex>
#include <iostream>

namespace gem {
namespace particle {
namespace project_dict {
namespace {
std::once_flag init_flag;
// For readability, we define the dictionary as collection of entries
// mapping strings with pipelines
using ProjectDictionary = std::map<std::string, ProjectPipeline>;
ProjectDictionary _Dict;
}/* unnamed namespace */

void Init() {
  std::call_once(init_flag, [&]() {
    REGISTER_STD_PROJECT(attractor);
    REGISTER_STD_PROJECT(gpu_particles);
    REGISTER_STD_PROJECT(light);
    REGISTER_STD_PROJECT(lit_particles);
    REGISTER_STD_PROJECT(rain);
    REGISTER_STD_PROJECT(skybox);
  });
}

ProjectPipeline* LookUp(const std::string& a_sProjectName) {
  for (auto& entry : _Dict) {
    if (entry.first.compare(a_sProjectName) == 0) {
      return &entry.second;
    }
  }
  return nullptr;
}
// NOTE: For the add functions, this is what happens when using them:
//
// If the project is not already in the dictionary,
// construct the key by copy and default construct the
// pipeline and add a stage/sub-pipeline to it. Otherwise, simply
// access the pipeline and a stage/sub-pipeline to it.
void AddStage(const std::string& a_sProjectName, const ProjectStage&& a_fProjectStage) {
  _Dict[a_sProjectName].push_back(std::move(a_fProjectStage));
}

void AddPipeline(const std::string& a_sProjectName, const ProjectPipeline&& a_vProjectPipeline) {
  _Dict[a_sProjectName].insert(a_vProjectPipeline.end(),
    a_vProjectPipeline.begin(),
    a_vProjectPipeline.end());
  std::cout << "Added pipeline to project " << a_sProjectName << "." << std::endl; 
}

void SetPipeline(const std::string& a_sProjectName, const ProjectPipeline&& a_vProjectPipeline) {
  _Dict[a_sProjectName].clear();
  _Dict[a_sProjectName] = std::move(a_vProjectPipeline);
  std::cout << "Project " << a_sProjectName << " has been registered." << std::endl; 
}
} /* namespace project_dict */
} /* namespace particle */
} /* namespace gem */
