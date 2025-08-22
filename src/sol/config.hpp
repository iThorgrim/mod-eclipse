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

/* Configuration for Eclipse Sol3 integration */

// Disable ALL automatic comparison operators generation
#define SOL_NO_DEFAULT_USERTYPE_TRAITS 1

// Disable automatic metatable generation 
#define SOL_NO_OVERRIDE 1

// Completely disable usertype automatic operators
// Dynamic configuration via CMake
#ifndef SOL_USING_CXX_LUA
  #define SOL_USING_CXX_LUA 0
#endif

#ifndef SOL_USING_CXX_LUAJIT  
  #define SOL_USING_CXX_LUAJIT 0
#endif

#ifndef SOL_LUAJIT
  #define SOL_LUAJIT 0
#endif

#define SOL_USING_CXX_LUAJIT 0

// Use minimal safe operations only
#define SOL_SAFE_USERTYPE 1
#define SOL_SAFE_STACK_CHECK 1

// end of sol/config.hpp

#endif // SOL_SINGLE_CONFIG_HPP
