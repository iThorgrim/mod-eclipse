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
#include "LuaManager.hpp"
#include "LuaEngine.hpp"

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
            
            if (!Eclipse::LuaManager::GetInstance())
            {
                LOG_ERROR("server.eclipse", "Failed to initialize Eclipse Lua Engine");
            }
        }
    }

    void OnShutdown() override
    {
        LOG_INFO("server.eclipse", "Shutting down Eclipse Engine...");
        Eclipse::LuaManager::DestroyInstance();
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
            if (auto* engine = Eclipse::LuaManager::GetInstance())
                engine->ReloadScripts();
            return false;
        }

        return true;
    }
};

void AddSC_EclipseScript()
{
    new Eclipse_WorldScript();
    new Eclipse_CommandSC();
}