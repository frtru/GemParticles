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
#ifndef RAIN_EMITTER_TEST_HH
#define RAIN_EMITTER_TEST_HH

#include <memory>
#include <gtest/gtest.h>

#include "opengl_context.hh"

class RainEmitterTest : public ::testing::Test {
public:
  RainEmitterTest();
  virtual ~RainEmitterTest();
protected:
  virtual void SetUp();
  virtual void TearDown();
private:
  // A pointer to interface, to enable flexibility over
  // window management system or 3D API (GLFW/Windows
  // & OpenGL/Direct3D)
std::shared_ptr<gem::particle::GraphicContext> m_pGraphicContext;
};

#endif /* end of include guard: RAIN_EMITTER_TEST_HH */
