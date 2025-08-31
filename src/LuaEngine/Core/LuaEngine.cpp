#include "LuaEngine.hpp"
#include "EclipseIncludes.hpp"
#include "EventManager.hpp"
#include "Methods/Methods.hpp"
#include "Methods/GlobalMethods.hpp"
#include "MessageManager.hpp"
#include "ScriptLoader.hpp"
#include "LuaCompiler.hpp"
#include "LuaCache.hpp"
#include "LuaPathManager.hpp"
#include <filesystem>
#include <mutex>
#include <chrono>

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
            InitializeComponents();
            RegisterBindings();
            
            isInitialized = true;
            
            if (stateMapId == -1)
            {
                // Global compiler state: discover and compile all scripts
                LOG_DEBUG("server.eclipse", "[Eclipse]: Global state (-1): Discovering and compiling scripts");
                LoadStatistics stats;
                ScriptLoader::LoadDirectory(GetState(), GetGlobalCompilerState(), scriptsDirectory, loadedScripts, &stats);
                
                LOG_INFO("server.eclipse", "[Eclipse]: Executed {} Lua scripts in {} ms into state {} ({} compiled, {} cached, {} pre-compiled)",
                    stats.GetSuccessful(), stats.durationMs, stateMapId, 
                    stats.compiled, stats.cached, stats.precompiled);
                    
                if (stats.failed > 0)
                {
                    LOG_WARN("server.eclipse", "[Eclipse]: {} scripts failed to load in state {}", stats.failed, stateMapId);
                }
            }
            else
            {
                // Specific states: load from pre-compiled cache only
                LOG_DEBUG("server.eclipse", "[Eclipse]: State {}: Loading pre-compiled scripts from cache", stateMapId);
                auto startTime = std::chrono::high_resolution_clock::now();
                
                LoadCachedScriptsFromGlobalState();
                
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                
                LOG_INFO("server.eclipse", "[Eclipse]: Executed {} Lua scripts in {} ms into state {} (0 compiled, {} cached, 0 pre-compiled)",
                    static_cast<int>(loadedScripts.size()), static_cast<uint32>(duration.count()), stateMapId, 
                    static_cast<int>(loadedScripts.size()));
            }
            
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

    bool LuaEngine::ExecuteScript(const std::string& script)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Cannot execute script: Engine not initialized");
            return false;
        }

        auto bytecode = LuaCompiler::CompileTobytecode(GetGlobalCompilerState(), script, "inline_script");
        if (bytecode.empty())
        {
            return false;
        }
        
        return ScriptLoader::LoadBytecodeIntoState(GetState(), bytecode, "inline_script");
    }

    void LuaEngine::ReloadScripts()
    {
        if (!isInitialized)
            return;
        
        int32 mapId = GetStateMapId();

        Shutdown();
        Initialize(mapId);
    }

    void LuaEngine::ProcessMessages()
    {
        if (isInitialized)
        {
            MessageManager::GetInstance().ProcessMessages(stateMapId);
        }
    }

    void LuaEngine::InitializeComponents()
    {
        if (!luaState.Initialize())
        {
            throw std::runtime_error("Failed to initialize LuaState");
        }
        
        luaState.EnableOptimizations();
        
        LOG_DEBUG("server.eclipse", "[Eclipse]: LuaEngine components initialized for map {}", stateMapId);
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
}