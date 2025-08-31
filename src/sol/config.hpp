// The MIT License (MIT)

// Copyright (c) 2013-2020 Rapptz, ThePhD and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This file was generated with a script.
// Generated 2022-06-25 08:14:19.336233 UTC
// This header was generated with sol v3.3.0 (revision eba86625)
// https://github.com/ThePhD/sol2

#ifndef SOL_SINGLE_CONFIG_HPP
#define SOL_SINGLE_CONFIG_HPP

// beginning of sol/config.hpp

// ============================================================================
// PERFORMANCE OPTIMIZATIONS
// ============================================================================

// Enable LuaJIT optimizations
#ifdef LUAJIT_VERSION
#define SOL_LUAJIT 1
#define SOL_USING_CXX_LUAJIT 1
#else
#define SOL_LUAJIT 0
#define SOL_USING_CXX_LUAJIT 0
#endif

// Disable expensive safety checks in release builds
#ifdef NDEBUG
  #define SOL_ALL_SAFETIES_ON 0
  #define SOL_SAFE_USERTYPE 0
  #define SOL_SAFE_STACK_CHECK 0
  #define SOL_SAFE_FUNCTION_CALLS 0
  #define SOL_SAFE_FUNCTION_OBJECTS 0
  #define SOL_SAFE_PROXIES 0
  #define SOL_SAFE_NUMERICS 0
  #define SOL_SAFE_GETTER 0
  #define SOL_SAFE_REFERENCES 0
#else
  // Keep safety checks in debug builds
  #define SOL_ALL_SAFETIES_ON 1
  #define SOL_SAFE_USERTYPE 1
  #define SOL_SAFE_STACK_CHECK 1
#endif

// Disable automatic usertype features for performance
#define SOL_NO_DEFAULT_USERTYPE_TRAITS 1
#define SOL_NO_OVERRIDE 1

// Enable function call optimizations
#define SOL_FUNCTION_CALL_TRAMPOLINE 1
#define SOL_OPTIMIZE_FOR_SINGLE_RETURN 1

// Enable string optimizations  
#define SOL_STRING_VIEW_LITERALS 1

// Disable expensive runtime type checking
#define SOL_NO_CHECK_NUMBER_PRECISION 1

// Enable stack optimizations
#define SOL_STACK_STRING_OPTIMIZATION_SIZE 1024

// Memory optimizations
#define SOL_CONTAINER_TRAITS 1

// Enable compile-time optimizations
#define SOL_CONTAINERS_START 1

// Performance: Disable exception handling overhead where possible
#ifdef __cpp_exceptions
  #define SOL_NO_EXCEPTIONS 0
#else
  #define SOL_NO_EXCEPTIONS 1
#endif

#endif // SOL_SINGLE_CONFIG_HPP
