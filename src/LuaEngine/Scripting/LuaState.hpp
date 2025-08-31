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

        bool Initialize();
        void Reset();
        
        sol::state& GetState() { return luaState; }
        const sol::state& GetState() const { return luaState; }
        
        void EnableOptimizations();
        void ConfigureLibraries();
        
        bool IsValid() const { return isInitialized; }
        
    private:
        sol::state luaState;
        bool isInitialized;
        
        void OpenStandardLibraries();
        void SetupLuaJIT();
    };
}

#endif // ECLIPSE_LUA_STATE_HPP