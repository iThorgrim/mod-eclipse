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
        PLAYERHOOK_ON_AFTER_MOVE_ITEM_FROM_INVENTORY,
        PLAYERHOOK_ON_EQUIP,
        PLAYERHOOK_ON_PLAYER_JOIN_BG,
        PLAYERHOOK_ON_PLAYER_JOIN_ARENA,
        PLAYERHOOK_GET_CUSTOM_GET_ARENA_TEAM_ID,
        PLAYERHOOK_GET_CUSTOM_ARENA_PERSONAL_RATING,
        PLAYERHOOK_ON_GET_MAX_PERSONAL_ARENA_RATING_REQUIREMENT,
        PLAYERHOOK_ON_LOOT_ITEM,
        PLAYERHOOK_ON_BEFORE_FILL_QUEST_LOOT_ITEM,
        PLAYERHOOK_ON_STORE_NEW_ITEM,
        PLAYERHOOK_ON_CREATE_ITEM,
        PLAYERHOOK_ON_QUEST_REWARD_ITEM,
        PLAYERHOOK_CAN_PLACE_AUCTION_BID,
        PLAYERHOOK_ON_GROUP_ROLL_REWARD_ITEM,
        PLAYERHOOK_ON_BEFORE_OPEN_ITEM,
        PLAYERHOOK_ON_BEFORE_QUEST_COMPLETE,
        PLAYERHOOK_ON_QUEST_COMPUTE_EXP,
        PLAYERHOOK_ON_BEFORE_DURABILITY_REPAIR,
        PLAYERHOOK_ON_BEFORE_BUY_ITEM_FROM_VENDOR,
        PLAYERHOOK_ON_BEFORE_STORE_OR_EQUIP_NEW_ITEM,
        PLAYERHOOK_ON_AFTER_STORE_OR_EQUIP_NEW_ITEM,
        PLAYERHOOK_ON_AFTER_UPDATE_MAX_POWER,
        PLAYERHOOK_ON_AFTER_UPDATE_MAX_HEALTH,
        PLAYERHOOK_ON_BEFORE_UPDATE_ATTACK_POWER_AND_DAMAGE,
        PLAYERHOOK_ON_AFTER_UPDATE_ATTACK_POWER_AND_DAMAGE,
        PLAYERHOOK_ON_BEFORE_INIT_TALENT_FOR_LEVEL,
        PLAYERHOOK_ON_FIRST_LOGIN,
        PLAYERHOOK_ON_SET_MAX_LEVEL,
        PLAYERHOOK_CAN_JOIN_IN_BATTLEGROUND_QUEUE,
        PLAYERHOOK_SHOULD_BE_REWARDED_WITH_MONEY_INSTEAD_OF_EXP,
        PLAYERHOOK_ON_BEFORE_TEMP_SUMMON_INIT_STATS,
        PLAYERHOOK_ON_BEFORE_GUARDIAN_INIT_STATS_FOR_LEVEL,
        PLAYERHOOK_ON_AFTER_GUARDIAN_INIT_STATS_FOR_LEVEL,
        PLAYERHOOK_ON_BEFORE_LOAD_PET_FROM_DB,
        PLAYERHOOK_CAN_JOIN_IN_ARENA_QUEUE,
        PLAYERHOOK_CAN_BATTLEFIELD_PORT,
        PLAYERHOOK_CAN_GROUP_INVITE,
        PLAYERHOOK_CAN_GROUP_ACCEPT,
        PLAYERHOOK_CAN_SELL_ITEM,
        PLAYERHOOK_CAN_SEND_MAIL,
        PLAYERHOOK_PETITION_BUY,
        PLAYERHOOK_PETITION_SHOW_LIST,
        PLAYERHOOK_ON_REWARD_KILL_REWARDER,
        PLAYERHOOK_CAN_GIVE_MAIL_REWARD_AT_GIVE_LEVEL,
        PLAYERHOOK_ON_DELETE_FROM_DB,
        PLAYERHOOK_CAN_REPOP_AT_GRAVEYARD,
        PLAYERHOOK_ON_PLAYER_IS_CLASS,
        PLAYERHOOK_ON_GET_MAX_SKILL_VALUE,
        PLAYERHOOK_ON_PLAYER_HAS_ACTIVE_POWER_TYPE,
        PLAYERHOOK_ON_UPDATE_GATHERING_SKILL,
        PLAYERHOOK_ON_UPDATE_CRAFTING_SKILL,
        PLAYERHOOK_ON_UPDATE_FISHING_SKILL,
        PLAYERHOOK_CAN_AREA_EXPLORE_AND_OUTDOOR,
        PLAYERHOOK_ON_VICTIM_REWARD_BEFORE,
        PLAYERHOOK_ON_VICTIM_REWARD_AFTER,
        PLAYERHOOK_ON_CUSTOM_SCALING_STAT_VALUE_BEFORE,
        PLAYERHOOK_ON_CUSTOM_SCALING_STAT_VALUE,
        PLAYERHOOK_ON_APPLY_ITEM_MODS_BEFORE,
        PLAYERHOOK_ON_APPLY_ENCHANTMENT_ITEM_MODS_BEFORE,
        PLAYERHOOK_ON_APPLY_WEAPON_DAMAGE,
        PLAYERHOOK_CAN_ARMOR_DAMAGE_MODIFIER,
        PLAYERHOOK_ON_GET_FERAL_AP_BONUS,
        PLAYERHOOK_CAN_APPLY_WEAPON_DEPENDENT_AURA_DAMAGE_MOD,
        PLAYERHOOK_CAN_APPLY_EQUIP_SPELL,
        PLAYERHOOK_CAN_APPLY_EQUIP_SPELLS_ITEM_SET,
        PLAYERHOOK_CAN_CAST_ITEM_COMBAT_SPELL,
        PLAYERHOOK_CAN_CAST_ITEM_USE_SPELL,
        PLAYERHOOK_ON_APPLY_AMMO_BONUSES,
        PLAYERHOOK_CAN_EQUIP_ITEM,
        PLAYERHOOK_CAN_UNEQUIP_ITEM,
        PLAYERHOOK_CAN_USE_ITEM,
        PLAYERHOOK_CAN_SAVE_EQUIP_NEW_ITEM,
        PLAYERHOOK_CAN_APPLY_ENCHANTMENT,
        PLAYERHOOK_PASSED_QUEST_KILLED_MONSTER_CREDIT,
        PLAYERHOOK_CHECK_ITEM_IN_SLOT_AT_LOAD_INVENTORY,
        PLAYERHOOK_NOT_AVOID_SATISFY,
        PLAYERHOOK_NOT_VISIBLE_GLOBALLY_FOR,
        PLAYERHOOK_ON_GET_ARENA_PERSONAL_RATING,
        PLAYERHOOK_ON_GET_ARENA_TEAM_ID,
        PLAYERHOOK_ON_IS_FFA_PVP,
        PLAYERHOOK_ON_FFA_PVP_STATE_UPDATE,
        PLAYERHOOK_ON_IS_PVP,
        PLAYERHOOK_ON_GET_MAX_SKILL_VALUE_FOR_LEVEL,
        PLAYERHOOK_NOT_SET_ARENA_TEAM_INFO_FIELD,
        PLAYERHOOK_CAN_JOIN_LFG,
        PLAYERHOOK_CAN_ENTER_MAP,
        PLAYERHOOK_CAN_INIT_TRADE,
        PLAYERHOOK_CAN_SET_TRADE_ITEM,
        PLAYERHOOK_ON_SET_SERVER_SIDE_VISIBILITY,
        PLAYERHOOK_ON_SET_SERVER_SIDE_VISIBILITY_DETECT,
        PLAYERHOOK_ON_PLAYER_RESURRECT,
        PLAYERHOOK_ON_BEFORE_CHOOSE_GRAVEYARD,
        PLAYERHOOK_CAN_PLAYER_USE_CHAT,
        PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT,
        PLAYERHOOK_CAN_PLAYER_USE_GROUP_CHAT,
        PLAYERHOOK_CAN_PLAYER_USE_GUILD_CHAT,
        PLAYERHOOK_CAN_PLAYER_USE_CHANNEL_CHAT,
        PLAYERHOOK_ON_PLAYER_LEARN_TALENTS,
        PLAYERHOOK_ON_PLAYER_ENTER_COMBAT,
        PLAYERHOOK_ON_PLAYER_LEAVE_COMBAT,
        PLAYERHOOK_ON_QUEST_ABANDON,
        PLAYERHOOK_ON_GET_QUEST_RATE,
        PLAYERHOOK_ON_CAN_PLAYER_FLY_IN_ZONE,
        PLAYERHOOK_ANTICHEAT_SET_CAN_FLY_BY_SERVER,
        PLAYERHOOK_ANTICHEAT_SET_UNDER_ACK_MOUNT,
        PLAYERHOOK_ANTICHEAT_SET_ROOT_ACK_UPD,
        PLAYERHOOK_ANTICHEAT_SET_JUMPING_BY_OPCODE,
        PLAYERHOOK_ANTICHEAT_UPDATE_MOVEMENT_INFO,
        PLAYERHOOK_ANTICHEAT_HANDLE_DOUBLE_JUMP,
        PLAYERHOOK_ANTICHEAT_CHECK_MOVEMENT_INFO,
        PLAYERHOOK_CAN_SEND_ERROR_ALREADY_LOOTED,
        PLAYERHOOK_ON_AFTER_CREATURE_LOOT,
        PLAYERHOOK_ON_AFTER_CREATURE_LOOT_MONEY,
        PLAYERHOOK_ON_CAN_UPDATE_SKILL,
        PLAYERHOOK_ON_BEFORE_UPDATE_SKILL,
        PLAYERHOOK_ON_UPDATE_SKILL,
        PLAYERHOOK_CAN_RESURRECT,
        PLAYERHOOK_ON_CAN_GIVE_LEVEL,
        PLAYERHOOK_ON_SEND_LIST_INVENTORY,
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

    struct PlayerGuid
    {
        uint32 guid;
        PlayerGuid(uint32 g) : guid(g) {}
    };

    struct CreatureEntry
    {
        uint32 entry;
        CreatureEntry(uint32 e) : entry(e) {}
    };

    struct GameObjectEntry
    {
        uint32 entry;
        GameObjectEntry(uint32 e) : entry(e) {}
    };

    struct ItemEntry
    {
        uint32 entry;
        ItemEntry(uint32 e) : entry(e) {}
    };

    struct MapId
    {
        uint32 mapId;
        MapId(uint32 m) : mapId(m) {}
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

    template<>
    struct event_type_trait<PlayerGuid>
    {
        static constexpr EventType type = EventType::PLAYER;
    };

    template<>
    struct event_type_trait<CreatureEntry>
    {
        static constexpr EventType type = EventType::CREATURE;
    };

    template<>
    struct event_type_trait<GameObjectEntry>
    {
        static constexpr EventType type = EventType::GAMEOBJECT;
    };

    template<>
    struct event_type_trait<ItemEntry>
    {
        static constexpr EventType type = EventType::ITEM;
    };

    template<>
    struct event_type_trait<MapId>
    {
        static constexpr EventType type = EventType::MAP;
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

    inline EventType GetObjectGuidEventType(const ObjectGuid& guid)
    {
        if (guid.IsPlayer())
            return EventType::PLAYER;
        else if (guid.IsAnyTypeCreature())
            return EventType::CREATURE;
        else if (guid.IsAnyTypeGameObject())
            return EventType::GAMEOBJECT;
        else if (guid.IsItem())
            return EventType::ITEM;
        else
            return EventType::PLAYER; // Default fallback
    }

    template<typename T>
    constexpr EventType get_event_type()
    {
        using DecayedType = std::decay_t<T>;
        return event_type_trait<DecayedType>::type;
    }

    inline EventType get_event_type(const ObjectGuid& guid)
    {
        return GetObjectGuidEventType(guid);
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