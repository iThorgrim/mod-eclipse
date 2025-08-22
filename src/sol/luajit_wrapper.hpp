#ifndef SOL_LUAJIT_WRAPPER_HPP
#define SOL_LUAJIT_WRAPPER_HPP

// LuaJIT compatibility wrapper for Sol3
// LuaJIT uses the same header names as standard Lua

#ifdef SOL_LUAJIT
  // LuaJIT version detection
  #define LUAJIT_VERSION_NUM 20103  // 2.1.0-beta3
  
  // Include standard Lua headers (LuaJIT compatible)
  extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
  }
  
  // LuaJIT specific optimizations
  #define LUA_OPTIMIZE_MEMORY 1
  
#else
  // Standard Lua
  extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
  }
#endif

#endif // SOL_LUAJIT_WRAPPER_HPP