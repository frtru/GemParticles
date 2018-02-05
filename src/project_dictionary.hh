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

#include <string>
#include <vector>
#include <functional>

namespace gem {
namespace particle {
// For user convenience, this should be used to process the corresponding
// project pipeline (series of function/stages). In other words,
// pipeline = series of functions
// stage    = part of the pipeline (function)
using ProjectStage    = std::function<void()>;
using ProjectPipeline = std::vector<ProjectStage>;

namespace project_dict {

// TODO: If desired this could be replaced by another setup:
// Each project has its own dll/.so, the engine is a .lib/.a
// and the executable only runs the dll/.so loading (through .ini/xml files maybe?)
// and launching the said dynamic library. But this is good enough for now for decoupling.
// An issue will be created for this. Ref issue #63

// Explicitely loads and registers all the recognized
void Init();
// Retrieves the correct pipeline related to the given name.
// Returns nullptr when reaching the end of the dictionary
ProjectPipeline* LookUp(const std::string& a_sProjectName);
void AddStage(const std::string& a_sProjectName, const ProjectStage&& a_fProjectStage);
void AddPipeline(const std::string& a_sProjectName, const ProjectPipeline&& a_vProjectPipeline);
void SetPipeline(const std::string& a_sProjectName, const ProjectPipeline&& a_fProjectPipeline);
} /* namespace project_dict */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PROJECT_DICTIONARY_HH */
