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
#include "graphic_context.hh"

namespace gem {
namespace particle {
bool GraphicContext::m_bInitialized = false;
bool GraphicContext::m_bTerminated = false;

void GraphicContext::Init() {
  if (!m_bInitialized) {
    InitImpl();
    m_bInitialized = true;
  }
  // TODO: Else log error, already initialized or something
}

void GraphicContext::Terminate() {
  if (!m_bTerminated) {
    TerminateImpl();
    m_bTerminated = true;
  }
  // TODO: Else log error, already terminated or something
}

} /* namespace particle */
} /* namespace gem */
