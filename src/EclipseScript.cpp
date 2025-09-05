// Eclipse Engine - AzerothCore Integration

#include "ScriptMgr.h"
#include "LuaCache.hpp"
#include "EventDispatcher.hpp"
#include "MapStateManager.hpp"
#include "EclipseCreatureAI.hpp"
#include "Events.hpp"
#include "EclipseConfig.hpp"
#include "EclipseLogger.hpp"
#include <any>
#include <optional>

class Eclipse_WorldScript : public WorldScript
{
public:
    Eclipse_WorldScript() : WorldScript("Eclipse_WorldScript", {
        WORLDHOOK_ON_BEFORE_CONFIG_LOAD,
        WORLDHOOK_ON_SHUTDOWN,
        WORLDHOOK_ON_STARTUP
     }) { }

    void OnBeforeConfigLoad(bool reload) override
    {
        Eclipse::EclipseConfig::GetInstance().Initialize(reload);

        if (Eclipse::EclipseConfig::GetInstance().IsEclipseEnabled() && !reload)
        {
            Eclipse::EclipseLogger::GetInstance().LogEngineStartup();
            Eclipse::EclipseLogger::GetInstance().LogInfo("Searching scripts from `lua_scripts`");

            // Create global state (-1) early - required for other states
            auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();

            if (globalEngine)
            {
                Eclipse::EclipseLogger::GetInstance().LogInfo("Eclipse Global Lua Engine initialized");
            }
            else
            {
                Eclipse::EclipseLogger::GetInstance().LogError("Eclipse Global Lua Engine failed to initialize");
            }
        }
    }

    void OnShutdown() override
    {
        Eclipse::EclipseLogger::GetInstance().LogEngineShutdown();
    }

    void OnStartup() override
    {
        if (Eclipse::EclipseConfig::GetInstance().IsEclipseEnabled())
        {
            Eclipse::EclipseLogger::GetInstance().LogTotalInitializationTime();
        }
    }
};

class Eclipse_PlayerScript : public PlayerScript
{
public:
    Eclipse_PlayerScript() : PlayerScript("Eclipse_PlayerScript", {
        PLAYERHOOK_ON_PLAYER_JUST_DIED,
        PLAYERHOOK_ON_CALCULATE_TALENTS_POINTS,
        PLAYERHOOK_ON_PLAYER_RELEASED_GHOST,
        PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP,
        PLAYERHOOK_ON_BATTLEGROUND_DESERTION,
        PLAYERHOOK_ON_PLAYER_COMPLETE_QUEST,
        PLAYERHOOK_ON_PVP_KILL,
        PLAYERHOOK_ON_PLAYER_PVP_FLAG_CHANGE,
        PLAYERHOOK_ON_CREATURE_KILL,
        PLAYERHOOK_ON_CREATURE_KILLED_BY_PET,
        PLAYERHOOK_ON_PLAYER_KILLED_BY_CREATURE,
        PLAYERHOOK_ON_LEVEL_CHANGED,
        PLAYERHOOK_ON_FREE_TALENT_POINTS_CHANGED,
        PLAYERHOOK_ON_TALENTS_RESET,
        PLAYERHOOK_ON_AFTER_SPEC_SLOT_CHANGED,
        PLAYERHOOK_ON_BEFORE_UPDATE,
        PLAYERHOOK_ON_UPDATE,
        PLAYERHOOK_ON_MONEY_CHANGED,
        PLAYERHOOK_ON_BEFORE_LOOT_MONEY,
        PLAYERHOOK_ON_GIVE_EXP,
        PLAYERHOOK_ON_REPUTATION_CHANGE,
        PLAYERHOOK_ON_REPUTATION_RANK_CHANGE,
        PLAYERHOOK_ON_LEARN_SPELL,
        PLAYERHOOK_ON_FORGOT_SPELL,
        PLAYERHOOK_ON_DUEL_REQUEST,
        PLAYERHOOK_ON_DUEL_START,
        PLAYERHOOK_ON_DUEL_END,
        PLAYERHOOK_ON_CHAT,
        PLAYERHOOK_ON_BEFORE_SEND_CHAT_MESSAGE,
        PLAYERHOOK_ON_CHAT_WITH_RECEIVER,
        PLAYERHOOK_ON_CHAT_WITH_GROUP,
        PLAYERHOOK_ON_CHAT_WITH_GUILD,
        PLAYERHOOK_ON_CHAT_WITH_CHANNEL,
        PLAYERHOOK_ON_EMOTE,
        PLAYERHOOK_ON_TEXT_EMOTE,
        PLAYERHOOK_ON_SPELL_CAST,
        PLAYERHOOK_ON_LOAD_FROM_DB,
        PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_BEFORE_LOGOUT,
        PLAYERHOOK_ON_LOGOUT,
        PLAYERHOOK_ON_CREATE,
        PLAYERHOOK_ON_DELETE,
        PLAYERHOOK_ON_FAILED_DELETE,
        PLAYERHOOK_ON_SAVE,
        PLAYERHOOK_ON_BIND_TO_INSTANCE,
        PLAYERHOOK_ON_UPDATE_ZONE,
        PLAYERHOOK_ON_UPDATE_AREA,
        PLAYERHOOK_ON_MAP_CHANGED,
        PLAYERHOOK_ON_BEFORE_TELEPORT,
        PLAYERHOOK_ON_UPDATE_FACTION,
        PLAYERHOOK_ON_ADD_TO_BATTLEGROUND,
        PLAYERHOOK_ON_QUEUE_RANDOM_DUNGEON,
        PLAYERHOOK_ON_REMOVE_FROM_BATTLEGROUND,
        PLAYERHOOK_ON_ACHI_COMPLETE,
        PLAYERHOOK_ON_BEFORE_ACHI_COMPLETE,
        PLAYERHOOK_ON_CRITERIA_PROGRESS,
        PLAYERHOOK_ON_BEFORE_CRITERIA_PROGRESS,
        PLAYERHOOK_ON_ACHI_SAVE,
        PLAYERHOOK_ON_CRITERIA_SAVE,
        PLAYERHOOK_ON_GOSSIP_SELECT,
        PLAYERHOOK_ON_GOSSIP_SELECT_CODE,
        PLAYERHOOK_ON_BEING_CHARMED,
        PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT,
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
        PLAYERHOOK_CAN_SEND_ERROR_ALREADY_LOOTED,
        PLAYERHOOK_ON_AFTER_CREATURE_LOOT,
        PLAYERHOOK_ON_AFTER_CREATURE_LOOT_MONEY,
        PLAYERHOOK_ON_CAN_UPDATE_SKILL,
        PLAYERHOOK_ON_BEFORE_UPDATE_SKILL,
        PLAYERHOOK_ON_UPDATE_SKILL,
        PLAYERHOOK_CAN_RESURRECT,
        PLAYERHOOK_ON_CAN_GIVE_LEVEL,
        PLAYERHOOK_ON_SEND_LIST_INVENTORY,
        PLAYERHOOK_ON_GIVE_REPUTATION
    }) { }

