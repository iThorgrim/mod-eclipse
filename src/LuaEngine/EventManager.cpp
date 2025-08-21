#include "EventManager.hpp"
#include "Log.h"

namespace Eclipse
{

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
        lua["PLAYER_EVENT_ON_LOGIN"] = PLAYER_EVENT_ON_LOGIN;
        
        lua["RegisterPlayerEvent"] = [this](uint32 eventId, sol::function callback) {
            this->RegisterPlayerEvent(eventId, callback);
        };

        lua["ClearPlayerEvents"] = [this]() {
            this->ClearPlayerEvents();
        };
    }
}