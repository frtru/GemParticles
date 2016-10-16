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
#ifndef APP_HH
#define APP_HH

#include <string>

namespace Gem {
namespace Particle {
namespace App {
void Init();
void LoadConfig(const std::string& a_sConfigName);
void SaveConfig(const std::string& a_sConfigName);
void Run();
}
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: APP_HH */
