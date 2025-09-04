#ifndef ECLIPSE_EVENTS_HPP
#define ECLIPSE_EVENTS_HPP

#include "EclipseIncludes.hpp"
#include <type_traits>

namespace Eclipse
{
    enum PlayerEvents : uint32
    {
        PLAYERHOOK_ON_CREATE = 1,
        PLAYERHOOK_ON_DELETE,
        PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_LOGOUT,
        PLAYERHOOK_ON_SPELL_CAST,
        PLAYERHOOK_ON_PVP_KILL,
        PLAYERHOOK_ON_CREATURE_KILL,
        PLAYERHOOK_ON_PLAYER_KILLED_BY_CREATURE,
        PLAYERHOOK_ON_DUEL_REQUEST,
        PLAYERHOOK_ON_DUEL_START,
        PLAYERHOOK_ON_DUEL_END,
        PLAYERHOOK_ON_GIVE_EXP,
        PLAYERHOOK_ON_LEVEL_CHANGED,
        PLAYERHOOK_ON_MONEY_CHANGED,
        PLAYERHOOK_ON_REPUTATION_CHANGE,
        PLAYERHOOK_ON_FREE_TALENT_POINTS_CHANGED,
        PLAYERHOOK_ON_TALENTS_RESET,
        PLAYERHOOK_ON_CHAT,
        PLAYERHOOK_ON_CHAT_WITH_RECEIVER,
        PLAYERHOOK_ON_CHAT_WITH_GROUP,
        PLAYERHOOK_ON_CHAT_WITH_GUILD,
        PLAYERHOOK_ON_CHAT_WITH_CHANNEL,
        PLAYERHOOK_ON_EMOTE,
        PLAYERHOOK_ON_TEXT_EMOTE,
        PLAYERHOOK_ON_SAVE,
        PLAYERHOOK_ON_BIND_TO_INSTANCE,
        PLAYERHOOK_ON_UPDATE_AREA,
        PLAYERHOOK_ON_UPDATE_ZONE,
        PLAYERHOOK_ON_MAP_CHANGED,
        PLAYERHOOK_ON_LEARN_SPELL,
        PLAYERHOOK_ON_ACHI_COMPLETE,
        PLAYERHOOK_ON_PLAYER_COMPLETE_QUEST,
        PLAYER_EVENT_ON_BG_DESERTION,
        PLAYERHOOK_ON_CREATURE_KILLED_BY_PET,

        // Custom
        PLAYERHOOK_ON_PLAYER_JUST_DIED = 100,
        PLAYERHOOK_ON_CALCULATE_TALENTS_POINTS = 101,
        PLAYERHOOK_ON_PLAYER_RELEASED_GHOST = 102,
        PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP = 103,
        PLAYERHOOK_ON_PLAYER_PVP_FLAG_CHANGE = 104,
        PLAYERHOOK_ON_AFTER_SPEC_SLOT_CHANGED = 105,
        PLAYERHOOK_ON_BEFORE_UPDATE = 106,
        PLAYERHOOK_ON_UPDATE = 107,
        PLAYERHOOK_ON_BEFORE_LOOT_MONEY = 108,
        PLAYERHOOK_ON_REPUTATION_RANK_CHANGE = 109,
        PLAYERHOOK_ON_FORGOT_SPELL = 110,
        PLAYERHOOK_ON_BEFORE_SEND_CHAT_MESSAGE = 112,
        PLAYERHOOK_ON_LOAD_FROM_DB = 113,
        PLAYERHOOK_ON_BEFORE_LOGOUT = 114,
        PLAYERHOOK_ON_FAILED_DELETE = 115,
        PLAYERHOOK_ON_BEFORE_TELEPORT = 116,
        PLAYERHOOK_ON_UPDATE_FACTION = 117,
        PLAYERHOOK_ON_ADD_TO_BATTLEGROUND = 118,
        PLAYERHOOK_ON_QUEUE_RANDOM_DUNGEON = 119,
        PLAYERHOOK_ON_REMOVE_FROM_BATTLEGROUND = 120,
        PLAYERHOOK_ON_BEFORE_ACHI_COMPLETE = 121,
        PLAYERHOOK_ON_CRITERIA_PROGRESS = 122,
        PLAYERHOOK_ON_BEFORE_CRITERIA_PROGRESS = 123,
        PLAYERHOOK_ON_ACHI_SAVE = 124,
        PLAYERHOOK_ON_CRITERIA_SAVE = 125,
        PLAYERHOOK_ON_GOSSIP_SELECT = 126,
        PLAYERHOOK_ON_GOSSIP_SELECT_CODE = 127,
        PLAYERHOOK_ON_BEING_CHARMED = 128,
        PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT = 129,
        PLAYERHOOK_ON_GIVE_REPUTATION,
        PLAYERHOOK_ON_BATTLEGROUND_DESERTION,
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
    struct event_type_trait<const Player*>
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

    template<>
    struct event_type_trait<ObjectGuid>
    {
        static constexpr EventType type = EventType::PLAYER;
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