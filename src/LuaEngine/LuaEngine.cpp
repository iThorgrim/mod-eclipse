#include "LuaEngine.hpp"
#include "EclipseIncludes.hpp"
#include "EventManager.hpp"
#include "Methods/Methods.hpp"
#include "Methods/GlobalMethods.hpp"
#include "MessageManager.hpp"
#include "ScriptLoader.hpp"
#include <filesystem>

namespace Eclipse
{
    LuaEngine::LuaEngine() : luaState(), compiler(), isInitialized(false), scriptsDirectory("lua_scripts"), stateMapId(-1), eventManager(std::make_unique<EventManager>())
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
            
            ScriptLoader::LoadDirectory(GetState(), scriptsDirectory, loadedScripts);
            
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
            ShutdownComponents();
            isInitialized = false;
        }
    }

    bool LuaEngine::LoadScript(const std::string& scriptPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot load script: Engine not initialized");
            return false;
        }

        if (compiler.CompileAndExecute(luaState.GetState(), scriptPath))
        {
            loadedScripts.push_back(scriptPath);
            LOG_DEBUG("server.eclipse", "Loaded script: {}", scriptPath);
            return true;
        }
        else
        {
            LOG_ERROR("server.eclipse", "Failed to load script: {}", scriptPath);
            return false;
        }
    }

    bool LuaEngine::ExecuteScript(const std::string& script)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot execute script: Engine not initialized");
            return false;
        }

        return compiler.ExecuteScript(luaState.GetState(), script);
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
        compiler.ClearCache();
        
        LOG_DEBUG("server.eclipse", "LuaEngine components initialized for map {}", stateMapId);
    }

    void LuaEngine::ShutdownComponents()
    {
        // Clean up message handlers for this state
        MessageManager::GetInstance().ClearStateHandlers(stateMapId);

        // Clear all events for this state
        if (eventManager) eventManager->ClearAllEvents();
        
        // Clear loaded scripts
        loadedScripts.clear();
        
        // Clear compiler cache
        compiler.ClearCache();
        
        // Reset Lua state
        luaState.Reset();
        
        LOG_DEBUG("server.eclipse", "LuaEngine components shutdown for map {}", stateMapId);
    }
    
    void LuaEngine::RegisterBindings()
    {
        auto& state = luaState.GetState();
        
        Methods::RegisterAll(state);
        GlobalMethods::Register(this, state);

        if (eventManager)
            eventManager->Register(state);
    }
}