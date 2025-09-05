#ifndef ECLIPSE_EVENT_TYPES_HPP
#define ECLIPSE_EVENT_TYPES_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    // Primary event categories
    enum class EventType : uint8
    {
        PLAYER = 0,
        MAP = 1,
        CREATURE = 2,
        GAMEOBJECT = 3,
        ITEM = 4,
        COUNT // For validation and iteration
    };

    // ========== X-MACRO DEFINITIONS ==========
    #define DEFINE_PLAYER_EVENTS(X) \
        X(  PLAYER_EVENT_ON_CHARACTER_CREATE,        1    ) \
        X(  PLAYER_EVENT_ON_CHARACTER_DELETE,        2    ) \
        X(  PLAYER_EVENT_ON_LOGIN,                   3    ) \
        X(  PLAYER_EVENT_ON_LOGOUT,                  4    ) \
        X(  PLAYER_EVENT_ON_SPELL_CAST,              5    ) \
        X(  PLAYER_EVENT_ON_KILL_PLAYER,             6    ) \
        X(  PLAYER_EVENT_ON_KILL_CREATURE,           7    ) \
        X(  PLAYER_EVENT_ON_KILLED_BY_CREATURE,      8    ) \
        X(  PLAYER_EVENT_ON_DUEL_REQUEST,            9    ) \
        X(  PLAYER_EVENT_ON_DUEL_START,              10   ) \
        X(  PLAYER_EVENT_ON_DUEL_END,                11   ) \
        X(  PLAYER_EVENT_ON_GIVE_XP,                 12   ) \
        X(  PLAYER_EVENT_ON_LEVEL_CHANGE,            13   ) \
        X(  PLAYER_EVENT_ON_MONEY_CHANGE,            14   ) \
        X(  PLAYER_EVENT_ON_REPUTATION_CHANGE,       15   ) \
        X(  PLAYER_EVENT_ON_TALENTS_CHANGE,          16   ) \
        X(  PLAYER_EVENT_ON_TALENTS_RESET,           17   ) \
        X(  PLAYER_EVENT_ON_CHAT,                    18   ) \
        X(  PLAYER_EVENT_ON_WHISPER,                 19   ) \
        X(  PLAYER_EVENT_ON_GROUP_CHAT,              20   ) \
        X(  PLAYER_EVENT_ON_GUILD_CHAT,              21   ) \
        X(  PLAYER_EVENT_ON_CHANNEL_CHAT,            22   ) \
        X(  PLAYER_EVENT_ON_EMOTE,                   23   ) \
        X(  PLAYER_EVENT_ON_TEXT_EMOTE,              24   ) \
        X(  PLAYER_EVENT_ON_SAVE,                    25   ) \
        X(  PLAYER_EVENT_ON_BIND_TO_INSTANCE,        26   ) \
        X(  PLAYER_EVENT_ON_UPDATE_ZONE,             27   ) \
        X(  PLAYER_EVENT_ON_MAP_CHANGE,              28   ) \
        X(  PLAYER_EVENT_ON_EQUIP,                   29   ) \
        X(  PLAYER_EVENT_ON_FIRST_LOGIN,             30   ) \
        X(  PLAYER_EVENT_ON_CAN_USE_ITEM,            31   ) \
        X(  PLAYER_EVENT_ON_LOOT_ITEM,               32   ) \
        X(  PLAYER_EVENT_ON_CAN_REPOP_AT_GRAVEYARD,  35   ) \
        X(  PLAYER_EVENT_ON_LEARN_SPELL,             44   ) \
        X(  PLAYER_EVENT_ON_ACHI_COMPLETE,           45   ) \
        X(  PLAYER_EVENT_ON_UPDATE_AREA,             47   ) \
        X(  PLAYER_EVENT_ON_CAN_INIT_TRADE,          48  ) \
        X(  PLAYER_EVENT_ON_CAN_SEND_MAIL,           49  ) \
        X(  PLAYER_EVENT_ON_CAN_JOIN_LFG,            50  ) \
        X(  PLAYER_EVENT_ON_QUEST_REWARD_ITEM,       51  ) \
        X(  PLAYER_EVENT_ON_CREATE_ITEM,             52  ) \
        X(  PLAYER_EVENT_ON_STORE_NEW_ITEM,          53  ) \
        X(  PLAYER_EVENT_ON_COMPLETE_QUEST,          54   ) \
        X(  PLAYER_EVENT_ON_CAN_GROUP_INVITE,        55   ) \
        X(  PLAYER_EVENT_ON_GROUP_ROLL_REWARD_ITEM,  56   ) \
        X(  PLAYER_EVENT_ON_BG_DESERTION,            57   ) \
        X(  PLAYER_EVENT_ON_PET_KILL,                58   ) \
        X(  PLAYER_EVENT_ON_JUST_DIED,               64  ) \
        X(  PLAYER_EVENT_ON_CALCULATE_TALENT_POINTS, 65  ) \
        X(  PLAYER_EVENT_ON_RELEASED_GHOST,          66  ) \
        X(  PLAYER_EVENT_ON_SEND_INITIAL_PACKET_BEFORE_ADD_TO_MAP,          67  ) \
        X(  PLAYER_EVENT_ON_PVP_FLAG_CHANGE,          68  ) \
        X(  PLAYER_EVENT_ON_AFTER_SPEC_SLOT_CHANGED,          69  ) \
        X(  PLAYER_EVENT_ON_BEFORE_UPDATE,          70  ) \
        X(  PLAYER_EVENT_ON_UPDATE,          71  ) \
        X(  PLAYER_EVENT_ON_BEFORE_LOOT_MONEY,          72  ) \
        X(  PLAYER_EVENT_ON_REPUTATION_RANK_CHANGE,          73  ) \
        X(  PLAYER_EVENT_ON_GIVE_REPUTATION,          74  ) \
        X(  PLAYER_EVENT_ON_FORGOT_SPELL,             75  ) \
        X(  PLAYER_EVENT_ON_BEFORE_SEND_CHAT_MESSAGE, 76  ) \
        X(  PLAYER_EVENT_ON_LOAD_FROM_DB, 77  ) \
        X(  PLAYER_EVENT_ON_BEFORE_LOGOUT, 78  ) \
        X(  PLAYER_EVENT_ON_FAILED_DELETE, 79  ) \
        X(  PLAYER_EVENT_ON_BEFORE_TELEPORT, 80  ) \
        X(  PLAYER_EVENT_ON_UPDATE_FACTION, 81  ) \
        X(  PLAYER_EVENT_ON_ADD_TO_BATTLEGROUND, 82  ) \
        X(  PLAYER_EVENT_ON_QUEUE_RANDOM_DUNGEON, 83  ) \
        X(  PLAYER_EVENT_ON_REMOVE_FROM_BATTLEGROUND, 84  ) \
        X(  PLAYER_EVENT_ON_BEFORE_ACHI_COMPLETE, 85  ) \
        X(  PLAYER_EVENT_ON_CRITERIA_PROGRESS, 86  ) \
        X(  PLAYER_EVENT_ON_BEFORE_CRITERIA_PROGRESS, 87  ) \
        X(  PLAYER_EVENT_ON_ACHI_SAVE, 88  ) \
        X(  PLAYER_EVENT_ON_CRITERIA_SAVE, 89  ) \
        X(  PLAYER_EVENT_ON_BEING_CHARMED, 90  ) \
        X(  PLAYER_EVENT_ON_AFTER_SET_VISIBLE_ITEM_SLOT, 91  ) \
        X(  PLAYER_EVENT_ON_AFTER_MOVE_ITEM_FROM_INVENTORY, 92  ) \
        X(  PLAYER_EVENT_ON_PLAYER_JOIN_BG, 93  ) \
        X(  PLAYER_EVENT_ON_PLAYER_JOIN_ARENA, 94  ) \
        X(  PLAYER_EVENT_ON_GET_MAX_PERSONAL_ARENA_RATING_REQUIREMENT, 95  ) \
        X(  PLAYER_EVENT_ON_BEFORE_FILL_QUEST_LOOT_ITEM, 96  ) \
        X(  PLAYER_EVENT_ON_CAN_PLACE_AUCTION_BID, 97  ) \
        X(  PLAYER_EVENT_ON_BEFORE_OPEN_ITEM, 98  ) \
        X(  PLAYER_EVENT_ON_BEFORE_QUEST_COMPLETE, 99  ) \
        X(  PLAYER_EVENT_ON_QUEST_COMPUTE_EXP, 100  ) \
        X(  PLAYER_EVENT_ON_BEFORE_DURABILITY_REPAIR, 101  ) \
        X(  PLAYER_EVENT_ON_BEFORE_BUY_ITEM_FROM_VENDOR, 102  ) \
        X(  PLAYER_EVENT_ON_BEFORE_STORE_OR_EQUIP_NEW_ITEM, 103  ) \
        X(  PLAYER_EVENT_ON_AFTER_STORE_OR_EQUIP_NEW_ITEM, 104  ) \
        X(  PLAYER_EVENT_ON_AFTER_UPDATE_MAX_POWER, 105  ) \
        X(  PLAYER_EVENT_ON_AFTER_UPDATE_MAX_HEALTH, 106  ) \
        X(  PLAYER_EVENT_ON_BEFORE_UPDATE_ATTACK_POWER_AND_DAMAGE, 107  ) \
        X(  PLAYER_EVENT_ON_AFTER_UPDATE_ATTACK_POWER_AND_DAMAGE, 108  ) \
        X(  PLAYER_EVENT_ON_BEFORE_INIT_TALENT_FOR_LEVEL, 109  ) \
        X(  PLAYER_EVENT_ON_SET_MAX_LEVEL, 110  ) \
        X(  PLAYER_EVENT_ON_CAN_JOIN_IN_BATTLEGROUND_QUEUE, 111  ) \
        X(  PLAYER_EVENT_SHOULD_BE_REWARDED_WITH_MONEY_INSTEAD_OF_EXP, 112  ) \
        X(  PLAYER_EVENT_ON_BEFORE_TEMP_SUMMON_INIT_STATS, 113  ) \
        X(  PLAYER_EVENT_ON_BEFORE_GUARDIAN_INIT_STATS_FOR_LEVEL, 114  ) \
        X(  PLAYER_EVENT_ON_AFTER_GUARDIAN_INIT_STATS_FOR_LEVEL, 115  ) \
        X(  PLAYER_EVENT_ON_BEFORE_LOAD_PET_FROM_DB, 116  ) \
        X(  PLAYER_EVENT_ON_CAN_JOIN_IN_ARENA_QUEUE, 117  ) \
        X(  PLAYER_EVENT_ON_CAN_BATTLEFIELD_PORT, 118  ) \
        X(  PLAYER_EVENT_ON_CAN_GROUP_ACCEPT, 119  ) \
        X(  PLAYER_EVENT_ON_CAN_SELL_ITEM, 120  ) \
        X(  PLAYER_EVENT_PETITION_BUY, 121  ) \
        X(  PLAYER_EVENT_PETITION_SHOW_LIST, 122  ) \
        X(  PLAYER_EVENT_ON_REWARD_KILL_REWARDER, 123  ) \
        X(  PLAYER_EVENT_ON_CAN_GIVE_MAIL_REWARD_AT_GIVE_LEVEL, 124  ) \
        X(  PLAYER_EVENT_ON_DELETE_FROM_DB, 125  ) \
        X(  PLAYER_EVENT_ON_PLAYER_IS_CLASS, 126  ) \
        X(  PLAYER_EVENT_ON_GET_MAX_SKILL_VALUE, 127  ) \
        X(  PLAYER_EVENT_ON_PLAYER_HAS_ACTIVE_POWER_TYPE, 128  ) \
        X(  PLAYER_EVENT_ON_UPDATE_GATHERING_SKILL, 129  ) \
        X(  PLAYER_EVENT_ON_UPDATE_CRAFTING_SKILL, 130  ) \
        X(  PLAYER_EVENT_ON_UPDATE_FISHING_SKILL, 131  ) \
        X(  PLAYER_EVENT_ON_CAN_AREA_EXPLORE_AND_OUTDOOR, 132  ) \
        X(  PLAYER_EVENT_ON_VICTIM_REWARD_BEFORE, 133  ) \
        X(  PLAYER_EVENT_ON_VICTIM_REWARD_AFTER, 134  ) \
        X(  PLAYER_EVENT_ON_CUSTOM_SCALING_STAT_VALUE_BEFORE, 135  ) \
        X(  PLAYER_EVENT_ON_CUSTOM_SCALING_STAT_VALUE, 136  ) \
        X(  PLAYER_EVENT_ON_APPLY_ITEM_MODS_BEFORE, 137  ) \
        X(  PLAYER_EVENT_ON_APPLY_ENCHANTMENT_ITEM_MODS_BEFORE, 138  ) \
        X(  PLAYER_EVENT_ON_APPLY_WEAPON_DAMAGE, 139  ) \
        X(  PLAYER_EVENT_ON_CAN_ARMOR_DAMAGE_MODIFIER, 140  ) \
        X(  PLAYER_EVENT_ON_GET_FERAL_AP_BONUS, 141  ) \
        X(  PLAYER_EVENT_ON_CAN_APPLY_WEAPON_DEPENDENT_AURA_DAMAGE_MOD, 142  ) \
        X(  PLAYER_EVENT_ON_CAN_APPLY_EQUIP_SPELL, 143  ) \
        X(  PLAYER_EVENT_ON_CAN_APPLY_EQUIP_SPELLS_ITEM_SET, 144  ) \
        X(  PLAYER_EVENT_ON_CAN_CAST_ITEM_COMBAT_SPELL, 145  ) \
        X(  PLAYER_EVENT_ON_CAN_CAST_ITEM_USE_SPELL, 146  ) \
        X(  PLAYER_EVENT_ON_APPLY_AMMO_BONUSES, 147  ) \
        X(  PLAYER_EVENT_ON_CAN_EQUIP_ITEM, 148  ) \
        X(  PLAYER_EVENT_ON_CAN_UNEQUIP_ITEM, 149  ) \
        X(  PLAYER_EVENT_ON_CAN_SAVE_EQUIP_NEW_ITEM, 150  ) \
        X(  PLAYER_EVENT_ON_CAN_APPLY_ENCHANTMENT, 151  ) \
        X(  PLAYER_EVENT_ON_GET_QUEST_RATE, 152  ) \
        X(  PLAYER_EVENT_PASSED_QUEST_KILLED_MONSTER_CREDIT, 153  ) \
        X(  PLAYER_EVENT_CHECK_ITEM_IN_SLOT_AT_LOAD_INVENTORY, 154  ) \
        X(  PLAYER_EVENT_NOT_AVOID_SATISFY, 155  ) \
        X(  PLAYER_EVENT_NOT_VISIBLE_GLOBALLY_FOR, 156  ) \
        X(  PLAYER_EVENT_ON_GET_ARENA_PERSONAL_RATING, 157  ) \
        X(  PLAYER_EVENT_ON_GET_ARENA_TEAM_ID, 158  ) \
        X(  PLAYER_EVENT_ON_IS_FFA_PVP, 159  ) \
        X(  PLAYER_EVENT_ON_FFA_PVP_STATE_UPDATE, 160  ) \
        X(  PLAYER_EVENT_ON_IS_PVP, 161  ) \
        X(  PLAYER_EVENT_ON_GET_MAX_SKILL_VALUE_FOR_LEVEL, 162  ) \
        X(  PLAYER_EVENT_NOT_SET_ARENA_TEAM_INFO_FIELD, 163  ) \
        X(  PLAYER_EVENT_ON_CAN_SET_TRADE_ITEM, 164  ) \
        X(  PLAYER_EVENT_ON_SET_SERVER_SIDE_VISIBILITY, 165  ) \
        X(  PLAYER_EVENT_ON_CAN_ENTER_MAP, 166  ) \


