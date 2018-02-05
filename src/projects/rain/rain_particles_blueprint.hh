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
#ifndef RAIN_PARTICLES_BLUEPRINT_HH
#define RAIN_PARTICLES_BLUEPRINT_HH

#include <string>

namespace gem { namespace particle {
namespace rain_project {
namespace blueprint { namespace rain_particles_builder {
// This creates the said blueprint and adds it to the ParticleModule
void Create();

// Some parameters setter for this particuler blueprint builder
void SetTexture(const std::string& a_sTexturePath);
void SetParticleSystemName(const std::string& a_sSystemName);
void SetEmissionRate(double a_dEmissionRate);
void SetParticleCount(std::size_t a_unParticleCount);
} /* namespace rain_particles_builder */
} /* namespace blueprint */
} /* namespace rain_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: RAIN_PARTICLES_BLUEPRINT_HH */
