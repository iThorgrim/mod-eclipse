#include "LuaEngine.hpp"
#include "EclipseIncludes.hpp"
#include "EclipseConfig.hpp"
#include "EventManager.hpp"
#include "MessageManager.hpp"
#include "ScriptLoader.hpp"
#include "LuaCompiler.hpp"
#include "LuaCache.hpp"
#include "LuaPathManager.hpp"
#include "EclipseLogger.hpp"
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

        auto startTime = std::chrono::high_resolution_clock::now();
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

            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            // Add this state's initialization time to the total
            EclipseLogger::GetInstance().AddStateInitializationTime(duration.count());

            return true;
        }
        catch (const std::exception& e)
        {
            EclipseLogger::GetInstance().LogStateInitialization(mapId, false);
            EclipseLogger::GetInstance().LogError("Failed to initialize Eclipse Lua Engine: " + std::string(e.what()));
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
            EclipseLogger::GetInstance().LogError("Cannot load script: Engine not initialized");
            return false;
        }

        if (ScriptLoader::LoadScript(GetState(), GetGlobalCompilerState(), scriptPath))
        {
            loadedScripts.push_back(scriptPath);
            EclipseLogger::GetInstance().LogScriptLoad(scriptPath, true);
            return true;
        }
        else
        {
            EclipseLogger::GetInstance().LogScriptLoad(scriptPath, false);
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
            EclipseLogger::GetInstance().LogDebug("Cleared all cached scripts for reload");
        }

        // Clear current scripts without destroying the state
        ClearStateData();

        // Reset Lua state but keep it initialized
        luaState.Reset();
        if (!luaState.Initialize())
        {
            EclipseLogger::GetInstance().LogError("Failed to reinitialize LuaState during reload");
            return;
        }

        RegisterBindings();

        // Reload scripts
        LoadScriptsForState();

        EclipseLogger::GetInstance().LogStateReload(stateMapId);
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

        EclipseLogger::GetInstance().LogStateShutdown(stateMapId);
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
            EclipseLogger::GetInstance().LogDebug("Global compiler state created and paths applied");
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
        EclipseLogger::GetInstance().LogDebug("Applied paths to state " + std::to_string(stateMapId));

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
            // Silent return for map states when no scripts are cached yet
            // This is normal behavior when global state hasn't compiled scripts yet
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
                    EclipseLogger::GetInstance().LogLuaError(scriptPath, "Failed to load cached script into state " + std::to_string(stateMapId));
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
            EclipseLogger::GetInstance().LogDebug("Global state (-1): Sequential compilation");
            ScriptLoader::LoadDirectory(GetState(), GetGlobalCompilerState(), scriptsDirectory, loadedScripts, &stats);
        }
        else
        {
            // Specific states: load from pre-compiled cache only
            EclipseLogger::GetInstance().LogDebug("State " + std::to_string(stateMapId) + ": Loading pre-compiled scripts from cache");

            auto startTime = std::chrono::high_resolution_clock::now();
            LoadCachedScriptsFromGlobalState();
            auto endTime = std::chrono::high_resolution_clock::now();

            stats.duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
            stats.compiled = 0;
            stats.cached = loadedScripts.size();
            stats.precompiled = 0;
        }
        EclipseLogger::GetInstance().LogLoadStatistics(stateMapId, stats.GetSuccessful(), stats.compiled, stats.cached, stats.precompiled, stats.duration);
    }
}