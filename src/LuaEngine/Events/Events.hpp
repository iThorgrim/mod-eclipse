#ifndef ECLIPSE_EVENTS_HPP
#define ECLIPSE_EVENTS_HPP

// ========== ECLIPSE EVENT SYSTEM ==========
// Modern, high-performance event system for mod-eclipse
// 
// Features:
// - Type-safe event routing with compile-time validation
// - Zero-overhead abstractions using modern C++20 concepts
// - Automatic type detection and registration
// - Clean separation of concerns across multiple headers
// - Optimized for AzerothCore's threading model

#include "EventTypes.hpp"
#include "EventTraits.hpp"

namespace Eclipse
{
    // Template helpers for easier event system usage
    template<typename T>
    concept SupportedEventObject = ValidEventType<T>;

    template<typename T>
    concept KeyedEventObject = ValidKeyedType<T>;

    // Quick type checks for runtime decisions
    template<typename T>
    constexpr bool is_player_event() noexcept { return get_event_type<T>() == EventType::PLAYER; }

    template<typename T>
    constexpr bool is_map_event() noexcept { return get_event_type<T>() == EventType::MAP; }

    template<typename T>
    constexpr bool is_creature_event() noexcept { return get_event_type<T>() == EventType::CREATURE; }

    template<typename T>
    constexpr bool is_gameobject_event() noexcept { return get_event_type<T>() == EventType::GAMEOBJECT; }

    template<typename T>
    constexpr bool is_item_event() noexcept { return get_event_type<T>() == EventType::ITEM; }

    // Utility for script authors
    template<typename T>
    constexpr const char* get_event_type_name() noexcept
    {
        constexpr auto type = get_event_type<T>();
        switch (type) {
            case EventType::PLAYER: return "Player";
            case EventType::MAP: return "Map";
            case EventType::CREATURE: return "Creature";
            case EventType::GAMEOBJECT: return "GameObject";
            case EventType::ITEM: return "Item";
            default: return "Unknown";
        }
    }
}

#endif // ECLIPSE_EVENTS_HPP