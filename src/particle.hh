#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "glm/glm.hpp"

//DEPRECATED: WE'RE USING AN SOA APPROACH
//INSTEAD OF AOS

namespace gem {
namespace particle {
struct Particle {

  Particle() = default;
  ~Particle() = default;

  Particle(float a_lifetime,
           const glm::f32vec3& a_position = {0.0f,0.0f,0.0f},
           const glm::f32vec3& a_velocity = {0.0f,0.0f,0.0f},
           const glm::f32vec3& a_acceleration = {0.0f,0.0f,0.0f},
           const glm::u8vec4& a_color = { 0.0f,0.0f,0.0f,0.0f })
      : lifetime(a_lifetime),
        position(a_position),
        velocity(a_velocity),
        acceleration(a_acceleration),
        color(a_color) {}

  // Default copyable and movable particles
  Particle(Particle&& other) = default;
  Particle(const Particle& other) = default;
  Particle& operator=(Particle&& other) = default;
  Particle& operator=(const Particle& other) = default;

  float			    lifetime;
  glm::u8vec4	  color;
  glm::f32vec3	position;
  glm::f32vec3	velocity;
  glm::f32vec3	acceleration;
}; /* struct Particle*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_HH */
