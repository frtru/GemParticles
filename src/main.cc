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

using namespace gem::particle;

int main(int argc, const char *argv[]) {
  app::Launch("meme");
  // TODO: If desired, one could do more extensive
  // error handling, but this'll be enough for now
  return app::GetAppStatusError();
}
