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
#include "app.hh"

#include <vector>
#include "timer.hh"

// TODO: Temporary includes since test suite
// is not built yet...
#include "fountain_source.hh"
#include "global_acceleration.hh"

namespace Gem {
namespace Particle {
App::App()
  : m_particleSystem(100000) {
//TODO: When loading a config will be available 
//fill the particlesystem with what the config have
//and remove that temporary hardcoded effect
  m_particleSystem.AddSource(
    std::make_unique<Gem::Particle::FountainSource>(
      Gem::Particle::FountainSource({ 0.0f,0.0f,0.0f })
    )
  );
  m_particleSystem.AddDynamic(
    std::make_unique<Gem::Particle::GlobalAcceleration>()
  );
}
App::~App() {
}
void App::LoadConfig(const std::string& a_sConfigName) {
  //TODO
}
void App::SaveConfig(const std::string& a_sConfigName) {
  //TODO
}
void App::Run() {
  while (true) {
    //TODO: See how UI with anttweakbar goes, but
    //events subscription should go here if there's any
   double dt = timer::chrono::GetTimeElapsed<std::chrono::nanoseconds>()
      / timer::NANO_PER_SEC;
    m_particleSystem.Update(dt);
    
    //TODO: Render here
    //m_particleSystem.Render()
    
    timer::chrono::Update();
  }
}
} /* namespace Particle */
} /* namespace Gem */

