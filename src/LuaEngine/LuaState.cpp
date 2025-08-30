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
            SetupLuaJIT();
            
            isInitialized = true;
            LOG_TRACE("server.eclipse", "LuaState initialized successfully");
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to initialize LuaState: {}", e.what());
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
            LOG_TRACE("server.eclipse", "LuaState reset");
        }
    }

    void LuaState::EnableOptimizations()
    {
        if (isInitialized)
        {
            luaState["_ECLIPSE_OPTIMIZED"] = true;
            LOG_TRACE("server.eclipse", "LuaState optimizations enabled");
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
        );
    }

    void LuaState::SetupLuaJIT()
    {
#ifdef SOL_LUAJIT
        try 
        {
            lua_State* L = luaState.lua_state();
            lua_getglobal(L, "require");
            lua_pushstring(L, "jit");
            if (lua_pcall(L, 1, 1, 0) == 0) 
            {
                lua_setglobal(L, "jit");
                luaState.script("if jit and jit.opt then jit.opt.start(3) end");
                LOG_TRACE("server.eclipse", "LuaJIT optimizations enabled");
            } 
            else 
            {
                lua_pop(L, 1);
                LOG_DEBUG("server.eclipse", "LuaJIT not available, using standard Lua");
            }
        } 
        catch (const std::exception& e) 
        {
            LOG_DEBUG("server.eclipse", "LuaJIT setup failed: {}", e.what());
        }
#endif
    }
}