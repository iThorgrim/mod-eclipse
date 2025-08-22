#include "LuaBindings.hpp"
#include "EventManager.hpp"
#include "PlayerMethods.hpp"

namespace Eclipse
{
    void LuaBindings::Register(sol::state& lua, EventManager* eventMgr)
    {
        PlayerMethods::Register(lua);
        
        if (eventMgr)
        {
            eventMgr->Register(lua);
        }
    }
}