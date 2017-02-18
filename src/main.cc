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
//C system files
//C++ system files
//Other libraries' .h files
//Your project's .h files
#include "app.hh"

int main(int argc, const char *argv[]) {
  (void)argc;(void)argv;

  // Can't get much simpler than that
  gem::particle::app::Init();
  gem::particle::app::Run();
  gem::particle::app::Terminate();

  return 0;
}
