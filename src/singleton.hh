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
#ifndef SINGLETON_HH
#define SINGLETON_HH

// Simple CRTP C++11 thread-safe singleton
template <class T>
class Singleton
{
public:
  Singleton() = default;
  virtual ~Singleton() = default;

  static T& GetInstance() {
    static T instance;
    return instance;
  }
};

#endif /* end of include guard: SINGLETON_HH */

