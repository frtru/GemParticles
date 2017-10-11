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

#include <cstddef>
#include <string>

namespace gem {
namespace particle {
namespace app {
enum ErrCode {
  NO_ERROR                = 0x00,
  UNRECOGNIZED_PROJECT    = 0x01,
  PROJECT_LOADING_ERROR   = 0x02,
  UNRECOGNIZED_BLUEPRINT  = 0x03,
  MEMORY_ACCESS_ERROR     = 0x04,
  APP_LOGIC_ERROR         = 0x05
}; /* enum ErrCode */

struct Status {
  bool        _Running;
  ErrCode     _Error;
};

void LoadProjects();
ErrCode Launch(const std::string& a_sProjectName);
ErrCode GetAppStatusError();
void SetAppStatusError(ErrCode a_unErrorCode);
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: APP_HH */
