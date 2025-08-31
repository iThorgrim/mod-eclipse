#include "LuaPathManager.hpp"
#include "ScriptLoader.hpp"
#include "../Core/EclipseConfig.hpp"
#include "EclipseIncludes.hpp"
#include <filesystem>
#include <sstream>
#include <boost/filesystem.hpp>

namespace Eclipse
{
    void LuaPathManager::InitializeDefaultPaths()
    {
        if (initialized) return;
        
        LOG_DEBUG("server.eclipse", "[Eclipse]: Initializing LuaPathManager with default paths");
        
        // Discover all lua_scripts directories
        DiscoverLuaScriptDirectories();
        
        // Add custom require paths from config
        AddConfigPaths();
        
        // Build final paths
        BuildPaths();
        
        initialized = true;
        
        LOG_DEBUG("server.eclipse", "[Eclipse]: LuaPathManager initialized. Lua path: {}", luaRequirePath);
        LOG_DEBUG("server.eclipse", "[Eclipse]: LuaPathManager C path: {}", luaRequireCPath);
    }
    
    void LuaPathManager::DiscoverLuaScriptDirectories()
    {
        std::string currentPath = boost::filesystem::current_path().string();
        std::string luaScriptsPath = currentPath + "/lua_scripts";
        
        // Reuse existing script discovery to find all directories containing scripts
        auto scripts = ScriptLoader::DiscoverScripts(luaScriptsPath);
        auto directories = ScriptLoader::ExtractDirectoriesFromScripts(scripts);
        
        // Also add base lua_scripts directory
        directories.insert(luaScriptsPath);
        
        for (const auto& dir : directories)
        {
            processedPaths.insert(dir);
            LOG_DEBUG("server.eclipse", "[Eclipse]: Added lua_scripts directory to paths: {}", dir);
        }
    }

    void LuaPathManager::AddSearchPath(const std::string& path)
    {
        try
        {
            if (processedPaths.insert(path).second)
            {
                BuildPaths();
                LOG_DEBUG("server.eclipse", "[Eclipse]: Added custom search path: {}", path);
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to add search path '{}': {}", path, e.what());
            throw;
        }
    }

    void LuaPathManager::ApplyPaths(sol::state& lua)
    {
        try
        {
            // Replace default paths completely with our own
            lua["package"]["path"] = luaRequirePath;
            lua["package"]["cpath"] = luaRequireCPath;
            
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to apply Lua paths: {}", e.what());
            throw;
        }
    }

    void LuaPathManager::Reset()
    {
        processedPaths.clear();
        luaRequirePath.clear();
        luaRequireCPath.clear();
        customRequirePath.clear();
        customRequireCPath.clear();
        initialized = false;
    }

    bool LuaPathManager::HasPath(const std::string& path) const
    {
        return processedPaths.find(path) != processedPaths.end();
    }

    void LuaPathManager::BuildPaths()
    {
        luaRequirePath.clear();
        luaRequireCPath.clear();

        // Pre-allocate strings to avoid reallocations
        size_t estimatedSize = processedPaths.size() * 100;
        luaRequirePath.reserve(estimatedSize);
        luaRequireCPath.reserve(estimatedSize);

        for (const auto& path : processedPaths)
        {
            if (!luaRequirePath.empty()) luaRequirePath += ";";
            luaRequirePath += path + "/?.ext;" +
                path + "/?.lua;" +
                path + "/?.out;" +
                path + "/?.moon;" +
                path + "/?/init.lua";
            
            if (!luaRequireCPath.empty()) luaRequireCPath += ";";
            luaRequireCPath += path + "/?.dll;" +
                path + "/?.so";
        }
        
        if (!customRequirePath.empty())
        {
            if (!luaRequirePath.empty()) luaRequirePath += ";";
            luaRequirePath += customRequirePath;
        }
        
        if (!customRequireCPath.empty())
        {
            if (!luaRequireCPath.empty()) luaRequireCPath += ";";
            luaRequireCPath += customRequireCPath;
        }
    }
    
    void LuaPathManager::AddConfigPaths()
    {
        auto& config = EclipseConfig::GetInstance();
        
        customRequirePath = std::string(config.GetRequirePathExtra());
        customRequireCPath = std::string(config.GetRequireCPathExtra());
    }
}