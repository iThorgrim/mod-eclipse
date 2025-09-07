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
         * @brief Register a callback function for a specific player event
         *
         * @enum PlayerEvents
         * @enum {
         * @enum    PLAYER_EVENT_ON_CHARACTER_CREATE    = 1,    // Triggered when character is created | Context: WORLD | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_CHARACTER_DELETE    = 2,    // Triggered when character is deleted | Context: WORLD | Args: (event, guid)
         * @enum    PLAYER_EVENT_ON_LOGIN               = 3,    // Triggered when player loggin | Context: WORLD & MAP | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_LOGOUT              = 4,    // Triggered when player logout | Context: WORLD & MAP | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_SPELL_CAST          = 5,    // Triggered when player cast spell | Context: WORLD & MAP | Args: (event, player, spell, skipCheck)
         * @enum    PLAYER_EVENT_ON_KILL_PLAYER         = 6,    // Triggered when player kill player | Context: WORLD & MAP | Args: (event, killer, killed)
         * @enum    PLAYER_EVENT_ON_KILL_CREATURE       = 7,    // Triggered when player kill creature | Context: WORLD & MAP | Args: (event, killer, killed)
         * @enum    PLAYER_EVENT_ON_KILLED_BY_CREATURE  = 8,    // Trigerred when player is killed by creature | Context: WORLD & MAP | Args: (event, killed, killer)
         * @enum    PLAYER_EVENT_ON_DUEL_REQUEST        = 9,    // Triggered when player received duel request | Context: WORLD & MAP | Args: (event, target, challenger)
         * @enum    PLAYER_EVENT_ON_DUEL_START          = 10,    // Triggered when player duel start | Context: WORLD & MAP | Args: (event, player1, player2)
         * @enum    PLAYER_EVENT_ON_DUEL_END            = 11,    // Triggered when player duel end | Context: WORLD & MAP | Args: (event, winner, loser, type)
         * @enum    PLAYER_EVENT_ON_GIVE_XP             = 12,    // Triggered before player received exp <br>*(you can return new amount)* | Context: WORLD & MAP | Args: (event, player, amount, victim, xp_source)
         * @enum    PLAYER_EVENT_ON_LEVEL_CHANGE        = 13,    // Triggered when player level change | Context: WORLD & MAP | Args: (event, player, old_level)
         * @enum    PLAYER_EVENT_ON_MONEY_CHANGE        = 14,    // Triggered before player money changed <br>*(you can return new amount)* | Context: WORLD & MAP | Args: (event, player, amount)
         * @enum    PLAYER_EVENT_ON_REPUTATION_CHANGE   = 15,    // Triggered before player reputation changed <br>*(you can return table with key "allow" and "standing")* | Context: WORLD & MAP | Args: (event, player, faction_id, standing, incremental)
         * @enum    PLAYER_EVENT_ON_TALENTS_CHANGE      = 16,    // Triggered when player talent changed | Context: WORLD & MAP | Args: (event, player, points)
         * @enum    PLAYER_EVENT_ON_TALENTS_RESET       = 17,    // Triggered when player talent reset | Context: WORLD & MAP | Args: (event, player, no_cost)
         * @enum    PLAYER_EVENT_ON_CHAT                = 18,    // Triggered before player send message <br>*(you can return the new message)* | Context: WORLD & MAP | Args: (event, player, type, lang, message)
         * @enum    PLAYER_EVENT_ON_WHISPER             = 19,    // Triggered before player whisper message <br>*(you can return the new message)* | Context: WORLD & MAP | Args: (event, player, type, lang, message, receiver)
         * @enum    PLAYER_EVENT_ON_GROUP_CHAT          = 20,    // Triggered before player send group message <br>*(you can return the new message)* | Context: WORLD & MAP | Args: (event, player, type, lang, message, group)
         * @enum    PLAYER_EVENT_ON_GUILD_CHAT          = 21,    // Triggered before player send guild message <br>*(you can return the new message)* | Context: WORLD & MAP | Args: (event, player, type, lang, message, guild)
         * @enum    PLAYER_EVENT_ON_CHANNEL_CHAT        = 22,    // Triggered before player send channel message <br>*(you can return the new message)* | Context: WORLD & MAP | Args: (event, player, type, lang, message, channel)
         * @enum    PLAYER_EVENT_ON_EMOTE               = 23,    // Triggered when player perform an emote | Context: WORLD & MAP | Args: (event, player, emote)
         * @enum    PLAYER_EVENT_ON_EMOTE               = 24,    // Triggered when player perform a text emote | Context: WORLD & MAP | Args: (event, player, test_emote, emote_num, guid)
         * @enum    PLAYER_EVENT_ON_EMOTE               = 25,    // Triggered when player is saved | Context: WORLD & MAP | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_BIND_TO_INSTANCE    = 26,    // Triggered when player is bind to instance | Context: WORLD & MAP | Args: (event, player, difficulty, map_id, permanent)
         * @enum    PLAYER_EVENT_ON_UPDATE_ZONE         = 27,    // Triggered when player change zone | Context: WORLD & MAP | Args: (event, player, new_zone, new_area)
         * @enum    PLAYER_EVENT_ON_MAP_CHANGE          = 28,    // Triggered when player change map | Context: WORLD & MAP | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_EQUIP               = 29,    // Triggered when player equip item | Context: WORLD & MAP | Args: (event, player, item, bag, slot, update)
         * @enum    PLAYER_EVENT_ON_FIRST_LOGIN         = 30,    // Triggered when player first time login | Context: WORLD & MAP | Args: (event, player)
         * @enum    PLAYER_EVENT_ON_CAN_USE_ITEM        = 31,    // Triggered before player use item <br>*(you can return table with key "allow" and "result")*| Context: WORLD & MAP | Args: (event, player, item_template, inventory_result)
         * @enum PLAYER_EVENT_ON_LOOT_ITEM = 32 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_REPOP_AT_GRAVEYARD = 35 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_LEARN_SPELL = 44 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_ACHI_COMPLETE = 45 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE_AREA = 47 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_INIT_TRADE = 48 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_SEND_MAIL = 49 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_JOIN_LFG = 50 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_QUEST_REWARD_ITEM = 51 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CREATE_ITEM = 52 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_STORE_NEW_ITEM = 53 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_COMPLETE_QUEST = 54 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_GROUP_INVITE = 55 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GROUP_ROLL_REWARD_ITEM = 56 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BG_DESERTION = 57 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PET_KILL = 58 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_JUST_DIED = 64 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CALCULATE_TALENT_POINTS = 65 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_RELEASED_GHOST = 66 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_SEND_INITIAL_PACKET_BEFORE_ADD_TO_MAP = 67 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PVP_FLAG_CHANGE = 68 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_SPEC_SLOT_CHANGED = 69 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_UPDATE = 70 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE = 71 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_LOOT_MONEY = 72 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_REPUTATION_RANK_CHANGE = 73 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GIVE_REPUTATION = 74 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_FORGOT_SPELL = 75 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_SEND_CHAT_MESSAGE = 76 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_LOAD_FROM_DB = 77 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_LOGOUT = 78 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_FAILED_DELETE = 79 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_TELEPORT = 80 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE_FACTION = 81 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_ADD_TO_BATTLEGROUND = 82 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_QUEUE_RANDOM_DUNGEON = 83 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_REMOVE_FROM_BATTLEGROUND = 84 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_ACHI_COMPLETE = 85 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CRITERIA_PROGRESS = 86 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_CRITERIA_PROGRESS = 87 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_ACHI_SAVE = 88 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CRITERIA_SAVE = 89 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEING_CHARMED = 90 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_SET_VISIBLE_ITEM_SLOT = 91 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_MOVE_ITEM_FROM_INVENTORY = 92 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PLAYER_JOIN_BG = 93 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PLAYER_JOIN_ARENA = 94 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_MAX_PERSONAL_ARENA_RATING_REQUIREMENT = 95 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_FILL_QUEST_LOOT_ITEM = 96 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_PLACE_AUCTION_BID = 97 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_OPEN_ITEM = 98 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_QUEST_COMPLETE = 99 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_QUEST_COMPUTE_EXP = 100 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_DURABILITY_REPAIR = 101 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_BUY_ITEM_FROM_VENDOR = 102 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_STORE_OR_EQUIP_NEW_ITEM = 103 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_STORE_OR_EQUIP_NEW_ITEM = 104 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_UPDATE_MAX_POWER = 105 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_UPDATE_MAX_HEALTH = 106 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_UPDATE_ATTACK_POWER_AND_DAMAGE = 107 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_UPDATE_ATTACK_POWER_AND_DAMAGE = 108 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_INIT_TALENT_FOR_LEVEL = 109 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_SET_MAX_LEVEL = 110 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_JOIN_IN_BATTLEGROUND_QUEUE = 111 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_SHOULD_BE_REWARDED_WITH_MONEY_INSTEAD_OF_EXP = 112 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_TEMP_SUMMON_INIT_STATS = 113 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_GUARDIAN_INIT_STATS_FOR_LEVEL = 114 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_AFTER_GUARDIAN_INIT_STATS_FOR_LEVEL = 115 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_BEFORE_LOAD_PET_FROM_DB = 116 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_JOIN_IN_ARENA_QUEUE = 117 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_BATTLEFIELD_PORT = 118 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_GROUP_ACCEPT = 119 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_SELL_ITEM = 120 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_PETITION_BUY = 121 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_PETITION_SHOW_LIST = 122 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_REWARD_KILL_REWARDER = 123 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_GIVE_MAIL_REWARD_AT_GIVE_LEVEL = 124 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_DELETE_FROM_DB = 125 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PLAYER_IS_CLASS = 126 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_MAX_SKILL_VALUE = 127 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_PLAYER_HAS_ACTIVE_POWER_TYPE = 128 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE_GATHERING_SKILL = 129 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE_CRAFTING_SKILL = 130 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_UPDATE_FISHING_SKILL = 131 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_AREA_EXPLORE_AND_OUTDOOR = 132 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_VICTIM_REWARD_BEFORE = 133 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_VICTIM_REWARD_AFTER = 134 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CUSTOM_SCALING_STAT_VALUE_BEFORE = 135 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CUSTOM_SCALING_STAT_VALUE = 136 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_APPLY_ITEM_MODS_BEFORE = 137 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_APPLY_ENCHANTMENT_ITEM_MODS_BEFORE = 138 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_APPLY_WEAPON_DAMAGE = 139 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_ARMOR_DAMAGE_MODIFIER = 140 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_FERAL_AP_BONUS = 141 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_APPLY_WEAPON_DEPENDENT_AURA_DAMAGE_MOD = 142 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_APPLY_EQUIP_SPELL = 143 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_APPLY_EQUIP_SPELLS_ITEM_SET = 144 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_CAST_ITEM_COMBAT_SPELL = 145 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_CAST_ITEM_USE_SPELL = 146 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_APPLY_AMMO_BONUSES = 147 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_EQUIP_ITEM = 148 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_UNEQUIP_ITEM = 149 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_SAVE_EQUIP_NEW_ITEM = 150 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_APPLY_ENCHANTMENT = 151 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_QUEST_RATE = 152 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_PASSED_QUEST_KILLED_MONSTER_CREDIT = 153 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_CHECK_ITEM_IN_SLOT_AT_LOAD_INVENTORY = 154 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_NOT_AVOID_SATISFY = 155 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_NOT_VISIBLE_GLOBALLY_FOR = 156 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_ARENA_PERSONAL_RATING = 157 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_ARENA_TEAM_ID = 158 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_IS_FFA_PVP = 159 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_FFA_PVP_STATE_UPDATE = 160 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_IS_PVP = 161 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_GET_MAX_SKILL_VALUE_FOR_LEVEL = 162 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_NOT_SET_ARENA_TEAM_INFO_FIELD = 163 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_SET_TRADE_ITEM = 164 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_SET_SERVER_SIDE_VISIBILITY = 165 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum PLAYER_EVENT_ON_CAN_ENTER_MAP = 166 // NOT DOCUMENTED YET  | Context: WORLD & MAP | Args: (event, player)
         * @enum }
         *
         * @code {.lua}
         * local function OnPlayerLogin(event, player)
         *     print("Player " .. player:GetName() .. " logged in")
         * end
         * RegisterPlayerEvent(3, OnPlayerLogin)
         *
         * local function OnPlayerChangeReputation(event, player, faction_id, standing, incremental)
         *      if (player:GetName() == "Eclipse") then
         *          return false -- This disabled reputation gain
         *      end
         *      return { ["allow"] = true, ["standing"] = standing * 2 } -- This multiply reputation gain
         * end
         * @endcode
         *
         * @param uint32 eventId The player event ID to register for
         * @param function callback The Lua function to call when event triggers
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