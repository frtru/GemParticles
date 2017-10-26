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

#include <iostream>
#include <vector>
#include <mutex>

#include "utils/shader_module.hh"

namespace gem { namespace particle {
namespace light {
namespace {
constexpr GLuint        LIGHTS_SSBO_BINDING_POINT = 0U;
GLuint                  _MaxLightCount = 10U;
std::vector<Light>      _Lights;
std::once_flag          _InitFlag;
std::once_flag          _TerminateFlag;

void UpdateLightsGPUBuffer(std::size_t a_unIndex) {
  // Check if capacity changed after add
  if (_MaxLightCount != _Lights.capacity()) {
    _MaxLightCount = _Lights.capacity();
    shader::module::UpdateSSBOBlockData(
      LIGHTS_SSBO_BINDING_POINT, _MaxLightCount,
      static_cast<void*>(_Lights.data()));
  }
  else { // Otherwise just update 
    shader::module::SetSSBOBlockSubData(
      LIGHTS_SSBO_BINDING_POINT,
      a_unIndex * sizeof(Light), &_Lights[a_unIndex],
      sizeof(Light));
  }
}
}

namespace module {
void Init() {
  std::call_once(_InitFlag, [&]() {
    _Lights.reserve(_MaxLightCount);
    shader::module::RegisterSSBOBlock(
      LIGHTS_SSBO_BINDING_POINT,
      _Lights.capacity() * sizeof(Light),
      static_cast<void*>(_Lights.data()));
  });
}

void Terminate() {
  std::call_once(_TerminateFlag, [&]() {});
}

/* Alters the lights count. The AddLight functions return the ID of the lights added. */
std::size_t AddLight(const Light &light) {
  // Add to _Lights
  _Lights.push_back(light);
  std::size_t wIndex = _Lights.size() - 1;
  UpdateLightsGPUBuffer(wIndex);
  // Return the index of the newly added light
  std::cout << __func__ << " -> Added a light with index = " << wIndex << std::endl;
  return wIndex;
}
std::size_t AddLight(Light&& light) {
  // Add to _Lights
  _Lights.push_back(std::move(light));
  std::size_t wIndex = _Lights.size() - 1;
  UpdateLightsGPUBuffer(wIndex);
  // Return the index of the newly added light
  return wIndex;
}
void ExpandLightsCapacityBy(std::size_t numberOfAdditionalLights) {
  _MaxLightCount += numberOfAdditionalLights;
  _Lights.reserve(_MaxLightCount);
  shader::module::UpdateSSBOBlockData(
    LIGHTS_SSBO_BINDING_POINT, _MaxLightCount,
    static_cast<void*>(_Lights.data()));
}

/* Getters */
std::size_t GetNumberOfLights() {
  return _Lights.size();
}
Light GetLight(std::size_t lightID) {
  if (lightID < _Lights.size()) {
    return _Lights[lightID];
  }
  std::cerr << __func__ << " : lightID is out of bound." << std::endl;
  return Light();
}

/* By using the light ID, one can update the corresponding light in the module. */
void UpdateLight(std::size_t lightID, const Light &light) {
  if (lightID >= 0 && lightID < _Lights.size()) {
    _Lights[lightID] = light;
    shader::module::SetSSBOBlockSubData(
      LIGHTS_SSBO_BINDING_POINT,
      lightID * sizeof(Light), &_Lights[lightID],
      sizeof(Light));
    return;
  }
  std::cerr << __func__ << " : lightID is out of bound." << std::endl;
}
void UpdateLight(std::size_t lightID, Light &&light) {
  if (lightID >= 0 && lightID < _Lights.size()) {
    _Lights[lightID] = std::move(light);
    shader::module::SetSSBOBlockSubData(
      LIGHTS_SSBO_BINDING_POINT,
      lightID * sizeof(Light), &_Lights[lightID],
      sizeof(Light));
    return;
  }
  std::cerr << __func__ << " : lightID is out of bound." << std::endl;
}
} /* namespace module */
} /* namespace light */
} /* namespace particle */
} /* namespace gem */