    void OnPlayerJustDied(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_JUST_DIED,
            player
        );
    }

    void OnPlayerCalculateTalentsPoints(Player const* player, uint32& talentPointsForLevel) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CALCULATE_TALENTS_POINTS,
            player,
            talentPointsForLevel
        );

        if (result && result->type() == typeid(uint32))
        {
            talentPointsForLevel = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerReleasedGhost(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_RELEASED_GHOST,
            player
        );
    }

    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket& data) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP,
            player,
            data
        );
    }

    void OnPlayerBattlegroundDesertion(Player* player, BattlegroundDesertionType const desertionType) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BATTLEGROUND_DESERTION,
            player,
            desertionType
        );
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest_id) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_COMPLETE_QUEST,
            player,
            quest_id
        );
    }

    void OnPlayerPVPKill(Player* killer, Player* killed) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PVP_KILL,
            killer,
            killed
        );
    }

    void OnPlayerPVPFlagChange(Player* player, bool state) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_PVP_FLAG_CHANGE,
            player,
            state
        );
    }

    void OnPlayerCreatureKill(Player* killer, Creature* killed) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CREATURE_KILL,
            killer,
            killed
        );
    }

    void OnPlayerCreatureKilledByPet(Player* PetOwner, Creature* killed) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CREATURE_KILLED_BY_PET,
            PetOwner,
            killed
        );
    }

    void OnPlayerKilledByCreature(Creature* killer, Player* killed) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_KILLED_BY_CREATURE,
            killed,
            killer
        );
    }

    void OnPlayerLevelChanged(Player* player, uint8 oldlevel) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LEVEL_CHANGED,
            player,
            oldlevel
        );
    }

    void OnPlayerFreeTalentPointsChanged(Player* player, uint32 points) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_FREE_TALENT_POINTS_CHANGED,
            player,
            points
        );
    }

    void OnPlayerTalentsReset(Player* player, bool noCost) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_TALENTS_RESET,
            player,
            noCost
        );
    }

    void OnPlayerAfterSpecSlotChanged(Player* player, uint8 newSlot) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_SPEC_SLOT_CHANGED,
            player,
            newSlot
        );
    }

    void OnPlayerBeforeUpdate(Player* player, uint32 p_time) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_UPDATE,
            player,
            p_time
        );
    }

    void OnPlayerUpdate(Player* player, uint32 p_time) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE,
            player,
            p_time
        );
    }

    void OnPlayerMoneyChanged(Player* player, int32& amount) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_MONEY_CHANGED,
            player,
            amount
        );

        if (result && result->type() == typeid(int32))
        {
            amount = std::any_cast<int32>(*result);
        }
    }

    void OnPlayerBeforeLootMoney(Player* player, Loot* loot) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_LOOT_MONEY,
            player,
            loot
        );
    }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GIVE_EXP,
            player,
            amount,
            victim,
            xpSource
        );

        if (result && result->type() == typeid(uint32))
        {
            amount = std::any_cast<uint32>(*result);
        }
    }

    bool OnPlayerReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_REPUTATION_CHANGE,
            player,
            factionID,
            standing,
            incremental
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["standing"].valid() && resultTable["standing"].is<int32>())
            {
                standing = resultTable["standing"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        else if (result && result->type() == typeid(int32))
        {
            standing = std::any_cast<int32>(*result);
        }

        return true;
    }

    void OnPlayerReputationRankChange(Player* player, uint32 factionID, ReputationRank newRank, ReputationRank oldRank, bool increased) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_REPUTATION_RANK_CHANGE,
            player,
            factionID,
            newRank,
            oldRank,
            increased
        );
    }

    void OnPlayerGiveReputation(Player* player, int32 factionID, float& amount, ReputationSource repSource) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GIVE_REPUTATION,
            player,
            factionID,
            amount,
            repSource
        );

        if (result && result->type() == typeid(float))
        {
            amount = std::any_cast<float>(*result);
        }
    }

    void OnPlayerLearnSpell(Player* player, uint32 spellID) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LEARN_SPELL,
            player,
            spellID
        );
    }

    void OnPlayerForgotSpell(Player* player, uint32 spellID) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_FORGOT_SPELL,
            player,
            spellID
        );
    }

    void OnPlayerDuelRequest(Player* target, Player* challenger) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_DUEL_REQUEST,
            target,
            challenger
        );
    }

    void OnPlayerDuelStart(Player* player1, Player* player2) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_DUEL_START,
            player1,
            player2
        );
    }

    void OnPlayerDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_DUEL_END,
            winner,
            loser,
            type
        );
    }

    void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CHAT,
            player,
            type,
            lang,
            msg
        );

        if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerBeforeSendChatMessage(Player* player, uint32& type, uint32& lang, std::string& msg) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_SEND_CHAT_MESSAGE,
            player,
            type,
            lang,
            msg
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["type"].valid() && resultTable["type"].is<uint32>())
            {
                type = resultTable["type"];
            }

            if (resultTable["lang"].valid() && resultTable["lang"].is<uint32>())
            {
                lang = resultTable["lang"];
            }

            if (resultTable["msg"].valid() && resultTable["msg"].is<std::string>())
            {
                msg = resultTable["msg"];
            }
        }
        else if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CHAT_WITH_RECEIVER,
            player,
            type,
            lang,
            msg,
            receiver
        );

        if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CHAT_WITH_GROUP,
            player,
            type,
            lang,
            msg,
            group
        );

        if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CHAT_WITH_GUILD,
            player,
            type,
            lang,
            msg,
            guild
        );

        if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CHAT_WITH_CHANNEL,
            player,
            type,
            lang,
            msg,
            channel
        );

        if (result && result->type() == typeid(std::string))
        {
            msg = std::any_cast<std::string>(*result);
        }
    }

    void OnPlayerEmote(Player* player, uint32 emote) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_EMOTE,
            player,
            emote
        );
    }

    void OnPlayerTextEmote(Player* player, uint32 textEmote, uint32 emoteNum, ObjectGuid guid) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_TEXT_EMOTE,
            player,
            textEmote,
            emoteNum,
            guid
        );
    }

    void OnPlayerSpellCast(Player* player, Spell* spell, bool skipCheck) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_SPELL_CAST,
            player,
            spell,
            skipCheck
        );
    }

    void OnPlayerLoadFromDB(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LOAD_FROM_DB,
            player
        );
    }

    void OnPlayerLogin(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LOGIN,
            player
        );
    }

    void OnPlayerBeforeLogout(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_LOGOUT,
            player
        );
    }

    void OnPlayerLogout(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LOGOUT,
            player
        );
    }

    void OnPlayerCreate(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CREATE,
            player
        );
    }

    void OnPlayerDelete(ObjectGuid guid, uint32 accountId) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_DELETE,
            guid,
            accountId
        );
    }

    void OnPlayerFailedDelete(ObjectGuid guid, uint32 accountId) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_FAILED_DELETE,
            guid,
            accountId
        );
    }

    void OnPlayerSave(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_SAVE,
            player
        );
    }

    void OnPlayerBindToInstance(Player* player, Difficulty difficulty, uint32 mapId, bool permanent) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BIND_TO_INSTANCE,
            player,
            difficulty,
            mapId,
            permanent
        );
    }

    void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_ZONE,
            player,
            newZone,
            newArea
        );
    }

    void OnPlayerUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_AREA,
            player,
            oldArea,
            newArea
        );
    }

    void OnPlayerMapChanged(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_MAP_CHANGED,
            player
        );
    }

    bool OnPlayerBeforeTeleport(Player* player, uint32 mapid, float x, float y, float z, float orientation, uint32 options, Unit* target) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_TELEPORT,
            player,
            mapid,
            x,
            y,
            z,
            orientation,
            options,
            target
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    void OnPlayerUpdateFaction(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_FACTION,
            player
        );
    }

    void OnPlayerAddToBattleground(Player* player, Battleground* bg) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_ADD_TO_BATTLEGROUND,
            player,
            bg
        );
    }

    void OnPlayerQueueRandomDungeon(Player* player, uint32& rDungeonId) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_QUEUE_RANDOM_DUNGEON,
            player,
            rDungeonId
        );

        if (result && result->type() == typeid(uint32))
        {
            rDungeonId = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerRemoveFromBattleground(Player* player, Battleground* bg) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_REMOVE_FROM_BATTLEGROUND,
            player,
            bg
        );
    }

    void OnPlayerAchievementComplete(Player* player, AchievementEntry const* achievement) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_ACHI_COMPLETE,
            player,
            achievement
        );
    }

    bool OnPlayerBeforeAchievementComplete(Player* player, AchievementEntry const* achievement) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_ACHI_COMPLETE,
            player,
            achievement
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    void OnPlayerCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CRITERIA_PROGRESS,
            player,
            criteria
        );
    }

    bool OnPlayerBeforeCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_CRITERIA_PROGRESS,
            player,
            criteria
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    void OnPlayerAchievementSave(CharacterDatabaseTransaction /*trans*/, Player* player, uint16 achId, CompletedAchievementData achiData) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_ACHI_SAVE,
            player,
            achId,
            achiData
        );
    }

    void OnPlayerCriteriaSave(CharacterDatabaseTransaction /*trans*/, Player* player, uint16 achId, CriteriaProgress criteriaData) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CRITERIA_SAVE,
            player,
            achId,
            criteriaData
        );
    }

    // void OnPlayerGossipSelect(Player* /*player*/, uint32 /*menu_id*/, uint32 /*sender*/, uint32 /*action*/)
    // void OnPlayerGossipSelectCode(Player* /*player*/, uint32 /*menu_id*/, uint32 /*sender*/, uint32 /*action*/, const char* /*code*/)

    void OnPlayerBeingCharmed(Player* player, Unit* charmer, uint32 oldFactionId, uint32 newFactionId) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_BEING_CHARMED,
            player,
            charmer,
            oldFactionId,
            newFactionId
        );
    }

    void OnPlayerAfterSetVisibleItemSlot(Player* player, uint8 slot, Item* item) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT,
            player,
            slot,
            item
        );
    }

    void OnPlayerAfterMoveItemFromInventory(Player* player, Item* it, uint8 bag, uint8 slot, bool update) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_MOVE_ITEM_FROM_INVENTORY,
            player,
            it,
            bag,
            slot,
            update
        );
    }

    void OnPlayerEquip(Player* player, Item* it, uint8 bag, uint8 slot, bool update) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_EQUIP,
            player,
            it,
            bag,
            slot,
            update
        );
    }

    void OnPlayerJoinBG(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_JOIN_BG,
            player
        );
    }

    void OnPlayerJoinArena(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_JOIN_ARENA,
            player
        );
    }

    void OnPlayerGetMaxPersonalArenaRatingRequirement(Player const* player, uint32 minSlot, uint32& maxArenaRating) const override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_MAX_PERSONAL_ARENA_RATING_REQUIREMENT,
            player,
            minSlot,
            maxArenaRating
        );

        if (result && result->type() == typeid(uint32))
        {
            maxArenaRating = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_LOOT_ITEM,
            player,
            item,
            count,
            lootguid
        );
    }

    void OnPlayerBeforeFillQuestLootItem(Player* player, LootItem& item) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_FILL_QUEST_LOOT_ITEM,
            player,
            item
        );

        if (result && result->type() == typeid(LootItem))
        {
            item = std::any_cast<LootItem>(*result);
        }
    }

    void OnPlayerStoreNewItem(Player* player, Item* item, uint32 count) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_STORE_NEW_ITEM,
            player,
            item,
            count
        );
    }

    void OnPlayerCreateItem(Player* player, Item* item, uint32 count) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_CREATE_ITEM,
            player,
            item,
            count
        );
    }

    void OnPlayerQuestRewardItem(Player* player, Item* item, uint32 count) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_QUEST_REWARD_ITEM,
            player,
            item,
            count
        );
    }

    bool OnPlayerCanPlaceAuctionBid(Player* player, AuctionEntry* auction) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_PLACE_AUCTION_BID,
            player,
            auction
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    void OnPlayerGroupRollRewardItem(Player* player, Item* item, uint32 count, RollVote voteType, Roll* roll) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_GROUP_ROLL_REWARD_ITEM,
            player,
            item,
            count,
            voteType,
            roll
        );
    }

    bool OnPlayerBeforeOpenItem(Player* player, Item* item) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_OPEN_ITEM,
            player,
            item
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    bool OnPlayerBeforeQuestComplete(Player* player, uint32 quest_id) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_QUEST_COMPLETE,
            player,
            quest_id
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    void OnPlayerQuestComputeXP(Player* player, Quest const* quest, uint32& xpValue) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_QUEST_COMPUTE_EXP,
            player,
            quest,
            xpValue
        );

        if (result && result->type() == typeid(uint32))
        {
            xpValue = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerBeforeDurabilityRepair(Player* player, ObjectGuid npcGUID, ObjectGuid itemGUID, float& discountMod, uint8 guildBank) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_DURABILITY_REPAIR,
            player,
            npcGUID,
            itemGUID,
            discountMod,
            guildBank
        );

        if (result && result->type() == typeid(float))
        {
            discountMod = std::any_cast<float>(*result);
        }
    }

    void OnPlayerBeforeBuyItemFromVendor(Player* player, ObjectGuid vendorguid, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_BUY_ITEM_FROM_VENDOR,
            player,
            vendorguid,
            vendorslot,
            item,
            count,
            bag,
            slot
        );

        if (result && result->type() == typeid(uint32))
        {
            item = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerBeforeStoreOrEquipNewItem(Player* player, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_STORE_OR_EQUIP_NEW_ITEM,
            player,
            vendorslot,
            item,
            count,
            bag,
            slot,
            pProto,
            pVendor,
            crItem,
            bStore
        );

        if (result && result->type() == typeid(uint32))
        {
            item = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerAfterStoreOrEquipNewItem(Player* player, uint32 vendorslot, Item* item, uint8 count, uint8 bag, uint8 slot, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_STORE_OR_EQUIP_NEW_ITEM,
            player,
            vendorslot,
            item,
            count,
            bag,
            slot,
            pProto,
            pVendor,
            crItem,
            bStore
        );
    }

    void OnPlayerAfterUpdateMaxPower(Player* player, Powers& power, float& value) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_UPDATE_MAX_POWER,
            player,
            power,
            value
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["power"].valid() && resultTable["power"].is<Powers>())
            {
                power = resultTable["power"];
            }

            if (resultTable["value"].valid() && resultTable["value"].is<float>())
            {
                value = resultTable["value"];
            }
        }
    }

    void OnPlayerAfterUpdateMaxHealth(Player* player, float& value) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_UPDATE_MAX_HEALTH,
            player,
            value
        );

        if (result && result->type() == typeid(float))
        {
            value = std::any_cast<float>(*result);
        }
    }

    void OnPlayerBeforeUpdateAttackPowerAndDamage(Player* player, float& level, float& val2, bool ranged) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_UPDATE_ATTACK_POWER_AND_DAMAGE,
            player,
            level,
            val2,
            ranged
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["level"].valid() && resultTable["level"].is<float>())
            {
                level = resultTable["level"];
            }

            if (resultTable["val2"].valid() && resultTable["val2"].is<float>())
            {
                val2 = resultTable["val2"];
            }
        }
    }

    void OnPlayerAfterUpdateAttackPowerAndDamage(Player* player, float& level, float& base_attPower, float& attPowerMod, float& attPowerMultiplier, bool ranged) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_UPDATE_ATTACK_POWER_AND_DAMAGE,
            player,
            level,
            base_attPower,
            attPowerMod,
            attPowerMultiplier,
            ranged
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["level"].valid() && resultTable["level"].is<float>())
            {
                level = resultTable["level"];
            }

            if (resultTable["base_attPower"].valid() && resultTable["base_attPower"].is<float>())
            {
                base_attPower = resultTable["base_attPower"];
            }

            if (resultTable["attPowerMod"].valid() && resultTable["attPowerMod"].is<float>())
            {
                attPowerMod = resultTable["attPowerMod"];
            }

            if (resultTable["attPowerMultiplier"].valid() && resultTable["attPowerMultiplier"].is<float>())
            {
                attPowerMultiplier = resultTable["attPowerMultiplier"];
            }
        }
    }

    void OnPlayerBeforeInitTalentForLevel(Player* player, uint8& level, uint32& talentPointsForLevel) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_INIT_TALENT_FOR_LEVEL,
            player,
            level,
            talentPointsForLevel
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["level"].valid() && resultTable["level"].is<uint8>())
            {
                level = resultTable["level"];
            }

            if (resultTable["talentPointsForLevel"].valid() && resultTable["talentPointsForLevel"].is<uint32>())
            {
                talentPointsForLevel = resultTable["talentPointsForLevel"];
            }
        }
    }

    void OnPlayerFirstLogin(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_FIRST_LOGIN,
            player
        );
    }

    void OnPlayerSetMaxLevel(Player* player, uint32& maxPlayerLevel) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_SET_MAX_LEVEL,
            player,
            maxPlayerLevel
        );

        if (result && result->type() == typeid(uint32))
        {
            maxPlayerLevel = std::any_cast<uint32>(*result);
        }
    }

    bool OnPlayerCanJoinInBattlegroundQueue(Player* player, ObjectGuid BattlemasterGuid, BattlegroundTypeId BGTypeID, uint8 joinAsGroup, GroupJoinBattlegroundResult& err) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_JOIN_IN_BATTLEGROUND_QUEUE,
            player,
            BattlemasterGuid,
            BGTypeID,
            joinAsGroup,
            err
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["error"].valid() && resultTable["error"].is<GroupJoinBattlegroundResult>())
            {
                err = resultTable["error"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        else if (result && result->type() == typeid(GroupJoinBattlegroundResult))
        {
            err = std::any_cast<GroupJoinBattlegroundResult>(*result);
        }

        return true;
    }

    bool OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(Player* player) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_SHOULD_BE_REWARDED_WITH_MONEY_INSTEAD_OF_EXP,
            player
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return false;
    }

    void OnPlayerBeforeTempSummonInitStats(Player* player, TempSummon* tempSummon, uint32& duration) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_TEMP_SUMMON_INIT_STATS,
            player,
            tempSummon,
            duration
        );

        if (result && result->type() == typeid(uint32))
        {
            duration = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerBeforeGuardianInitStatsForLevel(Player* player, Guardian* guardian, CreatureTemplate const* cinfo, PetType& petType) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_GUARDIAN_INIT_STATS_FOR_LEVEL,
            player,
            guardian,
            cinfo,
            petType
        );

        if (result && result->type() == typeid(PetType))
        {
            petType = std::any_cast<PetType>(*result);
        }
    }

    void OnPlayerAfterGuardianInitStatsForLevel(Player* player, Guardian* guardian) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_AFTER_GUARDIAN_INIT_STATS_FOR_LEVEL,
            player,
            guardian
        );
    }

    void OnPlayerBeforeLoadPetFromDB(Player* player, uint32& petentry, uint32& petnumber, bool& current, bool& forceLoadFromDB) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_BEFORE_LOAD_PET_FROM_DB,
            player,
            petentry,
            petnumber,
            current,
            forceLoadFromDB
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["petentry"].valid() && resultTable["petentry"].is<uint32>())
            {
                petentry = resultTable["petentry"];
            }

            if (resultTable["petnumber"].valid() && resultTable["petnumber"].is<uint32>())
            {
                petnumber = resultTable["petnumber"];
            }

            if (resultTable["current"].valid() && resultTable["current"].is<bool>())
            {
                current = resultTable["current"];
            }

            if (resultTable["forceLoadFromDB"].valid() && resultTable["forceLoadFromDB"].is<bool>())
            {
                forceLoadFromDB = resultTable["forceLoadFromDB"];
            }
        }
    }

    bool OnPlayerCanJoinInArenaQueue(Player* player, ObjectGuid BattlemasterGuid, uint8 arenaslot, BattlegroundTypeId BGTypeID, uint8 joinAsGroup, uint8 IsRated, GroupJoinBattlegroundResult& err) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_JOIN_IN_ARENA_QUEUE,
            player,
            BattlemasterGuid,
            arenaslot,
            BGTypeID,
            joinAsGroup,
            IsRated,
            err
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["error"].valid() && resultTable["error"].is<GroupJoinBattlegroundResult>())
            {
                err = resultTable["error"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        else if (result && result->type() == typeid(GroupJoinBattlegroundResult))
        {
            err = std::any_cast<GroupJoinBattlegroundResult>(*result);
        }

        return true;
    }

    bool OnPlayerCanBattleFieldPort(Player* player, uint8 arenaType, BattlegroundTypeId BGTypeID, uint8 action) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_BATTLEFIELD_PORT,
            player,
            arenaType,
            BGTypeID,
            action
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        return true;
    }

    bool OnPlayerCanGroupInvite(Player* player, std::string& membername) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_GROUP_INVITE,
            player,
            membername
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["membername"].valid() && resultTable["membername"].is<std::string>())
            {
                membername = resultTable["membername"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }
        else if (result && result->type() == typeid(std::string))
        {
            membername = std::any_cast<std::string>(*result);
        }

        return true;
    }

    bool OnPlayerCanGroupAccept(Player* player, Group* group) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_GROUP_ACCEPT,
            player,
            group
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanSellItem(Player* player, Item* item, Creature* creature) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_SELL_ITEM,
            player,
            item,
            creature
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanSendMail(Player* player, ObjectGuid receiverGuid, ObjectGuid mailbox, std::string& subject, std::string& body, uint32 money, uint32 COD, Item* item) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_SEND_MAIL,
            player,
            receiverGuid,
            mailbox,
            subject,
            body,
            money,
            COD,
            item
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["subject"].valid() && resultTable["subject"].is<std::string>())
            {
                subject = resultTable["subject"];
            }

            if (resultTable["body"].valid() && resultTable["body"].is<std::string>())
            {
                body = resultTable["body"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerPetitionBuy(Player* player, Creature* creature, uint32& charterid, uint32& cost, uint32& type) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_PETITION_BUY,
            player,
            creature,
            charterid,
            cost,
            type
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["charterid"].valid() && resultTable["charterid"].is<uint32>())
            {
                charterid = resultTable["charterid"];
            }

            if (resultTable["cost"].valid() && resultTable["cost"].is<uint32>())
            {
                cost = resultTable["cost"];
            }

            if (resultTable["type"].valid() && resultTable["type"].is<uint32>())
            {
                type = resultTable["type"];
            }
        }
    }

    void OnPlayerPetitionShowList(Player* player, Creature* creature, uint32& CharterEntry, uint32& CharterDispayID, uint32& CharterCost) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_PETITION_SHOW_LIST,
            player,
            creature,
            CharterEntry,
            CharterDispayID,
            CharterCost
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["CharterEntry"].valid() && resultTable["CharterEntry"].is<uint32>())
            {
                CharterEntry = resultTable["CharterEntry"];
            }

            if (resultTable["CharterDispayID"].valid() && resultTable["CharterDispayID"].is<uint32>())
            {
                CharterDispayID = resultTable["CharterDispayID"];
            }

            if (resultTable["CharterCost"].valid() && resultTable["CharterCost"].is<uint32>())
            {
                CharterCost = resultTable["CharterCost"];
            }
        }
    }

    void OnPlayerRewardKillRewarder(Player* player, KillRewarder* rewarder, bool isDungeon, float& rate) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_REWARD_KILL_REWARDER,
            player,
            rewarder,
            isDungeon,
            rate
        );

        if (result && result->type() == typeid(float))
        {
            rate = std::any_cast<float>(*result);
        }
    }

    bool OnPlayerCanGiveMailRewardAtGiveLevel(Player* player, uint8 level) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_GIVE_MAIL_REWARD_AT_GIVE_LEVEL,
            player,
            level
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerDeleteFromDB(CharacterDatabaseTransaction /*trans*/, uint32 guid) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYERHOOK_ON_DELETE_FROM_DB,
            Eclipse::PlayerGuid(guid)
        );
    }

    bool OnPlayerCanRepopAtGraveyard(Player* player) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_REPOP_AT_GRAVEYARD,
            player
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    Optional<bool> OnPlayerIsClass(Player const* player, Classes playerClass, ClassContext context) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_IS_CLASS,
            player,
            playerClass,
            context
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return std::nullopt;
    }

    void OnPlayerGetMaxSkillValue(Player* player, uint32 skill, int32& value, bool IsPure) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_MAX_SKILL_VALUE,
            player,
            skill,
            value,
            IsPure
        );

        if (result && result->type() == typeid(int32))
        {
            value = std::any_cast<int32>(*result);
        }
    }

    bool OnPlayerHasActivePowerType(Player const* player, Powers power) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_PLAYER_HAS_ACTIVE_POWER_TYPE,
            player,
            power
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return false;
    }

    void OnPlayerUpdateGatheringSkill(Player* player, uint32 skill_id, uint32 current, uint32 gray, uint32 green, uint32 yellow, uint32& gain) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_GATHERING_SKILL,
            player,
            skill_id,
            current,
            gray,
            green,
            yellow,
            gain
        );

        if (result && result->type() == typeid(uint32))
        {
            gain = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerUpdateCraftingSkill(Player* player, SkillLineAbilityEntry const* skill, uint32 current_level, uint32& gain) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_CRAFTING_SKILL,
            player,
            skill,
            current_level,
            gain
        );

        if (result && result->type() == typeid(uint32))
        {
            gain = std::any_cast<uint32>(*result);
        }
    }

    bool OnPlayerUpdateFishingSkill(Player* player, int32 skill, int32 zone_skill, int32 chance, int32 roll) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_UPDATE_FISHING_SKILL,
            player,
            skill,
            zone_skill,
            chance,
            roll
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanAreaExploreAndOutdoor(Player* player) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_AREA_EXPLORE_AND_OUTDOOR,
            player
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerVictimRewardBefore(Player* player, Player* victim, uint32& killer_title, uint32& victim_title) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_VICTIM_REWARD_BEFORE,
            player,
            victim,
            killer_title,
            victim_title
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["killer_title"].valid() && resultTable["killer_title"].is<uint32>())
            {
                killer_title = resultTable["killer_title"];
            }

            if (resultTable["victim_title"].valid() && resultTable["victim_title"].is<uint32>())
            {
                victim_title = resultTable["victim_title"];
            }
        }
    }

    void OnPlayerVictimRewardAfter(Player* player, Player* victim, uint32& killer_title, uint32& victim_rank, float& honor_f) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_VICTIM_REWARD_AFTER,
            player,
            victim,
            killer_title,
            victim_rank,
            honor_f
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["killer_title"].valid() && resultTable["killer_title"].is<uint32>())
            {
                killer_title = resultTable["killer_title"];
            }

            if (resultTable["victim_rank"].valid() && resultTable["victim_rank"].is<uint32>())
            {
                victim_rank = resultTable["victim_rank"];
            }

            if (resultTable["honor_f"].valid() && resultTable["honor_f"].is<uint32>())
            {
                honor_f = resultTable["honor_f"];
            }
        }
    }

    void OnPlayerCustomScalingStatValueBefore(Player* player, ItemTemplate const* proto, uint8 slot, bool apply, uint32& CustomScalingStatValue) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CUSTOM_SCALING_STAT_VALUE_BEFORE,
            player,
            proto,
            slot,
            apply,
            CustomScalingStatValue
        );

        if (result && result->type() == typeid(uint32))
        {
            CustomScalingStatValue = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerCustomScalingStatValue(Player* player, ItemTemplate const* proto, uint32& statType, int32& val, uint8 itemProtoStatNumber, uint32 ScalingStatValue, ScalingStatValuesEntry const* ssv) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_CUSTOM_SCALING_STAT_VALUE,
            player,
            proto,
            statType,
            val,
            itemProtoStatNumber,
            ScalingStatValue,
            ssv
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["statType"].valid() && resultTable["statType"].is<uint32>())
            {
                statType = resultTable["statType"];
            }

            if (resultTable["val"].valid() && resultTable["val"].is<int32>())
            {
                val = resultTable["val"];
            }
        }
    }

    void OnPlayerApplyItemModsBefore(Player* player, uint8 slot, bool apply, uint8 itemProtoStatNumber, uint32 statType, int32& val) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_APPLY_ITEM_MODS_BEFORE,
            player,
            slot,
            apply,
            itemProtoStatNumber,
            statType,
            val
        );

        if (result && result->type() == typeid(int32))
        {
            val = std::any_cast<int32>(*result);
        }
    }

    void OnPlayerApplyEnchantmentItemModsBefore(Player* player, Item* item, EnchantmentSlot slot, bool apply, uint32 enchant_spell_id, uint32& enchant_amount) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_APPLY_ENCHANTMENT_ITEM_MODS_BEFORE,
            player,
            item,
            slot,
            apply,
            enchant_spell_id,
            enchant_amount
        );

        if (result && result->type() == typeid(uint32))
        {
            enchant_amount = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerApplyWeaponDamage(Player* player, uint8 slot, ItemTemplate const* proto, float& minDamage, float& maxDamage, uint8 damageIndex) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_APPLY_WEAPON_DAMAGE,
            player,
            slot,
            proto,
            minDamage,
            maxDamage,
            damageIndex
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);

            if (resultTable["minDamage"].valid() && resultTable["minDamage"].is<float>())
            {
                minDamage = resultTable["minDamage"];
            }

            if (resultTable["maxDamage"].valid() && resultTable["maxDamage"].is<float>())
            {
                maxDamage = resultTable["maxDamage"];
            }
        }
    }

    bool OnPlayerCanArmorDamageModifier(Player* player) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_ARMOR_DAMAGE_MODIFIER,
            player
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerGetFeralApBonus(Player* player, int32& feral_bonus, int32 dpsMod, ItemTemplate const* proto, ScalingStatValuesEntry const* ssv) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_FERAL_AP_BONUS,
            player,
            feral_bonus,
            dpsMod,
            proto,
            ssv
        );

        if (result && result->type() == typeid(int32))
        {
            feral_bonus = std::any_cast<int32>(*result);
        }
    }

    bool OnPlayerCanApplyWeaponDependentAuraDamageMod(Player* player, Item* item, WeaponAttackType attackType, AuraEffect const* aura, bool apply) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_APPLY_WEAPON_DEPENDENT_AURA_DAMAGE_MOD,
            player,
            item,
            attackType,
            aura,
            apply
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanApplyEquipSpell(Player* player, SpellInfo const* spellInfo, Item* item, bool apply, bool form_change) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_APPLY_EQUIP_SPELL,
            player,
            spellInfo,
            item,
            apply,
            form_change
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanApplyEquipSpellsItemSet(Player* player, ItemSetEffect* eff) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_APPLY_EQUIP_SPELLS_ITEM_SET,
            player,
            eff
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanCastItemCombatSpell(Player* player, Unit* target, WeaponAttackType attType, uint32 procVictim, uint32 procEx, Item* item, ItemTemplate const* proto) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_CAST_ITEM_COMBAT_SPELL,
            player,
            target,
            attType,
            procVictim,
            procEx,
            item,
            proto
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanCastItemUseSpell(Player* player, Item* item, SpellCastTargets const& targets, uint8 cast_count, uint32 glyphIndex) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_CAST_ITEM_USE_SPELL,
            player,
            item,
            targets,
            cast_count,
            glyphIndex
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerApplyAmmoBonuses(Player* player, ItemTemplate const* proto, float& currentAmmoDPS) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_APPLY_AMMO_BONUSES,
            player,
            proto,
            currentAmmoDPS
        );

        if (result && result->type() == typeid(float))
        {
            currentAmmoDPS = std::any_cast<float>(*result);
        }
    }

    bool OnPlayerCanEquipItem(Player* player, uint8 slot, uint16& dest, Item* pItem, bool swap, bool not_loading) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_EQUIP_ITEM,
            player,
            slot,
            dest,
            pItem,
            swap,
            not_loading
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["dest"].valid() && resultTable["dest"].is<uint16>())
            {
                dest = resultTable["dest"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanUnequipItem(Player* player, uint16 pos, bool swap) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_UNEQUIP_ITEM,
            player,
            pos,
            swap
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanUseItem(Player* player, ItemTemplate const* proto, InventoryResult& inventoryResult) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_USE_ITEM,
            player,
            proto,
            inventoryResult
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["result"].valid() && resultTable["result"].is<InventoryResult>())
            {
                inventoryResult = resultTable["result"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanSaveEquipNewItem(Player* player, Item* item, uint16 pos, bool update) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_SAVE_EQUIP_NEW_ITEM,
            player,
            item,
            pos,
            update
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanApplyEnchantment(Player* player, Item* item, EnchantmentSlot slot, bool apply, bool apply_dur, bool ignore_condition) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_APPLY_ENCHANTMENT,
            player,
            item,
            slot,
            apply,
            apply_dur,
            ignore_condition
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerGetQuestRate(Player* player, float& rate) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_QUEST_RATE,
            player,
            rate
        );

        if (result && result->type() == typeid(float))
        {
            rate = std::any_cast<float>(*result);
        }
    }

    bool OnPlayerPassedQuestKilledMonsterCredit(Player* player, Quest const* qinfo, uint32 entry, uint32 real_entry, ObjectGuid guid) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_PASSED_QUEST_KILLED_MONSTER_CREDIT,
            player,
            qinfo,
            entry,
            real_entry,
            guid
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCheckItemInSlotAtLoadInventory(Player* player, Item* item, uint8 slot, uint8& err, uint16& dest) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CHECK_ITEM_IN_SLOT_AT_LOAD_INVENTORY,
            player,
            item,
            slot,
            err,
            dest
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["allow"].valid() && resultTable["allow"].is<bool>())
            {
                bool allow = resultTable["allow"];
                if (!allow)
                {
                    return false;
                }
            }

            if (resultTable["err"].valid() && resultTable["err"].is<uint8>())
            {
                err = resultTable["err"];
            }

            if (resultTable["dest"].valid() && resultTable["dest"].is<uint16>())
            {
                dest = resultTable["dest"];
            }
        }
        else if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerNotAvoidSatisfy(Player* player, DungeonProgressionRequirements const* ar, uint32 target_map, bool report) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_NOT_AVOID_SATISFY,
            player,
            ar,
            target_map,
            report
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerNotVisibleGloballyFor(Player* player, Player const* u) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_NOT_VISIBLE_GLOBALLY_FOR,
            player,
            u
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerGetArenaPersonalRating(Player* player, uint8 slot, uint32& rating) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_ARENA_PERSONAL_RATING,
            player,
            slot,
            rating
        );

        if (result && result->type() == typeid(uint32))
        {
            rating = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerGetArenaTeamId(Player* player, uint8 slot, uint32& rating) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_ARENA_TEAM_ID,
            player,
            slot,
            rating
        );

        if (result && result->type() == typeid(uint32))
        {
            rating = std::any_cast<uint32>(*result);
        }
    }

    void OnPlayerIsFFAPvP(Player* player, bool& active) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_IS_FFA_PVP,
            player,
            active
        );

        if (result && result->type() == typeid(bool))
        {
            active = std::any_cast<bool>(*result);
        }
    }

    void OnPlayerFfaPvpStateUpdate(Player* player, bool active) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_FFA_PVP_STATE_UPDATE,
            player,
            active
        );

        if (result && result->type() == typeid(bool))
        {
            active = std::any_cast<bool>(*result);
        }
    }

    void OnPlayerIsPvP(Player* player, bool& active) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_IS_PVP,
            player,
            active
        );

        if (result && result->type() == typeid(bool))
        {
            active = std::any_cast<bool>(*result);
        }
    }

    void OnPlayerGetMaxSkillValueForLevel(Player* player, uint16& value) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_GET_MAX_SKILL_VALUE_FOR_LEVEL,
            player,
            value
        );

        if (result && result->type() == typeid(uint16))
        {
            value = std::any_cast<uint16>(*result);
        }
    }

    bool OnPlayerNotSetArenaTeamInfoField(Player* player, uint8 slot, ArenaTeamInfoType type, uint32 value) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_NOT_SET_ARENA_TEAM_INFO_FIELD,
            player,
            slot,
            type,
            value
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanJoinLfg(Player* player, uint8 roles, std::set<uint32>& dungeons, const std::string& comment) override
    {

        return true; // PLAYERHOOK_CAN_JOIN_LFG
    }

    bool OnPlayerCanEnterMap(Player* player, MapEntry const* entry, InstanceTemplate const* instance, MapDifficulty const* mapDiff, bool loginCheck) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_ENTER_MAP,
            player,
            entry,
            instance,
            mapDiff,
            loginCheck
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanInitTrade(Player* player, Player* target) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_INIT_TRADE,
            player,
            target
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    bool OnPlayerCanSetTradeItem(Player* player, Item* tradedItem, uint8 tradeSlot) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_CAN_SET_TRADE_ITEM,
            player,
            tradedItem,
            tradeSlot
        );

        if (result && result->type() == typeid(bool))
        {
            return std::any_cast<bool>(*result);
        }

        return true;
    }

    void OnPlayerSetServerSideVisibility(Player* player, ServerSideVisibilityType& type, AccountTypes& sec) override
    {
        std::optional<std::any> result = Eclipse::EventDispatcher::GetInstance().TriggerWithRetValueEvent(
            Eclipse::PLAYERHOOK_ON_SET_SERVER_SIDE_VISIBILITY,
            player,
            type,
            sec
        );

        if (result && result->type() == typeid(sol::table))
        {
            sol::table resultTable = std::any_cast<sol::table>(*result);
            if (resultTable["type"].valid() && resultTable["type"].is<ServerSideVisibilityType>())
            {
                type = resultTable["type"];
            }

            if (resultTable["sec"].valid() && resultTable["sec"].is<AccountTypes>())
            {
                sec = resultTable["sec"];
            }
        }
    }
};

