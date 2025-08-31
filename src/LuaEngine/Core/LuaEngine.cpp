#include "LuaEngine.hpp"
#include "EclipseIncludes.hpp"
#include "EclipseConfig.hpp"
#include "EventManager.hpp"
#include "MessageManager.hpp"
#include "ScriptLoader.hpp"
#include "LuaCompiler.hpp"
#include "LuaCache.hpp"
#include "LuaPathManager.hpp"
#include <filesystem>
#include <mutex>
#include <chrono>

#include "Methods.hpp"
#include "GlobalMethods.hpp"

namespace Eclipse
{
    LuaEngine::LuaEngine() : luaState(), isInitialized(false), scriptsDirectory("lua_scripts"), stateMapId(-1), eventManager(std::make_unique<EventManager>())
    {
    }

    LuaEngine::~LuaEngine()
    {
        Shutdown();
    }

    bool LuaEngine::Initialize(int32 mapId)
    {
        if (isInitialized)
            return true;

        stateMapId = mapId;

        try
        {
            if (!luaState.Initialize())
            {
                throw std::runtime_error("Failed to initialize LuaState");
            }
            
            RegisterBindings();
            
            isInitialized = true;
            
            LoadScriptsForState();
            
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to initialize Eclipse Lua Engine: {}", e.what());
            return false;
        }
    }

    void LuaEngine::Shutdown()
    {
        if (isInitialized)
        {
            ShutdownComponents();
            isInitialized = false;
        }
    }

    bool LuaEngine::LoadScript(const std::string& scriptPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Cannot load script: Engine not initialized");
            return false;
        }

