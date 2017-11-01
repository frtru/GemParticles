/*************************************************************************
* Copyright (c) 2017 François Trudel
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
#ifndef MACROS_HH
#define MACROS_HH

// TODO: Define macros for < C++11 stuff,
// with functions declared as private...
// Maybe use __cplusplus macro ...

// Class declaration/definition helper macros
#define DECLARE_COPYABLE(ClassName)             \
  public:                                       \
  ClassName(const ClassName& other);            \
  ClassName& operator=(const ClassName& other); \
  private:

#define DECLARE_MOVABLE(ClassName)          \
  public:                                   \
  ClassName(ClassName&& other);             \
  ClassName& operator=(ClassName&& other);  \
  private:

// Those default macros are pretty useless on their own,
// but they show that the class is voluntarily declared
// as default copyable/movable
#define DECLARE_DEFAULT_COPYABLE(ClassName)                 \
  public:                                                   \
  ClassName(const ClassName& other) = default;              \
  ClassName& operator=(const ClassName& other)  = default;  \
  private:

#define DECLARE_DEFAULT_MOVABLE(ClassName)            \
  public:                                             \
  ClassName(ClassName&& other) = default;             \
  ClassName& operator=(ClassName&& other) = default;  \
  private:

// NOTE: When using the next two macros, keep in mind
// that when they are used in the base class of a hierarchy
// every child becomes also uncopyable/unmovable.
#define DECLARE_UNCOPYABLE(ClassName)         \
  ClassName(const ClassName& other) = delete; \
  ClassName& operator=(const ClassName& other) = delete;

#define DECLARE_UNMOVABLE(ClassName)      \
  ClassName(ClassName&& other) = delete;  \
  ClassName& operator=(ClassName&& other) = delete;

// Alignment cross-compiler compatible (useful for SIMD instructions) 
// DEPRECATED: C++11 offers the alignas directive
#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif
#endif

#endif /* MACROS_HH */