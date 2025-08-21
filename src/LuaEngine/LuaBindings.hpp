#ifndef ECLIPSE_LUA_BINDINGS_HPP
#define ECLIPSE_LUA_BINDINGS_HPP

#include "sol.hpp"

namespace Eclipse
{
    class LuaBindings
    {
    public:
        static void RegisterCore(sol::state& lua);
        static void RegisterLogging(sol::state& lua);
        
        // Future bindings can be added here
        // static void RegisterPlayer(sol::state& lua);
        // static void RegisterCreature(sol::state& lua);
        // static void RegisterSpell(sol::state& lua);
        
    private:
        LuaBindings() = delete; // Static-only class
    };
}

#endif // ECLIPSE_LUA_BINDINGS_HPP