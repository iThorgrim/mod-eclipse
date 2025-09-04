#include "MapStateManager.hpp"
#include "EclipseIncludes.hpp"
#include "EclipseConfig.hpp"
#include "EclipseLogger.hpp"
#include <chrono>

namespace Eclipse
{
    MapStateManager& MapStateManager::GetInstance()
    {
        static MapStateManager instance;
        return instance;
    }

    LuaEngine* MapStateManager::GetStateForMap(int32 mapId)
    {
        // In compatibility mode, always return global state (-1) except when explicitly requested
        if (mapId != -1 && EclipseConfig::GetInstance().IsCompatibilityEnabled())
        {
            EclipseLogger::GetInstance().LogDebug("Compatibility mode: redirecting map " + std::to_string(mapId) + " to global state (-1)");
            return GetStateForMap(-1);
        }
        
        auto [it, inserted] = mapStates.try_emplace(mapId, nullptr);
        if (!inserted) return it->second.get();
        
        auto engine = std::make_unique<LuaEngine>();
        EclipseLogger::GetInstance().LogDebug("Creating new Lua state for map " + std::to_string(mapId));
        if (engine->Initialize(mapId))
        {
            EclipseLogger::GetInstance().LogStateInitialization(mapId, true);
            LuaEngine* enginePtr = engine.get();
            it->second = std::move(engine);
            return enginePtr;
        }
        
        mapStates.erase(it);
        EclipseLogger::GetInstance().LogStateInitialization(mapId, false);
        return nullptr;
    }

    LuaEngine* MapStateManager::GetGlobalState()
    {
        return GetStateForMap(-1);
    }


    void MapStateManager::UnloadMapState(int32 mapId)
    {
        auto it = mapStates.find(mapId);
        if (it != mapStates.end())
        {
            EclipseLogger::GetInstance().LogDebug("Unloading Lua state for map " + std::to_string(mapId));
            it->second->Shutdown();
            mapStates.erase(it);
        }
    }

    void MapStateManager::UnloadAllStates()
    {
        EclipseLogger::GetInstance().LogDebug("Unloading all Lua states (" + std::to_string(mapStates.size()) + " states)");
        for (auto& [mapId, engine] : mapStates)
        {
            engine->Shutdown();
        }
        mapStates.clear();
        EclipseLogger::GetInstance().LogInfo("Unloaded all Lua states");
    }

    void MapStateManager::ReloadAllScripts()
    {
        EclipseLogger::GetInstance().LogInfo("Searching scripts from `lua_scripts`");
        EclipseLogger::GetInstance().LogDebug("Starting script reload for " + std::to_string(mapStates.size()) + " states");
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        auto* globalEngine = GetGlobalState();
        if (globalEngine) 
        {
            EclipseLogger::GetInstance().LogDebug("Reloading global state (-1) scripts");
            globalEngine->ReloadScripts();
        }
        
        for (auto& [mapId, engine] : mapStates)
        {
            if (mapId != -1) 
            {
                EclipseLogger::GetInstance().LogDebug("Reloading scripts for map state " + std::to_string(mapId));
                engine->ReloadScripts();
            }
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        EclipseLogger::GetInstance().LogInfo("All scripts reloaded successfully in " + std::to_string(totalDuration.count()) + " ms");
    }
}