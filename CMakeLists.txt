set(LUA_VERSION "lua52" CACHE STRING "Lua version to use")
set_property(CACHE LUA_VERSION PROPERTY STRINGS luajit lua51 lua52 lua53 lua54)
MESSAGE(STATUS "Lua version: ${LUA_VERSION}")

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

option(LUA_STATIC "link lua statically" ON)
if (LUA_STATIC)
  MESSAGE(STATUS "Lua linking: static")
else()
  MESSAGE(STATUS "Lua linking: dynamic")
endif()

if (LUA_VERSION MATCHES "luajit")
  add_subdirectory(src/lualib/luajit)
else()
  add_subdirectory(src/lualib/lua)
endif()

# Download and configure Sol2 automatically
include(FetchContent)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG c1f95a773c6f8f4fde8ca3efe872e7286afe4444
)

FetchContent_MakeAvailable(sol2)

target_compile_definitions(sol2 INTERFACE
  SOL_ALL_SAFETIES_ON=0
  SOL_SAFE_USERTYPE=0
  SOL_SAFE_STACK_CHECK=0
  SOL_SAFE_FUNCTION_CALLS=0
  SOL_SAFE_FUNCTION_OBJECTS=0
  SOL_SAFE_PROXIES=0
  SOL_SAFE_NUMERICS=0
  SOL_SAFE_GETTER=0
  SOL_SAFE_REFERENCES=0
  
  SOL_NO_DEFAULT_USERTYPE_TRAITS=1
  SOL_NO_OVERRIDE=1
  SOL_FUNCTION_CALL_TRAMPOLINE=1
  SOL_OPTIMIZE_FOR_SINGLE_RETURN=1
  SOL_STRING_VIEW_LITERALS=1
  SOL_NO_CHECK_NUMBER_PRECISION=1
  SOL_STACK_STRING_OPTIMIZATION_SIZE=2048
  SOL_CONTAINER_TRAITS=1
  SOL_CONTAINERS_START=1
  
  SOL_USERTYPE_TYPE_BINDING_INFO=0
  SOL_NO_NIL=1
  SOL_MINIMIZE_USERTYPE_SIZE=1
  SOL_FAST_TYPE_CHECKING=1
)

# Enable LuaJIT optimizations if available
if (LUA_VERSION MATCHES "luajit")
  target_compile_definitions(sol2 INTERFACE
    SOL_LUAJIT=1
    SOL_USING_CXX_LUAJIT=1
  )
else()
  target_compile_definitions(sol2 INTERFACE
    SOL_LUAJIT=0
    SOL_USING_CXX_LUAJIT=0
  )
endif()

# Link sol2 with lualib
target_link_libraries(sol2 INTERFACE lualib)