class Eclipse_AllMapScript : public AllMapScript
{
public:
    Eclipse_AllMapScript() : AllMapScript("Eclipse_AllMapScript", {
        ALLMAPHOOK_ON_CREATE_MAP,
        ALLMAPHOOK_ON_DESTROY_MAP,
        ALLMAPHOOK_ON_PLAYER_ENTER_ALL,
        ALLMAPHOOK_ON_MAP_UPDATE,
     }) { }

    void OnPlayerEnterAll(Map* map, Player* player) override
    {
        uint32 mapId = map->GetId();
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);

        Eclipse::EventDispatcher::GetInstance().TriggerEvent(Eclipse::MAP_EVENT_ON_PLAYER_ENTER,
            map,
            player
        );
    }

    void OnCreateMap(Map* map) override
    {
        uint32 mapId = map->GetId();
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
    }

    void OnDestroyMap(Map* map) override
    {
        uint32 mapId = map->GetId();
        Eclipse::MapStateManager::GetInstance().UnloadMapState(mapId);
    }

    void OnMapUpdate(Map* map, uint32 diff) override
    {
        auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
        if (globalEngine)
            globalEngine->ProcessMessages();

        auto* mapEngine = Eclipse::MapStateManager::GetInstance().GetStateForMap(map->GetId());
        if (mapEngine && mapEngine != globalEngine)
            mapEngine->ProcessMessages();

        Eclipse::EventDispatcher::GetInstance().TriggerEvent(Eclipse::MAP_EVENT_ON_UPDATE,
            map,
            diff
        );
    }
};

class Eclipse_CommandSC : public CommandSC
{
public:
    Eclipse_CommandSC() : CommandSC("Eclipse_CommandSC", {
        ALLCOMMANDHOOK_ON_TRY_EXECUTE_COMMAND
    }) { }

    bool OnTryExecuteCommand(ChatHandler& /*handler*/, std::string_view cmdStr) override
    {
        std::string cmd(cmdStr);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd.find("reload eclipse") == 0)
        {
            Eclipse::MapStateManager::GetInstance().ReloadAllScripts();
            return false;
        }

        return true;
    }
};

// Forward declaration
void AddEclipseCreatureAIScripts();

void Addmod_eclipseScripts()
{
    new Eclipse_WorldScript();
    new Eclipse_PlayerScript();
    new Eclipse_AllMapScript();
    new Eclipse_CommandSC();

    AddEclipseCreatureAIScripts();
}
