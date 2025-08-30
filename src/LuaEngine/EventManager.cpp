#include "EventManager.hpp"
#include <numeric>

namespace Eclipse
{
    void EventManager::ClearAllEvents()
    {
        ClearEvents<EventType::PLAYER>();
        ClearEvents<EventType::MAP>();
        ClearKeyedEvents<EventType::CREATURE>();
        ClearKeyedEvents<EventType::GAMEOBJECT>();
        ClearKeyedEvents<EventType::ITEM>();
    }

    void EventManager::Register(sol::state& lua)
    {       
        // === BASIC EVENTS (non-keyed) ===
        lua["RegisterPlayerEvent"] = [this](uint32 eventId, sol::function callback) {
            this->RegisterEvent<EventType::PLAYER>(eventId, callback);
        };
        lua["ClearPlayerEvents"] = [this]() {
            this->ClearEvents<EventType::PLAYER>();
        };

        lua["RegisterMapEvent"] = [this](uint32 eventId, sol::function callback) {
            this->RegisterEvent<EventType::MAP>(eventId, callback);
        };
        lua["ClearMapEvents"] = [this]() {
            this->ClearEvents<EventType::MAP>();
        };

        // === KEYED EVENTS ===
        lua["RegisterCreatureEvent"] = [this](uint32 objectId, uint32 eventId, sol::function callback) {
            this->RegisterKeyedEvent<EventType::CREATURE>(objectId, eventId, callback);
        };
        lua["ClearCreatureEvents"] = [this]() {
            this->ClearKeyedEvents<EventType::CREATURE>();
        };

        lua["RegisterGameObjectEvent"] = [this](uint32 objectId, uint32 eventId, sol::function callback) {
            this->RegisterKeyedEvent<EventType::GAMEOBJECT>(objectId, eventId, callback);
        };
        lua["ClearGameObjectEvents"] = [this]() {
            this->ClearKeyedEvents<EventType::GAMEOBJECT>();
        };

        lua["RegisterItemEvent"] = [this](uint32 objectId, uint32 eventId, sol::function callback) {
            this->RegisterKeyedEvent<EventType::ITEM>(objectId, eventId, callback);
        };
        lua["ClearItemEvents"] = [this]() {
            this->ClearKeyedEvents<EventType::ITEM>();
        };

        // === FUTURE: AUTO-GENERATED BINDINGS ===
        // TODO: Use template magic to auto-generate bindings from the tuples in Events.hpp
        // This would eliminate the need to manually add each binding
    }
}