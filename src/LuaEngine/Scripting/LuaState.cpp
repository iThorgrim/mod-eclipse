#include "LuaState.hpp"

namespace Eclipse
{
    LuaState::LuaState() : isInitialized(false)
    {
    }

    bool LuaState::Initialize()
    {
        if (isInitialized)
            return true;

        try
        {
            luaState = sol::state();
            OpenStandardLibraries();
            
            isInitialized = true;
            LOG_TRACE("server.eclipse", "[Eclipse]: LuaState initialized successfully");
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to initialize LuaState: {}", e.what());
            isInitialized = false;
            return false;
        }
    }

    void LuaState::Reset()
    {
        if (isInitialized)
        {
            luaState = sol::state();
            isInitialized = false;
            LOG_TRACE("server.eclipse", "[Eclipse]: LuaState reset");
        }
    }

    void LuaState::ConfigureLibraries()
    {
        if (isInitialized)
        {
            OpenStandardLibraries();
        }
    }

    void LuaState::OpenStandardLibraries()
    {
        luaState.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            sol::lib::debug,
            sol::lib::bit32,
            sol::lib::io,
            sol::lib::utf8
#ifdef SOL_LUAJIT
            , sol::lib::jit
#endif
        );
    }
}