#include "ScriptLoader.hpp"
#include "LuaCache.hpp"
#include "LuaCompiler.hpp"
#include "EclipseLogger.hpp"

#include <filesystem>
#include <algorithm>
#include <set>
#include <chrono>
#include <future>
#include <unordered_map>
#include <ctime>
#include <mutex>
#include <thread>
#include <boost/filesystem.hpp>

namespace Eclipse
{
    bool ScriptLoader::IsValidScriptExtension(const std::string& extension)
    {
        return extension == ".ext" || extension == ".lua" || extension == ".moon" || extension == ".out";
    }

    std::vector<std::string> ScriptLoader::DiscoverScripts(const std::string& directoryPath)
    {
        static std::unordered_map<std::string, std::pair<std::vector<std::string>, std::time_t>> discoverCache;

        if (!boost::filesystem::exists(directoryPath) || !boost::filesystem::is_directory(directoryPath))
        {
            EclipseLogger::GetInstance().LogScriptNotFound(directoryPath, true);
            return {};
        }

        auto lastWrite = boost::filesystem::last_write_time(directoryPath);
        auto it = discoverCache.find(directoryPath);
        if (it != discoverCache.end() && it->second.second >= lastWrite) {
            return it->second.first;
        }

        std::vector<std::string> scripts;
        scripts.reserve(100);

        try
        {
            for (const auto& entry : boost::filesystem::recursive_directory_iterator(directoryPath))
            {
                if (boost::filesystem::is_regular_file(entry))
                {
                    std::string extension = entry.path().extension().string();
                    if (IsValidScriptExtension(extension))
                        scripts.emplace_back(entry.path().string());
                }
            }

            std::sort(scripts.begin(), scripts.end());

            discoverCache[directoryPath] = {scripts, lastWrite};

            EclipseLogger::GetInstance().LogDebug("[Boost] Discovered " + std::to_string(scripts.size()) + " scripts in " + directoryPath);
        }
        catch (const std::exception& e)
        {
            EclipseLogger::GetInstance().LogError("Error discovering scripts in " + directoryPath + ": " + e.what());
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

        EclipseLogger::GetInstance().LogDebug("Extracted " + std::to_string(directories.size()) + " unique directories from " + std::to_string(scriptPaths.size()) + " scripts");
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
                EclipseLogger::GetInstance().LogLuaError(chunkName, "Failed to load bytecode: " + error);
                return false;
            }

            result = lua_pcall(L, 0, LUA_MULTRET, 0);
            if (result != LUA_OK)
            {
                std::string error = lua_tostring(L, -1);
                lua_pop(L, 1);
                EclipseLogger::GetInstance().LogLuaExecutionError(chunkName, error);
                return false;
            }

            return true;
        }
        catch (const std::exception& e)
        {
            EclipseLogger::GetInstance().LogLuaError(chunkName, "Exception loading bytecode: " + std::string(e.what()));
            return false;
        }
    }

    bool ScriptLoader::LoadScript(sol::state& targetState, sol::state& compilerState, const std::string& filePath)
    {
        if (!std::filesystem::exists(filePath))
        {
            EclipseLogger::GetInstance().LogScriptNotFound(filePath, false);
            return false;
        }

        auto& cache = LuaCache::GetInstance();

        auto cachedBytecode = cache.GetBytecode(filePath);
        if (cachedBytecode.has_value())
        {
            EclipseLogger::GetInstance().LogTrace("Loading script from cache: " + filePath);
            return LoadBytecodeIntoState(targetState, *cachedBytecode, filePath);
        }

        EclipseLogger::GetInstance().LogTrace("Compiling script: " + filePath);
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
                EclipseLogger::GetInstance().LogScriptNotFound(file, false);
                if (stats) stats->failed++;
                continue;
            }

            auto& cache = LuaCache::GetInstance();
            bool wasInCache = cache.GetBytecode(file).has_value();

            if (LoadScript(targetState, compilerState, file))
            {
                loadedScripts.emplace_back(file);
                successCount++;
                EclipseLogger::GetInstance().LogTrace("Successfully loaded script: " + file);

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
                EclipseLogger::GetInstance().LogScriptLoad(file, false);
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
            EclipseLogger::GetInstance().LogDebug("Loading " + std::to_string(scripts.size()) + " scripts from directory: " + directoryPath);
            int loaded = LoadFiles(targetState, compilerState, scripts, loadedScripts, stats);
            EclipseLogger::GetInstance().LogDebug("Successfully loaded " + std::to_string(loaded) + "/" + std::to_string(scripts.size()) + " scripts from " + directoryPath);
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
                stats->duration = static_cast<uint32>(duration.count());
            }

            return !loadedScripts.empty();
        }
        catch (const std::exception& e)
        {
            EclipseLogger::GetInstance().LogError("Exception loading directory '" + directoryPath + "': " + e.what());
            return false;
        }
    }
}