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
#ifndef PROJECT_DICTIONARY_HH
#define PROJECT_DICTIONARY_HH

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace gem {
namespace particle {
class GraphicContext;

// A project plugs into the app's main loop: the app owns the graphic context
// and the frame, the project sets itself up and does its part of each frame.
struct Project {
  std::function<void(const std::shared_ptr<GraphicContext>& a_pContext)> init;
  std::function<void(double a_dt)> update;
  std::function<void()> terminate;
};

namespace project_dict {
// Explicitely loads and registers all the recognized
void Init();
// Retrieves the project registered under the given name, nullptr if there is none
const Project* LookUp(const std::string& a_sProjectName);
// Names of every registered project, in alphabetical order
std::vector<std::string> GetProjectNames();
// Name of the project registered after the given one, wrapping around to the first
std::string NextProjectName(const std::string& a_sProjectName);
void Register(const std::string& a_sProjectName, Project a_Project);

// Standard project should have those functions defined
//  - Init(const std::shared_ptr<GraphicContext>&)
//  - Update(double)
//  - Terminate()
#define REGISTER_STD_PROJECT(projectName) \
        gem::particle::project_dict::Register(#projectName, { \
          &gem::particle::projectName##_project::Init, \
          &gem::particle::projectName##_project::Update, \
          &gem::particle::projectName##_project::Terminate })

} /* namespace project_dict */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PROJECT_DICTIONARY_HH */
