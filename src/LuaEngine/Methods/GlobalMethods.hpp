#ifndef ECLIPSE_GLOBAL_METHODS_HPP
#define ECLIPSE_GLOBAL_METHODS_HPP

#include "LuaEngine.hpp"
#include "MessageManager.hpp"
#include "ObjectGuid.h"
#include "ObjectAccessor.h"

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
         * Get the current map ID from the Lua state
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

        /**
         *
         */
        inline Player* FindPlayer(LuaEngine* lua, ObjectGuid const& guid)
        {
            (void)lua;
            return ObjectAccessor::FindConnectedPlayer(guid);
        }

        /**
         *
         */
        inline Player* FindPlayerByLowGUID(LuaEngine* lua, ObjectGuid::LowType lowguid)
        {
            (void)lua;
            return ObjectAccessor::FindPlayerByLowGUID(lowguid);
        }

        /**
         *
         */
        inline Player* FindPlayerByName(LuaEngine* lua, std::string const& name, bool checkInWorld = true)
        {
            (void)lua;
            return ObjectAccessor::FindPlayerByName(name, checkInWorld);
        }

        /**
         *
         */
        inline Creature* GetSpawnedCreatureByDBGUID(LuaEngine* lua, uint32 mapId, uint64 guid)
        {
            (void)lua;
            return ObjectAccessor::GetSpawnedCreatureByDBGUID(mapId, guid);
        }

        /**
         *
         */
        inline GameObject* GetSpawnedGameObjectByDBGUID(LuaEngine* lua, uint32 mapId, uint64 guid)
        {
            (void)lua;
            return ObjectAccessor::GetSpawnedGameObjectByDBGUID(mapId, guid);
        }

        /**
         *
         */
        inline sol::table GetPlayers(LuaEngine* lua)
        {
            sol::table players = lua->GetState().create_table();
            auto const& playerMap = ObjectAccessor::GetPlayers();

            int index = 1;
            for (auto const& pair : playerMap)
            {
                if (pair.second && pair.second->IsInWorld())
                {
                    players[index] = pair.second;
                    ++index;
                }
            }

            return players;
        }

        inline void SaveAllPlayers(LuaEngine* lua)
        {
            (void)lua;
            ObjectAccessor::SaveAllPlayers();
        }

        // ========== LUA REGISTRATION ==========
        void Register(LuaEngine* lua_engine, sol::state& lua)
        {
            // Getters
            lua["GetStateMapId"] = Bind(&GetStateMapId, lua_engine);
            lua["GetSpawnedCreatureByDBGUID"] = Bind(&GetSpawnedCreatureByDBGUID, lua_engine);
            lua["GetSpawnedGameObjectByDBGUID"] = Bind(&GetSpawnedGameObjectByDBGUID, lua_engine);
            lua["GetPlayers"] = Bind(&GetPlayers, lua_engine);

            lua["FindPlayer"] = Bind(&FindPlayer, lua_engine);
            lua["FindPlayerByLowGUID"] = Bind(&FindPlayerByLowGUID, lua_engine);
            lua["FindPlayerByName"] = Bind(&FindPlayerByName, lua_engine);
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

            lua["SaveAllPlayers"] = Bind(&SaveAllPlayers, lua_engine);
        }
    }
}

#endif // ECLIPSE_GLOBAL_METHODS_HPP