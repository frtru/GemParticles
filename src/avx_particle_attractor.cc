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
#include "avx_particle_attractor.hh"

#include <immintrin.h>
#include "macro_definitions.hh"
#include "glm/gtx/simd_vec4.hpp"

namespace gem {
namespace particle {
namespace avx_details{
  const long long   mask_on  = 0xFFFFFFFFFFFFFFFF;
  const long long   mask_off = 0x0000000000000000;
  const std::size_t particles_per_iteration = sizeof(__m256) / sizeof(glm::f32vec3);
}
AVXParticleAttractor::AVXParticleAttractor(
  const glm::f32vec3 &a_fvAttractionPosition,
  float a_accelerationRate)
  : m_fvAttractionPosition(a_fvAttractionPosition),
    m_fAccelerationRate(a_accelerationRate) {}

void AVXParticleAttractor::Update(double a_dt,
  const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  const float       fDt = (float)a_dt;
  const float       fIterationAccel = fDt * m_fAccelerationRate;
  const std::size_t unParticleCount = a_pPool->GetActiveParticleCount();
  const std::size_t unAVXIterableRange = unParticleCount -
    unParticleCount % avx_details::particles_per_iteration;

  // Load constant iteration data into AVX registers
  const __m256      m256Dt = _mm256_set1_ps(fDt); // Could use a_dt instead with _mm256_set1_pd?
  const __m256      m256IterationAccel  = _mm256_set1_ps(
    fDt * m_fAccelerationRate);
  const __m256      m256AttractionPos   = _mm256_set_ps(
    m_fvAttractionPosition.x, m_fvAttractionPosition.y, m_fvAttractionPosition.z, 
    m_fvAttractionPosition.x, m_fvAttractionPosition.y, m_fvAttractionPosition.z, 
    0.0f, 0.0f); // Unused last 64 bits
  const __m256i     m256iMask = _mm256_setr_epi64x(
    avx_details::mask_off, avx_details::mask_on, avx_details::mask_on,
    avx_details::mask_on); // Unused last 64 bits

  // Velocity update

  // Since we can process 2 particles per iteration we have to stop the iteration 
  // before reaching out of bounds memory and handle the remaining data normally
  std::size_t i = 0;
  for (; i < unAVXIterableRange; i += avx_details::particles_per_iteration) {
    // Load position and velocity data into __m256 data structures
    __m256 m256Position = _mm256_maskload_ps(
      &(a_pPool->pCoreData->m_position[i][0]), m256iMask);
    __m256 m256Velocity = _mm256_maskload_ps(
      &(a_pPool->pCoreData->m_velocity[i][0]), m256iMask);

    // Compute new velocity
    __m256 m256AccelDir     = _mm256_sub_ps(m256AttractionPos, m256Position);
    __m256 m256DeltaVelocity = _mm256_mul_ps(m256AccelDir, m256IterationAccel);
    m256Velocity = _mm256_add_ps(m256Velocity, m256DeltaVelocity);

    // Store the results
    _mm256_maskstore_ps(
      &(a_pPool->pCoreData->m_velocity[i][0]), m256iMask, m256Velocity);
  }
  // Handle the array tail normally
  glm::f32vec3  fvAccelDir;
  for (; i < unParticleCount; ++i) {
    fvAccelDir = m_fvAttractionPosition - a_pPool->pCoreData->m_position[i];
    a_pPool->pCoreData->m_velocity[i] += fvAccelDir * fIterationAccel;
  }

  // Position update

  i = 0; // iteration reset
  for (; i < unAVXIterableRange; i += avx_details::particles_per_iteration) {
    // Load position and velocity data into __m256 data structures
    __m256 m256Position = _mm256_maskload_ps(
      &(a_pPool->pCoreData->m_position[i][0]), m256iMask);
    __m256 m256Velocity = _mm256_maskload_ps(
      &(a_pPool->pCoreData->m_velocity[i][0]), m256iMask);

    // Compute new position
    __m256 m256IterationVel = _mm256_mul_ps(m256Velocity, m256Dt);
    m256Position = _mm256_add_ps(m256Position, m256IterationVel);

    // Store the results
    _mm256_maskstore_ps(
      &(a_pPool->pCoreData->m_position[i][0]), m256iMask, m256Position);
  }

  for (; i < unParticleCount; ++i) {
    a_pPool->pCoreData->m_position[i] += a_pPool->pCoreData->m_velocity[i] * fDt;
  }
}
} /* namespace particle */
} /* namespace gem */
