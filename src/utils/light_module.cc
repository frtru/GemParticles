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
#include "utils/light_module.hh"

#include <vector>
#include <mutex>

#include "utils/shader_module.hh"
#include "utils/imgui/imgui_log.h"

namespace gem { namespace particle {
namespace light {

constexpr GLuint        LIGHTS_SSBO_BINDING_POINT = 1U;
std::vector<Light>      _Lights;
bool                    _DirtyFlag;
std::once_flag          _InitFlag;
std::once_flag          _TerminateFlag;

void UpdateGPUBufferSize() {
  // Check if capacity changed after add
  shader::module::UpdateSSBOBlockData(
  LIGHTS_SSBO_BINDING_POINT, _Lights.size() * sizeof(Light),
  static_cast<void*>(_Lights.data()));
  // this also updates the buffer data so no need to send it a second time in the same iteration
  _DirtyFlag = false;
}


namespace module {
void Init() {
  std::call_once(_InitFlag, [&]() {
    _DirtyFlag = false;
    shader::module::RegisterSSBOBlock(
      LIGHTS_SSBO_BINDING_POINT,
      _Lights.size() * sizeof(Light),
      static_cast<void*>(_Lights.data()));
  });
}

void Terminate() {
  std::call_once(_TerminateFlag, [&]() {});
}


void Resize(std::size_t LightCount, const Light& defaultLight) {
  if (LightCount > MAX_LIGHTS) {
    ImGuiLog::GetInstance().AddLog("[ERROR]light_module::Resize -> Trying to resize above the maximal capacity.\n");
    return;
  }
  _Lights.resize(LightCount, defaultLight);
  UpdateGPUBufferSize();
  ImGuiLog::GetInstance().AddLog("light_module::Resize -> Resized lights pool to size = %d\n", LightCount);
}

void FlushDataToGPU() {
  if (_DirtyFlag) {
    // Send data to GPU
    shader::module::SetSSBOBlockSubData(
      LIGHTS_SSBO_BINDING_POINT,
      0,                                  // Offset is set at the beginning of pool
      static_cast<void*>(_Lights.data()), // Lights data
      sizeof(Light) * _Lights.size());    // Send _Lights.size() light data
    _DirtyFlag = false;
  }
}

void SetDirty() {
  _DirtyFlag = true;
}

/* Alters the lights count. The AddLight functions return the ID of the lights added. */
std::size_t AddLight(const Light &light) {
  if (_Lights.size() + 1 > MAX_LIGHTS) {
    ImGuiLog::GetInstance().AddLog("[ERROR]light_module::AddLight -> Trying to add one light above the maximal capacity.\n");
    return -1;
  }
  // Add to _Lights
  std::size_t wIndex = _Lights.size();
  _Lights.push_back(light);
  UpdateGPUBufferSize();
  // Return the index of the newly added light
  ImGuiLog::GetInstance().AddLog("light_module::AddLight(&) -> Added a light with index = %d\n", wIndex);
  return wIndex;
}
std::size_t AddLight(Light&& light) {
  if (_Lights.size() + 1 > MAX_LIGHTS) {
    ImGuiLog::GetInstance().AddLog("[ERROR]light_module::AddLight -> Trying to add one light above the maximal capacity.\n");
    return -1;
  }
  // Add to _Lights
  std::size_t wIndex = _Lights.size();
  _Lights.push_back(std::move(light));
  UpdateGPUBufferSize();
  // Return the index of the newly added light
  ImGuiLog::GetInstance().AddLog("light_module::AddLight(&&) -> Added a light with index = %d\n", wIndex);
  return wIndex;
}

/* Getters */
std::size_t GetLightsCount() {
  return _Lights.size();
}
Light GetLight(std::size_t lightID) {
  if (lightID < _Lights.size()) {
    return _Lights[lightID];
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]light_module::GetNumberOfLights -> lightID is out of bound.\n");
  return Light();
}
Light& GetLightRef(std::size_t lightID) {
  if (lightID < _Lights.size()) {
    _DirtyFlag = true; // Assume change from the user
    return _Lights[lightID];
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]light_module::GetNumberOfLights -> lightID is out of bound.\n");
  return Light();
}
Light* GetLightPointer(std::size_t lightID) {
  if (lightID < _Lights.size()) {
    _DirtyFlag = true; // Assume change from the user
    return &(_Lights[lightID]);
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]light_module::GetNumberOfLights -> lightID is out of bound.\n");
  return nullptr;
}

/* By using the light ID, one can update the corresponding light in
the module if he or she used GetLight instead of the reference getters. */
void UpdateLight(std::size_t lightID, const Light &light) {
  if (lightID >= 0 && lightID < _Lights.size()) {
    _Lights[lightID] = light;
    _DirtyFlag = true;
    return;
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]light_module::UpdateLight -> lightID is out of bound.\n");
}
void UpdateLight(std::size_t lightID, Light &&light) {
  if (lightID >= 0 && lightID < _Lights.size()) {
    _Lights[lightID] = std::move(light);
    _DirtyFlag = true;
    return;
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]light_module::UpdateLight -> lightID is out of bound.\n");
}
} /* namespace module */
} /* namespace light */
} /* namespace particle */
} /* namespace gem */
