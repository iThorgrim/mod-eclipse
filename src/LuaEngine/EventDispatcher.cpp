#include "EventDispatcher.hpp"
#include "MapStateManager.hpp"
#include "LuaEngine.hpp"
#include "EventManager.hpp"
#include <unordered_set>
#include <tuple>
#include <type_traits>

namespace Eclipse
{
    EventDispatcher& EventDispatcher::GetInstance()
    {
        static EventDispatcher instance;
        return instance;
    }
    
    template<typename... Args>
    void EventDispatcher::TriggerEvent(uint32 eventId, Args&&... args)
    {
        static_assert(sizeof...(args) > 0, "At least one argument required");
        
        auto firstArg = std::get<0>(std::forward_as_tuple(args...));
        using FirstArgType = std::decay_t<decltype(firstArg)>;
        
        auto engines = GetRelevantEngines(firstArg);
        TriggerOnEngines<FirstArgType>(engines, eventId, std::forward<Args>(args)...);
    }
    
    
    template<typename T>
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(T* object)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(2);
        
        auto& manager = MapStateManager::GetInstance();
        
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.push_back(globalEngine);
        }
        
        if (object)
        {
            Map* objectMap = nullptr;
            
            if constexpr (std::is_same_v<T, Player>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, Map>)
            {
                objectMap = object;
            }
            else if constexpr (std::is_same_v<T, Creature>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, GameObject>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, Item>)
            {
                if (Player* owner = object->GetOwner())
                {
                    objectMap = owner->GetMap();
                }
            }
            
            if (objectMap)
            {
                uint32 mapId = objectMap->GetId();
                auto* mapEngine = manager.GetStateForMap(mapId);
                
                if (mapEngine && mapEngine != globalEngine)
                {
                    engines.push_back(mapEngine);
                }
            }
        }
        
        return engines;
    }
    
    template<typename T, typename... Args>
    void EventDispatcher::TriggerOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Args&&... args)
    {
        for (auto* engine : engines)
        {
            if (engine && engine->GetEventManager())
            {
                engine->GetEventManager()->TriggerEvent(eventId, std::forward<Args>(args)...);
            }
        }
    }

    template<EventType Type, typename... Args>
    void EventDispatcher::TriggerKeyedEventHelper(const std::vector<LuaEngine*>& engines, uint32 objectId, uint32 eventId, Args&&... args)
    {
        for (auto* engine : engines)
        {
            if (engine && engine->GetEventManager())
            {
                engine->GetEventManager()->template TriggerKeyedEvent<Type>(objectId, eventId, std::forward<Args>(args)...);
            }
        }
    }

    template<typename... Args>
    void EventDispatcher::TriggerKeyedEvent(uint32 eventId, Args&&... args)
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
}

namespace Eclipse {
    namespace detail {
        template<typename Tuple, std::size_t I = 0>
        struct BasicEventInstantiator {
            static void instantiate() {
                if constexpr (I < std::tuple_size_v<Tuple>) {
                    using Type = typename std::tuple_element<I, Tuple>::type;
                    using CleanType = std::remove_pointer_t<Type>;
                    
                    (void)&EventDispatcher::GetRelevantEngines<CleanType>;
                    BasicEventInstantiator<Tuple, I + 1>::instantiate();
                }
            }
        };
        
        template<typename Tuple, std::size_t I = 0>
        struct KeyedEventInstantiator {
            static void instantiate() {
                if constexpr (I < std::tuple_size_v<Tuple>) {
                    using Type = typename std::tuple_element<I, Tuple>::type;
                    using CleanType = std::remove_pointer_t<Type>;
                    
                    if constexpr (has_id_trait<Type>::value) {
                        (void)&EventDispatcher::GetRelevantEngines<CleanType>;
                        [[maybe_unused]] constexpr auto eventType = get_event_type<Type>();
                    }
                    
                    KeyedEventInstantiator<Tuple, I + 1>::instantiate();
                }
            }
        };
        
        [[maybe_unused]] static auto basic_instantiator = BasicEventInstantiator<BasicEventTypes>::instantiate;
        [[maybe_unused]] static auto keyed_instantiator = KeyedEventInstantiator<KeyedEventTypes>::instantiate;
    }
}

// Explicit template instantiations (auto-generated via macros)
#define INSTANTIATE_BASIC_EVENT(Type) \
    template std::vector<Eclipse::LuaEngine*> Eclipse::EventDispatcher::GetRelevantEngines<Type>(Type*); \
    template void Eclipse::EventDispatcher::TriggerEvent<Type*&>(uint32, Type*&); \
    template void Eclipse::EventDispatcher::TriggerEvent<Type*&, Player*&>(uint32, Type*&, Player*&);

#define INSTANTIATE_KEYED_EVENT(Type, EnumValue) \
    template std::vector<Eclipse::LuaEngine*> Eclipse::EventDispatcher::GetRelevantEngines<Type>(Type*); \
    template void Eclipse::EventDispatcher::TriggerKeyedEvent<Type*&>(uint32, Type*&); \
    template void Eclipse::EventDispatcher::TriggerKeyedEvent<Type*&, Player*&>(uint32, Type*&, Player*&); \
    template void Eclipse::EventDispatcher::TriggerKeyedEventHelper<Eclipse::EventType::EnumValue, Type*&>(const std::vector<Eclipse::LuaEngine*>&, uint32, uint32, Type*&); \
    template void Eclipse::EventDispatcher::TriggerKeyedEventHelper<Eclipse::EventType::EnumValue, Type*&, Player*&>(const std::vector<Eclipse::LuaEngine*>&, uint32, uint32, Type*&, Player*&);

// Basic events (auto-detected from BasicEventTypes tuple)
INSTANTIATE_BASIC_EVENT(Player)
INSTANTIATE_BASIC_EVENT(Map)

// Keyed events (auto-detected from KeyedEventTypes tuple)
INSTANTIATE_KEYED_EVENT(Creature, CREATURE)
INSTANTIATE_KEYED_EVENT(GameObject, GAMEOBJECT)
INSTANTIATE_KEYED_EVENT(Item, ITEM)

template void Eclipse::EventDispatcher::TriggerEvent<Player*&, Item*&, uint32&, ObjectGuid&>(uint32, Player*&, Item*&, uint32&, ObjectGuid&);
template void Eclipse::EventDispatcher::TriggerEvent<Player*&, uint8&>(uint32, Player*&, uint8&);
template void Eclipse::EventDispatcher::TriggerEvent<Map*&, uint32&>(uint32, Map*&, uint32&);

// Additional instantiations for EclipseCreatureAI
template void Eclipse::EventDispatcher::TriggerKeyedEvent<Creature*&, Unit*&>(uint32, Creature*&, Unit*&);
template void Eclipse::EventDispatcher::TriggerKeyedEventHelper<Eclipse::EventType::CREATURE, Creature*&, Unit*&>(const std::vector<Eclipse::LuaEngine*>&, uint32, uint32, Creature*&, Unit*&);

#undef INSTANTIATE_BASIC_EVENT
#undef INSTANTIATE_KEYED_EVENT

