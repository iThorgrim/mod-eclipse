#ifndef ECLIPSE_EVENT_TRAITS_HPP
#define ECLIPSE_EVENT_TRAITS_HPP

#include "EventTypes.hpp"
#include <type_traits>
#include <concepts>

namespace Eclipse
{
    // ========== CONCEPTS FOR TYPE DETECTION ==========
    template<typename T>
    concept HasGetEntry = requires(T* obj) {
        { obj->GetEntry() } -> std::convertible_to<uint32>;
    };

    template<typename T>
    concept HasGetMap = requires(T* obj) {
        { obj->GetMap() } -> std::convertible_to<Map*>;
    };

    template<typename T>
    concept HasGetOwner = requires(T* obj) {
        { obj->GetOwner() } -> std::convertible_to<Player*>;
    };

    // ========== CORE TRAIT SYSTEM ==========
    template<typename T>
    struct event_type_trait;

    template<typename T>
    struct has_entry_trait : std::bool_constant<HasGetEntry<T>> {};

    template<typename T>
    struct can_get_map_trait : std::bool_constant<HasGetMap<T>> {};

    // ========== AUTOMATIC TYPE REGISTRATION MACRO ==========
    #define ECLIPSE_REGISTER_TYPE(Type, EventTypeValue) \
        template<> struct event_type_trait<Type*> { \
            static constexpr EventType type = EventType::EventTypeValue; \
        }; \
        template<> struct event_type_trait<const Type*> { \
            static constexpr EventType type = EventType::EventTypeValue; \
        };

    #define ECLIPSE_REGISTER_KEYED_TYPE(Type, EventTypeValue) \
        ECLIPSE_REGISTER_TYPE(Type, EventTypeValue) \
        template<> struct has_entry_trait<Type> : std::true_type {}; \
        static_assert(HasGetEntry<Type>, #Type " must have GetEntry() method for keyed events");

    #define ECLIPSE_REGISTER_WRAPPER_TYPE(WrapperType, EventTypeValue) \
        template<> struct event_type_trait<WrapperType> { \
            static constexpr EventType type = EventType::EventTypeValue; \
        };

    // ========== TYPE REGISTRATIONS ==========
    // Basic object types
    ECLIPSE_REGISTER_TYPE(Player, PLAYER)
    ECLIPSE_REGISTER_TYPE(Map, MAP)

    // Keyed types (have GetEntry())
    ECLIPSE_REGISTER_KEYED_TYPE(Creature, CREATURE)
    ECLIPSE_REGISTER_KEYED_TYPE(GameObject, GAMEOBJECT)
    ECLIPSE_REGISTER_KEYED_TYPE(Item, ITEM)

    // Wrapper types for type-safe routing
    ECLIPSE_REGISTER_WRAPPER_TYPE(PlayerGuid, PLAYER)
    ECLIPSE_REGISTER_WRAPPER_TYPE(CreatureEntry, CREATURE)
    ECLIPSE_REGISTER_WRAPPER_TYPE(GameObjectEntry, GAMEOBJECT)
    ECLIPSE_REGISTER_WRAPPER_TYPE(ItemEntry, ITEM)
    ECLIPSE_REGISTER_WRAPPER_TYPE(MapId, MAP)

    template<>
    struct event_type_trait<ObjectGuid>
    {
        static constexpr EventType type = EventType::PLAYER;
    };

    // ========== RUNTIME TYPE DETECTION ==========
    inline constexpr EventType GetObjectGuidEventType(const ObjectGuid& guid) noexcept
    {
        if (guid.IsPlayer()) return EventType::PLAYER;
        if (guid.IsAnyTypeCreature()) return EventType::CREATURE;
        if (guid.IsAnyTypeGameObject()) return EventType::GAMEOBJECT;
        if (guid.IsItem()) return EventType::ITEM;
        return EventType::PLAYER;
    }

    // ========== UTILITY FUNCTIONS ==========

    template<typename T>
    constexpr EventType get_event_type() noexcept
    {
        using DecayedType = std::decay_t<T>;
        return event_type_trait<DecayedType>::type;
    }

    inline constexpr EventType get_event_type(const ObjectGuid& guid) noexcept
    {
        return GetObjectGuidEventType(guid);
    }

    template<typename T>
    constexpr bool has_entry() noexcept
    {
        using DecayedType = std::decay_t<T>;
        return has_entry_trait<DecayedType>::value;
    }

    template<typename T>
    constexpr bool can_get_map() noexcept
    {
        using DecayedType = std::decay_t<T>;
        return can_get_map_trait<DecayedType>::value;
    }

    // Safe entry getter with concept checking
    template<HasGetEntry T>
    constexpr uint32 get_entry(T* obj) noexcept
    {
        return obj ? obj->GetEntry() : 0;
    }

    // Safe map getter with concept checking
    template<HasGetMap T>
    constexpr Map* get_map(T* obj) noexcept
    {
        return obj ? obj->GetMap() : nullptr;
    }

    // Special case for Item -> Player -> Map
    inline Map* get_map(Item* item) noexcept
    {
        if (!item) return nullptr;
        if (auto* owner = item->GetOwner()) {
            return owner->GetMap();
        }
        return nullptr;
    }

    // ========== TYPE VALIDATION AT COMPILE TIME ==========

    template<typename T>
    concept ValidEventType = requires {
        event_type_trait<std::decay_t<T>>::type;
    };

    template<typename T>
    concept ValidKeyedType = ValidEventType<T> && has_entry<T>();

    // Clean up macros to avoid pollution
    #undef ECLIPSE_REGISTER_TYPE
    #undef ECLIPSE_REGISTER_KEYED_TYPE
    #undef ECLIPSE_REGISTER_WRAPPER_TYPE
}

#endif // ECLIPSE_EVENT_TRAITS_HPP