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
#include "texture_module.hh"

#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <mutex>

#include "texture_factory.hh"

namespace texture {
namespace module {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;
}
void Init() {
  std::call_once(init_flag, [&]() {
    texture::factory::Init();
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    texture::factory::Terminate();
  });
}
} /* namespace module */
} /* namespace texture */