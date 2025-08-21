#include "LuaEngine.hpp"
#include "LuaBindings.hpp"
#include "ScriptLoader.hpp"
#include "EventManager.hpp"
#include "Log.h"

namespace Eclipse
{
    LuaEngine::LuaEngine() : isInitialized(false), scriptsDirectory("lua_scripts"), stateMapId(-1), eventManager(std::make_unique<EventManager>())
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
            InitializeState();
            ConfigureOptimizations();
            RegisterBindings();
            
            isInitialized = true;
            
            LoadDirectory(scriptsDirectory);
            
            LOG_INFO("server.eclipse", "Eclipse Lua Engine initialized successfully for map {}", mapId);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to initialize Eclipse Lua Engine: {}", e.what());
            return false;
        }
    }

    void LuaEngine::Shutdown()
    {
        if (isInitialized)
        {
            cache.Clear();
            loadedScripts.clear();
            luaState = sol::state();
            isInitialized = false;
            LOG_INFO("server.eclipse", "Eclipse Lua Engine shutdown");
        }
    }

    bool LuaEngine::LoadScript(const std::string& scriptPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot load script: Engine not initialized");
            return false;
        }

        bool success = ScriptLoader::LoadFile(luaState, scriptPath);
        if (success)
        {
            cache.SetScriptTimestamp(scriptPath);
        }
        return success;
    }

    bool LuaEngine::ExecuteScript(const std::string& script)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot execute script: Engine not initialized");
            return false;
        }

        return ScriptLoader::LoadString(luaState, script);
    }

    void LuaEngine::RegisterBindings()
    {
        LuaBindings::Register(luaState, eventManager.get());
        
        // Override GetStateMapId to return this engine's mapId
        luaState["GetStateMapId"] = [this]() -> int32 {
            return this->stateMapId;
        };
    }

    bool LuaEngine::LoadDirectory(const std::string& directoryPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot load scripts: Engine not initialized");
            return false;
        }

        return ScriptLoader::LoadDirectory(luaState, directoryPath, loadedScripts);
    }

    void LuaEngine::ReloadScripts()
    {
        if (!isInitialized)
            return;

        LOG_INFO("server.eclipse", "Reloading Lua scripts...");
        
        // Clear events and cache
        if (eventManager)
        {
            eventManager->ClearAllEvents();
        }
        cache.Clear();
        loadedScripts.clear();
        
        InitializeState();
        ConfigureOptimizations();
        RegisterBindings();
        LoadDirectory(scriptsDirectory);
        
        LOG_INFO("server.eclipse", "Scripts reloaded successfully");
    }

    void LuaEngine::InitializeState()
    {
        luaState = sol::state();
        luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, 
                               sol::lib::math, sol::lib::table, sol::lib::os, sol::lib::io);
    }

    void LuaEngine::ConfigureOptimizations()
    {
        try 
        {
            // Enable JIT compilation if using LuaJIT
            luaState.script(R"(
                if jit then
                    jit.opt.start(3) -- Optimization level 3
                end
            )");
        }
        catch (const std::exception&)
        {
            // LuaJIT not available, continue with standard Lua
        }
        
        // Mark engine as optimized
        luaState["_ECLIPSE_OPTIMIZED"] = true;
    }
}