        if (ScriptLoader::LoadScript(GetState(), GetGlobalCompilerState(), scriptPath))
        {
            loadedScripts.push_back(scriptPath);
            LOG_DEBUG("server.eclipse", "[Eclipse]: Loaded script: {}", scriptPath);
            return true;
        }
        else
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to load script: {}", scriptPath);
            return false;
        }
    }

    void LuaEngine::ReloadScripts()
    {
        if (!isInitialized)
            return;
        
        int32 mapId = GetStateMapId();
        
        // Invalide the cache to force recompilation of modified scripts
        if (mapId == -1)
        {
            // Global state: clear all cache to force recompilation
            LuaCache::GetInstance().InvalidateAllScripts();
            LOG_DEBUG("server.eclipse", "[Eclipse]: Cleared all cached scripts for reload");
        }

        // Clear current scripts without destroying the state
        ClearStateData();

        // Reset Lua state but keep it initialized
        luaState.Reset();
        if (!luaState.Initialize())
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to reinitialize LuaState during reload");
            return;
        }

        RegisterBindings();

        // Reload scripts
        LoadScriptsForState();

        LOG_INFO("server.eclipse", "[Eclipse]: Scripts reloaded for state {}", stateMapId);
    }

    void LuaEngine::ProcessMessages()
    {
        if (isInitialized)
        {
            MessageManager::GetInstance().ProcessMessages(stateMapId);
        }
    }

    void LuaEngine::ShutdownComponents()
    {
        // Clean up message handlers for this state
        MessageManager::GetInstance().ClearStateHandlers(stateMapId);

        // Clear all events for this state
        if (eventManager) eventManager->ClearAllEvents();
        
        // Clear loaded scripts
        loadedScripts.clear();
        
        // Reset Lua state
        luaState.Reset();
        
        LOG_DEBUG("server.eclipse", "[Eclipse]: LuaEngine components shutdown for map {}", stateMapId);
    }
    
    sol::state& LuaEngine::GetGlobalCompilerState()
    {
        static std::unique_ptr<sol::state> globalState;
        
        if (!globalState)
        {
            globalState = std::make_unique<sol::state>();
            globalState->open_libraries();
            
            // Initialize paths once and apply to global state
            LuaPathManager::GetInstance().InitializeDefaultPaths();
            LuaPathManager::GetInstance().ApplyPaths(*globalState);
            LOG_DEBUG("server.eclipse", "[Eclipse]: Global compiler state created and paths applied");
        }
        
        return *globalState;
    }
    
    void LuaEngine::RegisterBindings()
    {
        auto& state = luaState.GetState();
        
        // Ensure paths are initialized before applying
        LuaPathManager::GetInstance().InitializeDefaultPaths();
        
        // Apply pre-computed paths to this state
        LuaPathManager::GetInstance().ApplyPaths(state);
        LOG_DEBUG("server.eclipse", "[Eclipse]: Applied paths to state {}", stateMapId);
        
        Methods::RegisterAll(state);
        GlobalMethods::Register(this, state);

        if (eventManager)
            eventManager->Register(state);
    }
    
    bool LuaEngine::LoadCachedScriptsFromGlobalState()
    {
        auto& cache = LuaCache::GetInstance();
        auto cachedScripts = cache.GetAllCachedScripts();
        
        if (cachedScripts.empty())
        {
            LOG_WARN("server.eclipse", "[Eclipse]: No cached scripts found for state {}. Global state (-1) may not be initialized yet.", stateMapId);
            return false;
        }
        
        int successCount = 0;
        for (const auto& scriptPath : cachedScripts)
        {
            auto bytecode = cache.GetBytecode(scriptPath);
            if (bytecode.has_value())
            {
                if (ScriptLoader::LoadBytecodeIntoState(GetState(), bytecode.value(), scriptPath))
                {
                    loadedScripts.push_back(scriptPath);
                    successCount++;
                }
                else
                {
                    LOG_ERROR("server.eclipse", "[Eclipse]: Failed to load cached script '{}' into state {}", scriptPath, stateMapId);
                }
            }
        }
        
        return successCount > 0;
    }
    
    void LuaEngine::ClearStateData()
    {
        MessageManager::GetInstance().ClearStateHandlers(stateMapId);
        if (eventManager) eventManager->ClearAllEvents();
        loadedScripts.clear();
    }
    
    void LuaEngine::LoadScriptsForState()
    {
        LoadStatistics stats;
        if (stateMapId == -1)
        {
            // Global compiler state: discover and compile all scripts
            LOG_DEBUG("server.eclipse", "[Eclipse]: Global state (-1): Sequential compilation");
            ScriptLoader::LoadDirectory(GetState(), GetGlobalCompilerState(), scriptsDirectory, loadedScripts, &stats);
        }
        else
        {
            // Specific states: load from pre-compiled cache only
            LOG_DEBUG("server.eclipse", "[Eclipse]: State {}: Loading pre-compiled scripts from cache", stateMapId);

            auto startTime = std::chrono::high_resolution_clock::now();
            LoadCachedScriptsFromGlobalState();
            auto endTime = std::chrono::high_resolution_clock::now();

            stats.durationUs = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
            stats.durationMs = stats.durationUs * 1000;
            stats.compiled = 0;
            stats.cached = loadedScripts.size();
            stats.precompiled = 0;
        }
        LogScriptLoadStats(stats);
    }

    std::pair<uint32, std::string> LuaEngine::FormatDuration(uint32 microseconds)
    {
        if (microseconds >= 1000)
        {
            uint32 milliseconds = microseconds / 1000;
            return {milliseconds, "ms"};
        }
        return {microseconds, "μs"};
    }

    void LuaEngine::LogScriptLoadStats(const LoadStatistics& stats) const
    {
        uint32 displayDuration = (stats.durationUs > 0) ? stats.durationUs : (stats.durationMs * 1000);
        auto [duration, unit] = FormatDuration(displayDuration);
        LOG_INFO("server.eclipse", "[Eclipse]: Loaded {} scripts ({} compiled, {} cached, {} pre-compiled) in {}{} for state {}",
            stats.GetSuccessful(), stats.compiled, stats.cached, stats.precompiled, duration, unit, stateMapId);
    }

    void LuaEngine::LogCacheLoadStats(size_t scriptCount, uint32 durationUs) const
    {
        auto [duration, unit] = FormatDuration(durationUs);
        LOG_INFO("server.eclipse", "[Eclipse]: Loaded {} cached scripts in {}{} for state {}", 
            scriptCount, duration, unit, stateMapId);
    }
}