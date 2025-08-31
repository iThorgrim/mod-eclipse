#include "ScriptLoader.hpp"
#include "LuaCache.hpp"
#include "LuaCompiler.hpp"

#include <filesystem>
#include <algorithm>
#include <set>
#include <chrono>
#include <boost/filesystem.hpp>

namespace Eclipse
{
    bool ScriptLoader::IsValidScriptExtension(const std::string& extension)
    {
        return extension == ".ext" || extension == ".lua" || extension == ".moon" || extension == ".out";
    }

    std::vector<std::string> ScriptLoader::DiscoverScripts(const std::string& directoryPath)
    {
        std::vector<std::string> scripts;
        
        if (!boost::filesystem::exists(directoryPath) || !boost::filesystem::is_directory(directoryPath))
        {
            LOG_WARN("server.eclipse", "[Eclipse]: Script directory not found: {}", directoryPath);
            return scripts;
        }

        try
        {
            // Use Boost filesystem for better performance and cross-platform compatibility
            for (const auto& entry : boost::filesystem::recursive_directory_iterator(directoryPath))
            {
                if (boost::filesystem::is_regular_file(entry))
                {
                    std::string extension = entry.path().extension().string();
                    if (IsValidScriptExtension(extension))
                    {
                        scripts.push_back(entry.path().string());
                    }
                }
            }
            
            std::sort(scripts.begin(), scripts.end());
            LOG_DEBUG("server.eclipse", "[Boost] Discovered {} scripts in {}", scripts.size(), directoryPath);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Error discovering scripts in {}: {}", directoryPath, e.what());
        }

        return scripts;
    }
    
    std::set<std::string> ScriptLoader::ExtractDirectoriesFromScripts(const std::vector<std::string>& scriptPaths)
    {
        std::set<std::string> directories;
        
        for (const auto& scriptPath : scriptPaths)
        {
            boost::filesystem::path path(scriptPath);
            std::string parentDir = path.parent_path().string();
            if (!parentDir.empty())
            {
                directories.insert(parentDir);
            }
        }
        
        LOG_DEBUG("server.eclipse", "[Eclipse]: Extracted {} unique directories from {} scripts", directories.size(), scriptPaths.size());
        return directories;
    }

    bool ScriptLoader::LoadBytecodeIntoState(sol::state& targetState, const std::vector<char>& bytecode, const std::string& chunkName)
    {
        try
        {
            lua_State* L = targetState.lua_state();
            
            int result = luaL_loadbuffer(L, bytecode.data(), bytecode.size(), chunkName.c_str());
            if (result != LUA_OK)
            {
                std::string error = lua_tostring(L, -1);
                lua_pop(L, 1);
                LOG_ERROR("server.eclipse", "[Eclipse]: Failed to load bytecode for '{}': {}", chunkName, error);
                return false;
            }

            result = lua_pcall(L, 0, LUA_MULTRET, 0);
            if (result != LUA_OK)
            {
                std::string error = lua_tostring(L, -1);
                lua_pop(L, 1);
                LOG_ERROR("server.eclipse", "[Eclipse]: Failed to execute script '{}': {}", chunkName, error);
                return false;
            }

            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Exception loading bytecode for '{}': {}", chunkName, e.what());
            return false;
        }
    }

    bool ScriptLoader::LoadScript(sol::state& targetState, sol::state& compilerState, const std::string& filePath)
    {
        if (!std::filesystem::exists(filePath))
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Script file not found: {}", filePath);
            return false;
        }

        auto& cache = LuaCache::GetInstance();
        
        auto cachedBytecode = cache.GetBytecode(filePath);
        if (cachedBytecode.has_value())
        {
            LOG_TRACE("server.eclipse", "[Eclipse]: Loading script from cache: {}", filePath);
            return LoadBytecodeIntoState(targetState, cachedBytecode.value(), filePath);
        }

        LOG_TRACE("server.eclipse", "[Eclipse]: Compiling script: {}", filePath);
        auto bytecode = LuaCompiler::CompileFileTobytecode(compilerState, filePath);
        if (bytecode.empty())
        {
            cache.StoreBytecode(filePath, std::move(bytecode), false);
            return false;
        }

        bool success = LoadBytecodeIntoState(targetState, bytecode, filePath);
        cache.StoreBytecode(filePath, std::move(bytecode), success);
        
        return success;
    }

    int ScriptLoader::LoadFiles(sol::state& targetState, sol::state& compilerState, const std::vector<std::string>& files, std::vector<std::string>& loadedScripts, LoadStatistics* stats)
    {
        int successCount = 0;
        
        for (const auto& file : files)
        {
            if (stats) stats->total++;
            
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("server.eclipse", "[Eclipse]: Script file not found: {}", file);
                if (stats) stats->failed++;
                continue;
            }

            auto& cache = LuaCache::GetInstance();
            bool wasInCache = cache.GetBytecode(file).has_value();
            
            if (LoadScript(targetState, compilerState, file))
            {
                loadedScripts.push_back(file);
                successCount++;
                LOG_TRACE("server.eclipse", "[Eclipse]: Successfully loaded script: {}", file);
                
                if (stats)
                {
                    if (wasInCache)
                        stats->cached++;
                    else
                    {
                        std::filesystem::path path(file);
                        if (path.extension() == ".out")
                            stats->precompiled++;
                        else
                            stats->compiled++;
                    }
                }
            }
            else
            {
                LOG_WARN("server.eclipse", "[Eclipse]: Failed to load script: {}", file);
                if (stats) stats->failed++;
            }
        }
        
        return successCount;
    }

    void ScriptLoader::ProcessSubdirectories(sol::state& targetState, sol::state& compilerState, const std::string& directoryPath, std::vector<std::string>& loadedScripts, LoadStatistics* stats)
    {
        auto scripts = DiscoverScripts(directoryPath);
        if (!scripts.empty())
        {
            LOG_DEBUG("server.eclipse", "[Eclipse]: Loading {} scripts from directory: {}", scripts.size(), directoryPath);
            int loaded = LoadFiles(targetState, compilerState, scripts, loadedScripts, stats);
            LOG_DEBUG("server.eclipse", "[Eclipse]: Successfully loaded {}/{} scripts from {}", loaded, scripts.size(), directoryPath);
        }
    }

    bool ScriptLoader::LoadDirectory(sol::state& targetState, sol::state& compilerState, const std::string& directoryPath, std::vector<std::string>& loadedScripts, LoadStatistics* stats)
    {
        try
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            ProcessSubdirectories(targetState, compilerState, directoryPath, loadedScripts, stats);
            
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            
            if (stats)
            {
                stats->durationMs = static_cast<uint32>(duration.count());
            }
            
            return !loadedScripts.empty();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Exception loading directory '{}': {}", directoryPath, e.what());
            return false;
        }
    }
}