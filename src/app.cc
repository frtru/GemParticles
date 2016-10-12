//C system files
//C++ system files
#include <iostream>
#include <vector>
//Other libraries' .h files
//Your project's .h files
#include "particle_system.hh"
#include "euler_model.hh"
#include "fountain.hh"
#include "timer.hh"

/*void Update(std::vector<Gem::Particle::Particle>& a_particles, float delta) {
    std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
    std::cout << "Delta: " << delta << std::endl;
    for (unsigned int i = 0; i < a_particles.size(); i++) {
        a_particles[i].position += a_particles[i].velocity*delta;
    }
    timer::chrono::Update();
}*/

int main(int argc, const char *argv[]) {
    (void)argc;(void)argv;

    Gem::Particle::System sys(2000000);
    sys.AddDynamic(new Gem::Particle::EulerModel);
    sys.AddSource(new Gem::Particle::Fountain({ 0.0f,0.0f,0.0f }));

    std::cout << sizeof(Gem::Particle::Particle) << std::endl;
    while(true) {
      sys.Udpate(timer::chrono::GetTimeElapsed<timer::nanoseconds>() / timer::NANO_PER_SEC);
//        Update(particles,timer::chrono::GetTimeElapsed<timer::nanoseconds>()/timer::NANO_PER_SEC);
    }
    return 0;
}
