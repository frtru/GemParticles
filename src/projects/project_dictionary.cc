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
// mapping strings with projects
using ProjectDictionary = std::map<std::string, Project>;
ProjectDictionary _Dict;
}/* unnamed namespace */

void Init() {
  std::call_once(init_flag, [&]() {
    REGISTER_STD_PROJECT(attractor);
    REGISTER_STD_PROJECT(light);
    REGISTER_STD_PROJECT(lit_particles);
    REGISTER_STD_PROJECT(rain);
    REGISTER_STD_PROJECT(skybox);
  });
}

const Project* LookUp(const std::string& a_sProjectName) {
  const auto entry = _Dict.find(a_sProjectName);
  return entry != _Dict.end() ? &entry->second : nullptr;
}

std::vector<std::string> GetProjectNames() {
  std::vector<std::string> names;
  for (const auto& entry : _Dict) {
    names.push_back(entry.first);
  }
  return names;
}
std::string NextProjectName(const std::string& a_sProjectName) {
  const auto next = _Dict.upper_bound(a_sProjectName);
  return (next != _Dict.end() ? next : _Dict.begin())->first;
}

void Register(const std::string& a_sProjectName, Project a_Project) {
  _Dict[a_sProjectName] = std::move(a_Project);
  std::cout << "Project " << a_sProjectName << " has been registered." << std::endl; 
}
} /* namespace project_dict */
} /* namespace particle */
} /* namespace gem */
