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
