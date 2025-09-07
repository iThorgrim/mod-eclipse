#ifndef ECLIPSE_EVENT_DISPATCHER_HPP
#define ECLIPSE_EVENT_DISPATCHER_HPP

#include "EclipseIncludes.hpp"
#include "Events.hpp"
#include "MapStateManager.hpp"
#include "LuaEngine.hpp"
#include "EventManager.hpp"
#include <span>
#include <vector>

namespace Eclipse
{
    class EventDispatcher
    {
    public:
        static EventDispatcher& GetInstance()
        {
            static EventDispatcher instance;
            return instance;
        }

        /**
         * Trigger event with automatic type detection and optimal routing
         */
        template<typename... Args>
        requires(sizeof...(Args) > 0 && SupportedEventObject<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>)
        void TriggerEvent(uint32 eventId, Args&&... args)
        {
            auto firstArg = std::get<0>(std::forward_as_tuple(args...));
            using FirstArgType = std::decay_t<decltype(firstArg)>;

            if constexpr (std::is_same_v<FirstArgType, ObjectGuid>)
            {
                // Runtime type detection for ObjectGuid
                auto engines = GetRelevantEnginesForObjectGuid(firstArg);
                TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
            else
            {
                auto engines = GetRelevantEngines(firstArg);
                TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
        }

        /**
         * Trigger event that can block actions (returns false to prevent)
         */
        template<typename... Args>
        requires(sizeof...(Args) > 0 && SupportedEventObject<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>)
        std::optional<std::any> TriggerWithRetValueEvent(uint32 eventId, Args&&... args)
        {
            auto firstArg = std::get<0>(std::forward_as_tuple(args...));
            using FirstArgType = std::decay_t<decltype(firstArg)>;

            if constexpr (std::is_same_v<FirstArgType, ObjectGuid>)
            {
                auto engines = GetRelevantEnginesForObjectGuid(firstArg);
                return TriggerWithRetValueOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
            else
            {
                auto engines = GetRelevantEngines(firstArg);
                return TriggerWithRetValueOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
        }

        /**
         * Trigger keyed event (events bound to specific object IDs)
         */
        template<KeyedEventObject T, typename... Args>
        void TriggerKeyedEvent(uint32 objectId, uint32 eventId, Args&&... args)
        {
            constexpr auto eventType = get_event_type<T>();
            auto engines = GetAllRelevantEngines();
            for (auto* engine : engines)
            {
                if (auto* eventManager = engine->GetEventManager())
                {
                    eventManager->template TriggerKeyedEvent<eventType>(objectId, eventId, std::forward<Args>(args)...);
                }
            }
        }

    private:
        EventDispatcher() = default;
        ~EventDispatcher() = default;
        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;

        /**
         * Get relevant engines for pointer-based objects (Player*, Creature*, etc.)
         */
        template<typename T>
        std::span<LuaEngine* const> GetRelevantEngines(T* object)
        {
            thread_local static std::vector<LuaEngine*> engines;
            engines.clear();
            engines.reserve(2);

            auto& manager = MapStateManager::GetInstance();

            // Always include global engine
            if (auto* globalEngine = manager.GetGlobalState())
            {
                engines.emplace_back(globalEngine);
            }

            // Add map-specific engine if applicable
            if (object)
            {
                if (auto* objectMap = GetObjectMap(object))
                {
                    if (auto* mapEngine = manager.GetStateForMap(objectMap->GetId()))
                    {
                        if (mapEngine != engines.front()) // Avoid duplicates
                        {
                            engines.emplace_back(mapEngine);
                        }
                    }
                }
            }

            return std::span<LuaEngine* const>(engines);
        }

        /**
         * Get relevant engines for ObjectGuid with runtime type detection
         */
        std::span<LuaEngine* const> GetRelevantEnginesForObjectGuid(const ObjectGuid& guid)
        {
            thread_local static std::vector<LuaEngine*> engines;
            engines.clear();
            engines.reserve(2);

            auto& manager = MapStateManager::GetInstance();
            Map* objectMap = nullptr;

            if (auto* globalEngine = manager.GetGlobalState())
            {
                engines.emplace_back(globalEngine);
            }

            if (guid.IsPlayer())
            {
                if (auto* player = ObjectAccessor::FindPlayer(guid))
                {
                    objectMap = player->GetMap();
                }
            }
            else if (guid.IsAnyTypeCreature() || guid.IsAnyTypeGameObject())
            {
                sMapMgr->DoForAllMaps([&](Map* map) {
                    if (objectMap) return;

                    WorldObject* obj = nullptr;

                    if (guid.IsAnyTypeCreature())
                    {
                        obj = map->GetCreature(guid);
                    }
                    else if (guid.IsAnyTypeGameObject())
                    {
                        obj = map->GetGameObject(guid);
                    }

                    if (obj)
                    {
                        objectMap = map;
                    }
                });
            }

            if (objectMap)
            {
                if (auto* mapEngine = manager.GetStateForMap(objectMap->GetId()))
                {
                    if (mapEngine != engines.front())
                    {
                        engines.emplace_back(mapEngine);
                    }
                }
            }

            return std::span<LuaEngine* const>(engines);
        }

        /**
         * Get relevant engines for wrapper types (PlayerGuid, CreatureEntry, etc.)
         */
        template<typename T>
        requires std::is_same_v<T, PlayerGuid> || std::is_same_v<T, CreatureEntry> ||
                 std::is_same_v<T, GameObjectEntry> || std::is_same_v<T, ItemEntry> ||
                 std::is_same_v<T, MapId>
        std::span<LuaEngine* const> GetRelevantEngines(const T& wrapper)
        {
            thread_local static std::vector<LuaEngine*> engines;
            engines.clear();
            engines.reserve(2);

            auto& manager = MapStateManager::GetInstance();

            // Always include global engine
            if (auto* globalEngine = manager.GetGlobalState())
            {
                engines.emplace_back(globalEngine);
            }

            if constexpr (std::is_same_v<T, MapId>)
            {
                if (auto* mapEngine = manager.GetStateForMap(wrapper.mapId))
                {
                    if (mapEngine != engines.front()) // Avoid duplicates
                    {
                        engines.emplace_back(mapEngine);
                    }
                }
            }

            return std::span<LuaEngine* const>(engines);
        }

        /**
         * Get all active engines (used for keyed events)
         */
        std::span<LuaEngine* const> GetAllRelevantEngines()
        {
            thread_local static std::vector<LuaEngine*> engines;

            auto& manager = MapStateManager::GetInstance();

            manager.FillActiveEngines(engines);

            return std::span<LuaEngine* const>(engines);
        }

        template<typename T>
        Map* GetObjectMap(T* object)
        {
            if constexpr (std::is_same_v<T, Player>) {
                return object->IsInWorld() ? object->GetMap() : nullptr;
            }
            else if constexpr (std::is_same_v<T, Map>) {
                return object;
            }
            else if constexpr (std::is_same_v<T, Item>) {
                return get_map(object);
            }
            else if constexpr (can_get_map<T>()) {
                return get_map(object);
            }
            else {
                return nullptr;
            }
        }

        template<typename FirstArgType, typename... Args>
        void TriggerOnEngines(std::span<LuaEngine* const> engines, uint32 eventId, Args&&... args)
        {
            for (auto* engine : engines)
            {
                if (auto* eventManager = engine->GetEventManager())
                {
                    // Check if there are callbacks to avoid unnecessary work
                    if (eventManager->template HasCallbacksFor<Args...>(eventId))
                    {
                        eventManager->TriggerEvent(eventId, std::forward<Args>(args)...);
                    }
                }
            }
        }

        template<typename FirstArgType, typename... Args>
        std::optional<std::any> TriggerWithRetValueOnEngines(std::span<LuaEngine* const> engines, uint32 eventId, Args&&... args)
        {
            for (auto* engine : engines)
            {
                if (auto* eventManager = engine->GetEventManager())
                {
                    if (eventManager->template HasCallbacksFor<Args...>(eventId))
                    {
                        auto result = eventManager->TriggerWithRetValueEvent(eventId, std::forward<Args>(args)...);
                        if (result)
                        {
                            return result;
                        }
                    }
                }
            }
            return std::nullopt; // No callbacks returned anything
        }
    };
}

#endif // ECLIPSE_EVENT_DISPATCHER_HPP