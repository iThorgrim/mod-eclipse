#ifndef ECLIPSE_EVENT_DISPATCHER_HPP
#define ECLIPSE_EVENT_DISPATCHER_HPP

#include "EclipseIncludes.hpp"
#include "Events.hpp"
#include "MapStateManager.hpp"
#include "LuaEngine.hpp"
#include "EventManager.hpp"
#include <functional>
#include <unordered_set>
#include <tuple>
#include <type_traits>

namespace Eclipse
{
    class EventDispatcher
    {
    public:
        static EventDispatcher& GetInstance();

        template<typename... Args>
        void TriggerEvent(uint32 eventId, Args&&... args)
        {
            static_assert(sizeof...(args) > 0, "At least one argument required");

            auto firstArg = std::get<0>(std::forward_as_tuple(args...));
            using FirstArgType = std::decay_t<decltype(firstArg)>;

            if constexpr (std::is_same_v<FirstArgType, ObjectGuid>)
            {
                auto engines = GetRelevantEngines<ObjectGuid>(nullptr);
                TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
            else if constexpr (std::is_same_v<FirstArgType, PlayerGuid> ||
                               std::is_same_v<FirstArgType, CreatureEntry> ||
                               std::is_same_v<FirstArgType, GameObjectEntry> ||
                               std::is_same_v<FirstArgType, ItemEntry> ||
                               std::is_same_v<FirstArgType, MapId>)
            {
                auto engines = GetRelevantEngines(firstArg);
                TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
            else
            {
                auto engines = GetRelevantEngines(firstArg);
                TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
        }

        template<typename... Args>
        bool TriggerWithRetValueEvent(uint32 eventId, Args&&... args)
        {
            static_assert(sizeof...(args) > 0, "At least one argument required");

            auto firstArg = std::get<0>(std::forward_as_tuple(args...));
            using FirstArgType = std::decay_t<decltype(firstArg)>;

            if constexpr (std::is_same_v<FirstArgType, ObjectGuid>)
            {
                auto engines = GetRelevantEngines<ObjectGuid>(nullptr);
                return TriggerWithRetValueOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
            else
            {
                auto engines = GetRelevantEngines(firstArg);
                return TriggerWithRetValueOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
            }
        }

        template<typename... Args>
        void TriggerKeyedEvent(uint32 eventId, Args&&... args)
        {
            static_assert(sizeof...(args) > 0, "At least one argument required");

            auto firstArg = std::get<0>(std::forward_as_tuple(args...));
            using FirstArgType = std::decay_t<decltype(firstArg)>;

            if (firstArg)
            {
                if constexpr (has_id<FirstArgType>())
                {
                    uint32 objectId = get_object_id(firstArg);
                    auto engines = GetRelevantEngines(firstArg);

                    if constexpr (std::is_same_v<FirstArgType, Creature*>)
                    {
                        TriggerKeyedEventHelper<EventType::CREATURE>(engines, objectId, eventId, std::forward<Args>(args)...);
                    }
                    else if constexpr (std::is_same_v<FirstArgType, GameObject*>)
                    {
                        TriggerKeyedEventHelper<EventType::GAMEOBJECT>(engines, objectId, eventId, std::forward<Args>(args)...);
                    }
                    else if constexpr (std::is_same_v<FirstArgType, Item*>)
                    {
                        TriggerKeyedEventHelper<EventType::ITEM>(engines, objectId, eventId, std::forward<Args>(args)...);
                    }
                }
            }
        }

    private:
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        template<typename T>
        std::vector<LuaEngine*> GetRelevantEngines(T* object);

        template<typename T, typename... Args>
        void TriggerOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Args&&... args)
        {
            for (auto* engine : engines)
            {
                if (engine && engine->GetEventManager())
                {
                    if (engine->GetEventManager()->template HasCallbacksFor<Args...>(eventId))
                    {
                        engine->GetEventManager()->TriggerEvent(eventId, std::forward<Args>(args)...);
                    }
                }
            }
        }

        template<typename T, typename... Args>
        bool TriggerWithRetValueOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Args&&... args)
        {
            const size_t engineCount = engines.size();

            if (engineCount == 0)
            {
                return true;
            }

            for (int i = static_cast<int>(engineCount) - 1; i >= 0; --i)
            {
                auto* engine = engines[i];
                if (engine && engine->GetEventManager())
                {
                    if (engine->GetEventManager()->template HasCallbacksFor<Args...>(eventId))
                    {
                        return engine->GetEventManager()->TriggerWithRetValueEvent(eventId, std::forward<Args>(args)...);
                    }
                }
            }

            return true;
        }

        template<EventType Type, typename... Args>
        void TriggerKeyedEventHelper(const std::vector<LuaEngine*>& engines, uint32 objectId, uint32 eventId, Args&&... args)
        {
            for (auto* engine : engines)
            {
                if (engine && engine->GetEventManager())
                {
                    engine->GetEventManager()->template TriggerKeyedEvent<Type>(objectId, eventId, std::forward<Args>(args)...);
                }
            }
        }

        // Specializations for wrapper types
        std::vector<LuaEngine*> GetRelevantEngines(const PlayerGuid& playerGuid);
        std::vector<LuaEngine*> GetRelevantEngines(const CreatureEntry& creatureEntry);
        std::vector<LuaEngine*> GetRelevantEngines(const GameObjectEntry& gameObjectEntry);
        std::vector<LuaEngine*> GetRelevantEngines(const ItemEntry& itemEntry);
        std::vector<LuaEngine*> GetRelevantEngines(const MapId& mapId);
    };
}

#endif // ECLIPSE_EVENT_DISPATCHER_HPP