// X(  PLAYER_EVENT_ON_ENTER_COMBAT,            33   )
// X(  PLAYER_EVENT_ON_LEAVE_COMBAT,            34   )
// X(  PLAYER_EVENT_ON_RESURRECT,            36   )
// X(  PLAYER_EVENT_ON_LOOT_MONEY,            37   )
// X(  PLAYER_EVENT_ON_QUEST_ABANDON,            38   )
// X(  PLAYER_EVENT_ON_LEARN_TALENTS,            39   )
// X(  UNUSED,            40   )
// X(  UNUSED,            41   )
// X(  PLAYER_EVENT_ON_COMMAND,            42   )
// X(  PLAYER_EVENT_ON_PET_ADDED_TO_WORLD,            43   )
// X(  PLAYER_EVENT_ON_FFAPVP_CHANGE,   46   )
// X(  PLAYER_EVENT_ON_CAN_RESURRECT,   59   )
// X(  PLAYER_EVENT_ON_CAN_UPDATE_SKILL,   60   )
// X(  PLAYER_EVENT_ON_BEFORE_UPDATE_SKILL,   61   )
// X(  PLAYER_EVENT_ON_UPDATE_SKILL,   62   )
// X(  PLAYER_EVENT_ON_QUEST_ACCEPT,   63   )

    #define DEFINE_MAP_EVENTS(X) \
        X(MAP_EVENT_ON_UPDATE,           1) \
        X(MAP_ON_PLAYER_ENTER,           2) \
        X(MAP_ON_PLAYER_LEAVE,           3) \
        X(MAP_ON_CREATURE_CREATE,        4) \
        X(MAP_ON_GAMEOBJECT_CREATE,      5)

    #define DEFINE_CREATURE_EVENTS(X) \
        X(CREATURE_ON_SPAWN,             1) \
        X(CREATURE_ON_DESPAWN,           2) \
        X(CREATURE_ON_JUST_DIED,         3) \
        X(CREATURE_ON_KILLED_UNIT,       4) \
        X(CREATURE_ON_ENTER_COMBAT,      5) \
        X(CREATURE_ON_LEAVE_COMBAT,      6) \
        X(CREATURE_ON_REACH_WP,          7) \
        X(CREATURE_ON_AIUPDATE,          8) \
        X(CREATURE_ON_RECEIVE_EMOTE,     9) \
        X(CREATURE_ON_DAMAGE_TAKEN,      10) \
        X(CREATURE_ON_PRE_COMBAT,        11)

    #define DEFINE_GAMEOBJECT_EVENTS(X) \
        X(GAMEOBJECT_ON_USE,                  1) \
        X(GAMEOBJECT_ON_SPAWN,                2) \
        X(GAMEOBJECT_ON_DESPAWN,              3) \
        X(GAMEOBJECT_ON_LOOT_STATE_CHANGED,   4) \
        X(GAMEOBJECT_ON_GO_STATE_CHANGED,     5) \
        X(GAMEOBJECT_ON_UPDATE,               6)

    #define DEFINE_ITEM_EVENTS(X) \
        X(ITEM_ON_USE,        1) \
        X(ITEM_ON_EQUIP,      2) \
        X(ITEM_ON_UNEQUIP,    3) \
        X(ITEM_ON_REMOVE,     4) \
        X(ITEM_ON_EXPIRE,     5) \
        X(ITEM_ON_DESTROY,    6)

    // ========== AUTO-GENERATED ENUMS ==========

    enum PlayerEvents : uint32 {
    #define MAKE_ENUM(name, value) name = value,
        DEFINE_PLAYER_EVENTS(MAKE_ENUM)
    #undef MAKE_ENUM
    };

    enum MapEvents : uint32 {
    #define MAKE_ENUM(name, value) name = value,
        DEFINE_MAP_EVENTS(MAKE_ENUM)
    #undef MAKE_ENUM
    };

    enum CreatureEvents : uint32 {
    #define MAKE_ENUM(name, value) name = value,
        DEFINE_CREATURE_EVENTS(MAKE_ENUM)
    #undef MAKE_ENUM
    };

    enum GameObjectEvents : uint32 {
    #define MAKE_ENUM(name, value) name = value,
        DEFINE_GAMEOBJECT_EVENTS(MAKE_ENUM)
    #undef MAKE_ENUM
    };

    enum ItemEvents : uint32 {
    #define MAKE_ENUM(name, value) name = value,
        DEFINE_ITEM_EVENTS(MAKE_ENUM)
    #undef MAKE_ENUM
    };

    // ========== TYPE-SAFE WRAPPER STRUCTS ==========

    struct PlayerGuid
    {
        ObjectGuid::LowType guid;
        constexpr explicit PlayerGuid(ObjectGuid::LowType g) noexcept : guid(g) {}
        constexpr explicit PlayerGuid(const ObjectGuid& g) noexcept : guid(g.GetCounter()) {}
    };

    struct CreatureEntry
    {
        uint32 entry;
        constexpr explicit CreatureEntry(uint32 e) noexcept : entry(e) {}
    };

    struct GameObjectEntry
    {
        uint32 entry;
        constexpr explicit GameObjectEntry(uint32 e) noexcept : entry(e) {}
    };

    struct ItemEntry
    {
        uint32 entry;
        constexpr explicit ItemEntry(uint32 e) noexcept : entry(e) {}
    };

    struct MapId
    {
        uint32 mapId;
        constexpr explicit MapId(uint32 m) noexcept : mapId(m) {}
    };

    /**
     * Automatic Lua enum registration - use this in LuaEngine initialization
     */
    template<typename EventEnum>
    void RegisterEventsToLua(sol::state& lua, const std::string& tableName);

    // Template specializations for each event type
    template<>
    inline void RegisterEventsToLua<PlayerEvents>(sol::state& lua, const std::string& tableName)
    {
        auto table = lua.create_named_table(tableName);
    #define MAKE_LUA_BINDING(name, value) table[#name] = name;
        DEFINE_PLAYER_EVENTS(MAKE_LUA_BINDING)
    #undef MAKE_LUA_BINDING
    }

    template<>
    inline void RegisterEventsToLua<MapEvents>(sol::state& lua, const std::string& tableName)
    {
        auto table = lua.create_named_table(tableName);
    #define MAKE_LUA_BINDING(name, value) table[#name] = name;
        DEFINE_MAP_EVENTS(MAKE_LUA_BINDING)
    #undef MAKE_LUA_BINDING
    }

    template<>
    inline void RegisterEventsToLua<CreatureEvents>(sol::state& lua, const std::string& tableName)
    {
        auto table = lua.create_named_table(tableName);
    #define MAKE_LUA_BINDING(name, value) table[#name] = name;
        DEFINE_CREATURE_EVENTS(MAKE_LUA_BINDING)
    #undef MAKE_LUA_BINDING
    }

    template<>
    inline void RegisterEventsToLua<GameObjectEvents>(sol::state& lua, const std::string& tableName)
    {
        auto table = lua.create_named_table(tableName);
    #define MAKE_LUA_BINDING(name, value) table[#name] = name;
        DEFINE_GAMEOBJECT_EVENTS(MAKE_LUA_BINDING)
    #undef MAKE_LUA_BINDING
    }

    template<>
    inline void RegisterEventsToLua<ItemEvents>(sol::state& lua, const std::string& tableName)
    {
        auto table = lua.create_named_table(tableName);
    #define MAKE_LUA_BINDING(name, value) table[#name] = name;
        DEFINE_ITEM_EVENTS(MAKE_LUA_BINDING)
    #undef MAKE_LUA_BINDING
    }

    /**
     * Convenience function to register all event enums at once
     */
    inline void RegisterEventKeysToLua(sol::state& lua)
    {
        RegisterEventsToLua<PlayerEvents>(lua, "PlayerEvents");
        RegisterEventsToLua<MapEvents>(lua, "MapEvents");
        RegisterEventsToLua<CreatureEvents>(lua, "CreatureEvents");
        RegisterEventsToLua<GameObjectEvents>(lua, "GameObjectEvents");
        RegisterEventsToLua<ItemEvents>(lua, "ItemEvents");
    }
}

#endif // ECLIPSE_EVENT_TYPES_HPP