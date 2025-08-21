#include "LuaBindings.hpp"
#include "Methods/PlayerMethods.hpp"
#include "EventManager.hpp"

namespace Eclipse
{
    void LuaBindings::Register(sol::state& lua, EventManager* eventMgr)
    {       
        lua["GetStateMapId"] = []() -> int32 {
            return -1; // Will be overridden by LuaEngine
        };
        
        PlayerMethods::Register(lua);
        
        if (eventMgr)
        {
            eventMgr->Register(lua);
        }
    }
}