#include "EventManager.hpp"

namespace Eclipse
{
    void EventManager::RegisterPlayerEvent(uint32 eventId, sol::function callback)
    {
        if (callback.valid())
        {
            auto& events = playerEvents[eventId];
            if (events.empty())
                events.reserve(4);
            events.push_back(callback);
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
        lua["PLAYER_EVENT_ON_LOGOUT"] = PLAYER_EVENT_ON_LOGOUT;
        lua["PLAYER_EVENT_ON_LOOT_ITEM"] = PLAYER_EVENT_ON_LOOT_ITEM;
        lua["PLAYER_EVENT_ON_LEVEL_CHANGED"] = PLAYER_EVENT_ON_LEVEL_CHANGED;
        lua["PLAYER_EVENT_ON_KILL_PLAYER"] = PLAYER_EVENT_ON_KILL_PLAYER;
        
        lua["RegisterPlayerEvent"] = [this](uint32 eventId, sol::function callback) {
            this->RegisterPlayerEvent(eventId, callback);
        };

        lua["ClearPlayerEvents"] = [this]() {
            this->ClearPlayerEvents();
        };
    }
}