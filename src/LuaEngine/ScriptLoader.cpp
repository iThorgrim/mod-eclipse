#include "ScriptLoader.hpp"
#include "Log.h"
#include <filesystem>
#include <algorithm>

namespace Eclipse
{
    bool ScriptLoader::LoadFile(sol::state& lua, const std::string& filePath)
    {
        try
        {
            lua.script_file(filePath);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to load script '{}': {}", filePath, e.what());
            return false;
        }
    }
    
    bool ScriptLoader::LoadString(sol::state& lua, const std::string& script)
    {
        try
        {
            lua.script(script);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to execute script: {}", e.what());
            return false;
        }
    }
    
    bool ScriptLoader::LoadDirectory(sol::state& lua, const std::string& directoryPath, 
                                   std::vector<std::string>& loadedScripts)
    {
        if (!std::filesystem::exists(directoryPath))
        {
            return true; // Silent exit if directory doesn't exist
        }

        auto luaFiles = ScanDirectory(directoryPath);
        
        if (luaFiles.empty())
        {
            return true; // Silent exit if no scripts found
        }

        int loadedCount = 0;
        for (const auto& filePath : luaFiles)
        {
            if (LoadFile(lua, filePath))
            {
                loadedScripts.push_back(filePath);
                loadedCount++;
            }
        }

        LOG_INFO("server.eclipse", "Loaded {} Lua script{}", loadedCount, 
                loadedCount > 1 ? "s" : "");
        return loadedCount > 0;
    }
    
    std::vector<std::string> ScriptLoader::ScanDirectory(const std::string& path)
    {
        std::vector<std::string> luaFiles;
        
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    const auto& filePath = entry.path();
                    if (filePath.extension() == ".lua")
                    {
                        luaFiles.push_back(filePath.string());
                    }
                }
            }
            
            // Sort files alphabetically for consistent loading order
            std::sort(luaFiles.begin(), luaFiles.end());
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to scan directory '{}': {}", path, e.what());
        }
        
        return luaFiles;
    }
}