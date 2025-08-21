/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "MapStateManager.hpp"
#include "EventManager.hpp"
#include "Events.hpp"

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
            
            // Initialize the global Lua state
            auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
            if (!globalEngine)
            {
                LOG_ERROR("server.eclipse", "Failed to initialize Eclipse Global Lua Engine");
            }
            else
            {
                LOG_INFO("server.eclipse", "Eclipse Global Lua Engine initialized successfully");
            }
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
        PLAYERHOOK_ON_LOGIN
    }) { }

    void OnPlayerLogin(Player* player) override
    {
        // Trigger event on global state (-1) first
        auto* globalEngine = Eclipse::MapStateManager::GetInstance().GetGlobalState();
        if (globalEngine && globalEngine->GetEventManager())
        {
            globalEngine->GetEventManager()->TriggerPlayerEvent(Eclipse::PLAYER_EVENT_ON_LOGIN, player);
        }
        
        // Also trigger on current map state if player is in a map
        if (player->GetMap())
        {
            uint32 mapId = player->GetMap()->GetId();
            auto* mapEngine = Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
            if (mapEngine && mapEngine != globalEngine && mapEngine->GetEventManager()) // Don't trigger twice on the same engine
            {
                mapEngine->GetEventManager()->TriggerPlayerEvent(Eclipse::PLAYER_EVENT_ON_LOGIN, player);
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
     }) { }

    void OnPlayerEnterAll(Map* map, Player* player) override
    {
        uint32 mapId = map->GetId();
        
        // Ensure the Lua state exists for this map
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
    }

    void OnCreateMap(Map* map) override
    {
        uint32 mapId = map->GetId();
        LOG_INFO("server.eclipse", "Map {} created, initializing Lua state...", mapId);
        
        // Pre-create the Lua state for this map
        Eclipse::MapStateManager::GetInstance().GetStateForMap(mapId);
    }

    void OnDestroyMap(Map* map) override
    {
        uint32 mapId = map->GetId();
        LOG_INFO("server.eclipse", "Map {} destroyed, cleaning up Lua state...", mapId);
        
        // Clean up the Lua state for this map
        Eclipse::MapStateManager::GetInstance().UnloadMapState(mapId);
    }
};

class Eclipse_CommandSC : public CommandSC
{
public:
    Eclipse_CommandSC() : CommandSC("Eclipse_CommandSC", {
        ALLCOMMANDHOOK_ON_TRY_EXECUTE_COMMAND
    }) { }

    bool OnTryExecuteCommand(ChatHandler& handler, std::string_view cmdStr) override
    {
        std::string reload = std::string(cmdStr).c_str();
        std::transform(reload.begin(), reload.end(), reload.begin(), ::tolower);
        if (reload.find("reload eclipse") == 0)
        {
            Eclipse::MapStateManager::GetInstance().ReloadAllScripts();
            return false;
        }

        return true;
    }
};

void AddSC_EclipseScript()
{
    new Eclipse_WorldScript();
    new Eclipse_PlayerScript();
    new Eclipse_AllMapScript();
    new Eclipse_CommandSC();
}