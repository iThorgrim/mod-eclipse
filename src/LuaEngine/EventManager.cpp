#include "EventManager.hpp"
#include "Log.h"

namespace Eclipse
{
    EventManager& EventManager::GetInstance()
    {
        static EventManager instance;
        return instance;
    }

    void EventManager::RegisterPlayerEvent(uint32 eventId, sol::function callback)
    {
        if (callback.valid())
        {
            playerEvents[eventId].push_back(callback);
        }
    }

    void EventManager::ClearAllEvents()
    {
        ClearPlayerEvents();
    }

    void EventManager::ClearPlayerEvents()
    {
        playerEvents.clear();
    }

    void EventManager::Register(sol::state& lua)
    {
        // Register Player Event constants
        lua["PLAYER_EVENT_ON_LOGIN"] = PLAYER_EVENT_ON_LOGIN;
        
        // Register event functions
        lua["RegisterPlayerEvent"] = [](uint32 eventId, sol::function callback) {
            EventManager::GetInstance().RegisterPlayerEvent(eventId, callback);
        };

        // Utility functions
        lua["ClearPlayerEvents"] = []() {
            EventManager::GetInstance().ClearPlayerEvents();
        };
    }
}