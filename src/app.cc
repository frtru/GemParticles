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
//C system files
//C++ system files
#include <iostream>
//Other libraries' .h files
//Your project's .h files
#include "timer.hh"
#include "shader.hh"
#include "particle_system.hh"

// TODO: Temporary includes since test suite
// is not built yet...
#include "fountain_source.hh"
#include "global_acceleration.hh"

namespace Gem {
namespace Particle {
namespace App {
namespace {
//TODO : Remove the follower shadermanager instance when it'll be a namespace
ShaderManager ShaderManagerInstance;
System particle_system(100000);
//TODO: GPU updater/renderer goes here
}
void Init() {
  //Context/OpenGL initialization
  //TODO:
  
  // GLEW initialization
  if (GLEW_OK != glewInit()) {
    std::cerr << "GLEW is not initialized!" << std::endl;
  }
  
  // Shaders initialization
  ShaderManagerInstance.LoadFromFile(GL_VERTEX_SHADER,   "shaders/default.vert");
  ShaderManagerInstance.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/default.frag");
  
  ShaderManagerInstance.CreateAndLink();
  ShaderManagerInstance.Bind();

  // Particle system initialization
  particle_system.AddSource(
    std::make_unique<Gem::Particle::FountainSource>(
      Gem::Particle::FountainSource({ 0.0f,0.0f,0.0f })
      )
  );
  particle_system.AddDynamic(
    std::make_unique<Gem::Particle::GlobalAcceleration>()
  );
}
void LoadConfig(const std::string& a_sConfigName) {
  //TODO
}
void SaveConfig(const std::string& a_sConfigName) {
  //TODO
}
void Run() {
  while (true) {
    std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
    //TODO: See how UI with anttweakbar goes, but
    //events subscription should go here if there's any
    double dt = timer::chrono::GetTimeElapsed<std::chrono::nanoseconds>()
      / timer::NANO_PER_SEC;
    particle_system.Update(dt);

    //TODO: Render here
    //m_particleSystem.Render()

    timer::chrono::Update();
  }
}
} /* namespace App */
} /* namespace Particle */
} /* namespace Gem */

