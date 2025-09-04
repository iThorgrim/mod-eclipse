#ifndef ECLIPSE_GLOBAL_METHODS_HPP
#define ECLIPSE_GLOBAL_METHODS_HPP

#include "LuaEngine.hpp"
#include "MessageManager.hpp"
#include "ObjectGuid.h"

namespace Eclipse
{
    namespace GlobalMethods
    {
        template <typename Ret, typename... Args>
        auto Bind(Ret(*func)(LuaEngine*, Args...), LuaEngine* engine) {
            return [engine, func](Args&&... args) -> Ret {
                return func(engine, std::forward<Args>(args)...);
            };
        }

        /**
         *
         */
        inline int32 GetStateMapId(LuaEngine* lua)
        {
            return lua->GetStateMapId();
        }

        /**
         *
         */
        inline void SendStateMessage(LuaEngine* lua, int32 toStateId, const std::string& messageType, sol::object data)
        {
            MessageManager::GetInstance().SendMessage(lua->GetStateMapId(), toStateId, messageType, data);
        }

        /**
         *
         */
        inline void RegisterStateMessage(LuaEngine* lua, const std::string& messageType, sol::function callback)
        {
            MessageManager::GetInstance().RegisterMessageEvent(lua->GetStateMapId(), messageType, callback);
        }

        /**
         *
         */
        inline void RegisterPlayerEvent(LuaEngine* lua, uint32 eventId, sol::function callback)
        {
            lua->GetEventManager()->RegisterEvent<EventType::PLAYER>(eventId, callback);
        }

        /**
         *
         */
        inline void ClearPlayerEvents(LuaEngine* lua)
        {
            lua->GetEventManager()->ClearEvents<EventType::PLAYER>();
        }

        /**
         *
         */
        inline void RegisterMapEvent(LuaEngine* lua, uint32 eventId, sol::function callback)
        {
            lua->GetEventManager()->RegisterEvent<EventType::MAP>(eventId, callback);
        }

        /**
         *
         */
        inline void ClearMapEvents(LuaEngine* lua)
        {
            lua->GetEventManager()->ClearEvents<EventType::MAP>();
        }

        /**
         *
         */
        inline void RegisterCreatureEvent(LuaEngine* lua, uint32 objectId, uint32 eventId, sol::function callback)
        {
            lua->GetEventManager()->RegisterKeyedEvent<EventType::CREATURE>(objectId, eventId, callback);
        }

        /**
         *
         */
        inline void ClearCreatureEvents(LuaEngine* lua)
        {
            lua->GetEventManager()->ClearKeyedEvents<EventType::CREATURE>();
        }

        /**
         *
         */
        inline void RegisterGameObjectEvent(LuaEngine* lua, uint32 objectId, uint32 eventId, sol::function callback)
        {
            lua->GetEventManager()->RegisterKeyedEvent<EventType::GAMEOBJECT>(objectId, eventId, callback);
        }

        /**
         *
         */
        inline void ClearGameObjectEvents(LuaEngine* lua)
        {
            lua->GetEventManager()->ClearKeyedEvents<EventType::GAMEOBJECT>();
        }

        /**
         *
         */
        inline void RegisterItemEvent(LuaEngine* lua, uint32 objectId, uint32 eventId, sol::function callback)
        {
            lua->GetEventManager()->RegisterKeyedEvent<EventType::ITEM>(objectId, eventId, callback);
        }

        /**
         *
         */
        inline void ClearItemEvents(LuaEngine* lua)
        {
            lua->GetEventManager()->ClearKeyedEvents<EventType::ITEM>();
        }

        /**
         *
         */
        inline ObjectGuid CreateGuidFromRaw(LuaEngine* lua, uint64 raw)
        {
            (void)lua;
            return ObjectGuid(raw);
        }

        // ========== LUA REGISTRATION ==========
        void Register(LuaEngine* lua_engine, sol::state& lua)
        {
            // Getters
            lua["GetStateMapId"] = Bind(&GetStateMapId, lua_engine);

            // Setters

            // Booleans

            // Actions
            lua["RegisterStateMessage"] = Bind(&RegisterStateMessage, lua_engine);
            lua["SendStateMessage"] = Bind(&SendStateMessage, lua_engine);
            lua["RegisterPlayerEvent"] = Bind(&RegisterPlayerEvent, lua_engine);
            lua["ClearPlayerEvents"] = Bind(&ClearPlayerEvents, lua_engine);
            lua["RegisterMapEvent"] = Bind(&RegisterMapEvent, lua_engine);
            lua["ClearMapEvents"] = Bind(&ClearMapEvents, lua_engine);
            lua["RegisterCreatureEvent"] = Bind(&RegisterCreatureEvent, lua_engine);
            lua["ClearCreatureEvents"] = Bind(&ClearCreatureEvents, lua_engine);
            lua["RegisterGameObjectEvent"] = Bind(&RegisterGameObjectEvent, lua_engine);
            lua["ClearGameObjectEvents"] = Bind(&ClearGameObjectEvents, lua_engine);
            lua["RegisterItemEvent"] = Bind(&RegisterItemEvent, lua_engine);
            lua["ClearItemEvents"] = Bind(&ClearItemEvents, lua_engine);
            lua["CreateGuidFromRaw"] = Bind(&CreateGuidFromRaw, lua_engine);
        }
    }
}

#endif // ECLIPSE_GLOBAL_METHODS_HPP