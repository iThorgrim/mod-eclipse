#include "LuaManager.hpp"
#include "Log.h"

namespace Eclipse
{
    std::unique_ptr<LuaEngine> LuaManager::instance = nullptr;
    std::once_flag LuaManager::initialized;

    LuaEngine* LuaManager::GetInstance()
    {
        std::call_once(initialized, &LuaManager::CreateInstance);
        return instance.get();
    }

    void LuaManager::CreateInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<LuaEngine>();
            if (!instance->Initialize())
            {
                LOG_ERROR("server.eclipse", "Failed to initialize Eclipse Lua Engine");
                instance.reset();
            }
        }
    }

    void LuaManager::DestroyInstance()
    {
        if (instance)
        {
            instance->Shutdown();
            instance.reset();
        }
    }
}