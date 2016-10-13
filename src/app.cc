//C system files
//C++ system files
#include <iostream>
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
#include <vector>
//Other libraries' .h files
//Your project's .h files
#include "particle_system.hh"
#include "euler_model.hh"
#include "fountain.hh"
#include "timer.hh"

int main(int argc, const char *argv[]) {
    (void)argc;(void)argv;

    Gem::Particle::System sys(2000000);
    sys.AddDynamic(new Gem::Particle::EulerModel);
    sys.AddSource(new Gem::Particle::Fountain({ 0.0f,0.0f,0.0f }));

    std::cout << sizeof(Gem::Particle::Particle) << std::endl;
    while(true) {
      std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
      sys.Udpate(timer::chrono::GetTimeElapsed<timer::nanoseconds>() / timer::NANO_PER_SEC);
      timer::chrono::Update();
    }
    return 0;
}
