#ifndef ECLIPSE_LUA_BINDINGS_HPP
#define ECLIPSE_LUA_BINDINGS_HPP

#include "sol.hpp"

namespace Eclipse
{
    class LuaBindings
    {
    public:
        static void Register(sol::state& lua);        
        
    private:
        LuaBindings() = delete;
    };
}

#endif // ECLIPSE_LUA_BINDINGS_HPP