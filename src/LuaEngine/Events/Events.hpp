#ifndef ECLIPSE_EVENTS_HPP
#define ECLIPSE_EVENTS_HPP

#include "EclipseIncludes.hpp"
#include <type_traits>

namespace Eclipse
{
    enum PlayerEvents : uint32
    {
        PLAYER_EVENT_ON_LOGIN = 1,
        PLAYER_EVENT_ON_LOGOUT = 2,
        PLAYER_EVENT_ON_LOOT_ITEM = 3,
        PLAYER_EVENT_ON_LEVEL_CHANGED = 4,
        PLAYER_EVENT_ON_KILL_PLAYER = 5,
    };

    enum MapEvents : uint32
    {
        MAP_EVENT_ON_UPDATE = 2,
        MAP_EVENT_ON_PLAYER_ENTER = 3,
    };

    enum CreatureEvents : uint32
    {
        CREATURE_EVENT_ON_SPAWN = 1,
        CREATURE_EVENT_ON_JUST_DIED = 2,
        CREATURE_EVENT_ON_KILLED_UNIT = 3,
        CREATURE_EVENT_ON_ENTER_COMBAT = 4,
        CREATURE_EVENT_ON_LEAVE_COMBAT = 5,
    };

    enum GameObjectEvents : uint32
    {
        GAMEOBJECT_EVENT_ON_USE = 1,
        GAMEOBJECT_EVENT_ON_SPAWN = 2,
        GAMEOBJECT_EVENT_ON_DESPAWN = 3,
    };

    enum ItemEvents : uint32
    {
        ITEM_EVENT_ON_USE = 1,
        ITEM_EVENT_ON_EQUIP = 2,
        ITEM_EVENT_ON_REMOVE = 3,
    };

    enum class EventType
    {
        PLAYER,
        MAP,
        CREATURE,
        GAMEOBJECT,
        ITEM
    };

    // Type traits for event type deduction
    template<typename T>
    struct event_type_trait;

    // Type traits for objects that have IDs (keyed events)
    template<typename T>
    struct has_id_trait : std::false_type {};

    template<typename T>
    struct id_getter_trait;

    template<>
    struct event_type_trait<Player*>
    {
        static constexpr EventType type = EventType::PLAYER;
    };

    template<>
    struct event_type_trait<Map*>
    {
        static constexpr EventType type = EventType::MAP;
    };

    template<>
    struct event_type_trait<Creature*>
    {
        static constexpr EventType type = EventType::CREATURE;
    };

    template<>
    struct event_type_trait<GameObject*>
    {
        static constexpr EventType type = EventType::GAMEOBJECT;
    };

    template<>
    struct event_type_trait<Item*>
    {
        static constexpr EventType type = EventType::ITEM;
    };

    // Traits for objects that have IDs
    template<>
    struct has_id_trait<Creature*> : std::true_type {};

    template<>
    struct has_id_trait<GameObject*> : std::true_type {};

    template<>
    struct has_id_trait<Item*> : std::true_type {};

    // ID getters
    template<>
    struct id_getter_trait<Creature*>
    {
        static uint32 get_id(Creature* obj) { return obj->GetEntry(); }
    };

    template<>
    struct id_getter_trait<GameObject*>
    {
        static uint32 get_id(GameObject* obj) { return obj->GetEntry(); }
    };

    template<>
    struct id_getter_trait<Item*>
    {
        static uint32 get_id(Item* obj) { return obj->GetEntry(); }
    };

    // Helper to get event type from first argument
    template<typename T>
    constexpr EventType get_event_type()
    {
        using DecayedType = std::decay_t<T>;
        return event_type_trait<DecayedType>::type;
    }

    // Helper to check if type has ID
    template<typename T>
    constexpr bool has_id()
    {
        using DecayedType = std::decay_t<T>;
        return has_id_trait<DecayedType>::value;
    }

    // Helper to get ID from object
    template<typename T>
    uint32 get_object_id(T obj)
    {
        using DecayedType = std::decay_t<T>;
        static_assert(has_id_trait<DecayedType>::value, "Type does not have ID");
        return id_getter_trait<DecayedType>::get_id(obj);
    }

    // ========== CENTRALIZED TYPE REGISTRATION ==========
    // To add a new event type, just add it to the appropriate list below!
    
    // Non-keyed event types (Player, Map, etc.)
    using BasicEventTypes = std::tuple<Player*, Map*>;
    
    // Keyed event types (Creature, GameObject, Item, etc.)
    using KeyedEventTypes = std::tuple<Creature*, GameObject*, Item*>;
    
    // All event types combined
    using AllEventTypes = std::tuple<Player*, Map*, Creature*, GameObject*, Item*>;

    // Macro to register a complete keyed event type (traits + enum + everything)
    #define REGISTER_KEYED_EVENT_TYPE(Type, EnumValue, EnumName) \
        enum EnumName : uint32; \
        template<> struct event_type_trait<Type*> { static constexpr EventType type = EventType::EnumValue; }; \
        template<> struct has_id_trait<Type*> : std::true_type {}; \
        template<> struct id_getter_trait<Type*> { static uint32 get_id(Type* obj) { return obj->GetEntry(); } };

    // Macro to register a basic (non-keyed) event type
    #define REGISTER_BASIC_EVENT_TYPE(Type, EnumValue) \
        template<> struct event_type_trait<Type*> { static constexpr EventType type = EventType::EnumValue; };
}

#endif // ECLIPSE_EVENTS_HPP