#include "LuaBindings.hpp"
#include "Log.h"
#include "PlayerMethods.hpp"
#include "EventManager.hpp"

namespace Eclipse
{
    void LuaBindings::Register(sol::state& lua)
    {       
        // Register player methods
        PlayerMethods::Register(lua);
        
        // Register event system
        EventManager::Register(lua);
    }
}