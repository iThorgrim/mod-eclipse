// Eclipse Engine - AzerothCore Integration

#include "ScriptMgr.h"
#include "LuaCache.hpp"
#include "EventDispatcher.hpp"
#include "MapStateManager.hpp"
#include "EclipseCreatureAI.hpp"
#include "Events.hpp"
#include "EclipseConfig.hpp"

class Eclipse_WorldScript : public WorldScript
{
public:
    Eclipse_WorldScript() : WorldScript("Eclipse_WorldScript", { 
        WORLDHOOK_ON_BEFORE_CONFIG_LOAD,
        WORLDHOOK_ON_SHUTDOWN
     }) { }

    void OnBeforeConfigLoad(bool reload) override
    {
        Eclipse::EclipseConfig::GetInstance().Initialize();

        if (Eclipse::EclipseConfig::GetInstance().IsEclipseEnabled())
        {
            if (!reload)
            {
                LOG_INFO("server.eclipse", "[Eclipse]: Initialize Eclipse Engine...");

                auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
                LOG_INFO("server.eclipse", "[Eclipse]: Eclipse Global Lua Engine {}", globalEngine ? "initialized" : "failed");
            }
        }
    }

    void OnShutdown() override
    {
        LOG_INFO("server.eclipse", "[Eclipse]: Shutting down Eclipse Engine...");
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
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYER_EVENT_ON_LOGIN, 
            player
        );
    }
    
    void OnPlayerLogout(Player* player) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYER_EVENT_ON_LOGOUT,
            player
        );
    }

    void OnPlayerLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override
    {
        Eclipse::EventDispatcher::GetInstance().TriggerEvent(
            Eclipse::PLAYER_EVENT_ON_LOOT_ITEM,
            player,
            item,
            count,
            lootguid
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
        
        // Process map-specific engine (will return global engine in compatibility mode)
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
    
    // Initialize automatic creature AI system
    AddEclipseCreatureAIScripts();
}
