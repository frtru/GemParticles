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
#ifndef GRAPHIC_CONTEXT_HH
#define GRAPHIC_CONTEXT_HH

namespace Gem {
namespace Particle {
class GraphicContext {
public:
  GraphicContext() {}
  virtual ~GraphicContext() {}

  virtual void Update() = 0;
  virtual int  PollWindowClosedEvent() = 0;
  void Init();
  void Terminate();

protected:
  virtual void InitImpl() = 0;
  virtual void TerminateImpl() = 0;

private:
  static bool  m_bInitialized;
  static bool  m_bTerminated;
}; /* class GraphicContext*/
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: GRAPHIC_CONTEXT_HH */