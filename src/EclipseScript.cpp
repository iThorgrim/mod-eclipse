// Eclipse Engine - AzerothCore Integration

#include "ScriptMgr.h"
#include "MapStateManager.hpp"
#include "EventDispatcher.hpp"
#include "Events.hpp"
#include "LuaCache.hpp"

class Eclipse_WorldScript : public WorldScript
{
public:
    Eclipse_WorldScript() : WorldScript("Eclipse_WorldScript", { 
        WORLDHOOK_ON_BEFORE_CONFIG_LOAD,
        WORLDHOOK_ON_SHUTDOWN
     }) { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload)
        {
            LOG_INFO("server.eclipse", "Initialize Eclipse Engine...");
            auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
            LOG_INFO("server.eclipse", "Eclipse Global Lua Engine {}", globalEngine ? "initialized" : "failed");
        }
    }

    void OnShutdown() override
    {
        LOG_INFO("server.eclipse", "Shutting down Eclipse Engine...");
        Eclipse::MapStateManager::GetInstance().UnloadAllStates();
    }
};

class Eclipse_PlayerScript : public PlayerScript
{
public:
    Eclipse_PlayerScript() : PlayerScript("Eclipse_PlayerScript", {
        PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_LOGOUT,
        PLAYERHOOK_ON_LOOT_ITEM
    }) { }

    void OnPlayerLogin(Player* player) override
    {
        auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
        if (globalEngine)
        {
            globalEngine->ProcessMessages();
        }
        
        Map* playerMap = player->GetMap();
        if (playerMap)
        {
            auto* mapEngine = Eclipse::MapStateManager::GetInstance().GetStateForMap(playerMap->GetId());
            if (mapEngine && mapEngine != globalEngine)
            {
                mapEngine->ProcessMessages();
            }
        }
        
        Eclipse::EventDispatcher::GetInstance().TriggerPlayerEvent(Eclipse::PLAYER_EVENT_ON_LOGIN, player);
    }
    
    void OnPlayerLogout(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerPlayerEvent(Eclipse::PLAYER_EVENT_ON_LOGOUT, player);
    }

    void OnPlayerLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerPlayerEvent(Eclipse::PLAYER_EVENT_ON_LOOT_ITEM, player, item, count, lootguid);
    }
};

class Eclipse_AllMapScript : public AllMapScript
{
public:
    Eclipse_AllMapScript() : AllMapScript("Eclipse_AllMapScript", { 
        ALLMAPHOOK_ON_CREATE_MAP,
        ALLMAPHOOK_ON_DESTROY_MAP,
        ALLMAPHOOK_ON_PLAYER_ENTER_ALL,
     }) { }

    void OnPlayerEnterAll(Map* map, Player* /*player*/) override
    {
        uint32 mapId = map->GetId();
        
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
    }

    void OnCreateMap(Map* map) override
    {
        uint32 mapId = map->GetId();
        LOG_INFO("server.eclipse", "Map {} created, initializing Lua state...", mapId);
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
    }

    void OnDestroyMap(Map* map) override
    {
        uint32 mapId = map->GetId();       
        Eclipse::MapStateManager::GetInstance().UnloadMapState(mapId);
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
        else if (cmd.find("eclipse cache clear") == 0)
        {
            Eclipse::GetGlobalCache().Clear();
            return false;
        }
        else if (cmd.find("eclipse cache stats") == 0)
        {
            Eclipse::GetGlobalCache().LogCacheStats();
            return false;
        }

        return true;
    }
};

void Addmod_eclipseScripts()
{
    new Eclipse_WorldScript();
    new Eclipse_PlayerScript();
    new Eclipse_AllMapScript();
    new Eclipse_CommandSC();
}
