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

# Link sol2 with lualib
target_link_libraries(sol2 INTERFACE lualib)