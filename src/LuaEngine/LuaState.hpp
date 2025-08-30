#ifndef ECLIPSE_LUA_STATE_HPP
#define ECLIPSE_LUA_STATE_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    class LuaState
    {
    public:
        LuaState();
        ~LuaState() = default;

        // Single responsibility: Lua state management
        bool Initialize();
        void Reset();
        
        // State access
        sol::state& GetState() { return luaState; }
        const sol::state& GetState() const { return luaState; }
        
        // State configuration
        void EnableOptimizations();
        void ConfigureLibraries();
        
        // State validation
        bool IsValid() const { return isInitialized; }
        
    private:
        sol::state luaState;
        bool isInitialized;
        
        // Internal initialization helpers
        void OpenStandardLibraries();
        void SetupLuaJIT();
    };
}

#endif // ECLIPSE_LUA_STATE_HPP