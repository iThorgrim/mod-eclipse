#ifndef ECLIPSE_LUA_BINDINGS_HPP
#define ECLIPSE_LUA_BINDINGS_HPP

#include "sol.hpp"

namespace Eclipse
{
    class EventManager; // Forward declaration
    
    class LuaBindings
    {
    public:
        static void Register(sol::state& lua, EventManager* eventMgr);        
        
    private:
        LuaBindings() = delete;
    };
}

#endif // ECLIPSE_LUA_BINDINGS_HPP