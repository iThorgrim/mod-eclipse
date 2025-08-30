#include "ScriptLoader.hpp"
#include "LuaCache.hpp"

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <set>

namespace Eclipse
{
    bool ScriptLoader::IsValidScriptExtension(const std::string& ext)
    {
        return ext == ".ext" || ext == ".lua" || ext == ".moon" || ext == ".out";
    }

    bool ScriptLoader::LoadFile(sol::state& lua, const std::string& filePath)
    {
        try
        {
            auto& cache = GetGlobalCache();
            
            if (cache.IsCacheValid(filePath))
            {
                if (cache.LoadFromCache(lua, filePath))
                {
                    return true;
                }
                cache.InvalidateCache(filePath);
            }
            
            std::filesystem::path path(filePath);
            auto ext = path.extension().string();
            std::string source;
            
            if (ext == ".moon")
                source = "return require('moonscript').loadfile([[" + filePath + "]])";
            else if (ext == ".out")
                return LoadPrecompiledFile(lua, filePath);
            else
                source = ReadFileContent(filePath);
            
            auto bytecode = CompileToBytecode(lua, source);
            
            if (!bytecode.empty())
            {
                int result = luaL_loadbuffer(lua.lua_state(), bytecode.data(), bytecode.size(), filePath.c_str());
                cache.CacheBytecode(filePath, std::move(bytecode));
                
                if (result == LUA_OK)
                {
                    lua_pcall(lua.lua_state(), 0, 0, 0);
                    return true;
                }
            }
            
            if (ext == ".moon")
                lua.script(source);
            else
                lua.script_file(filePath);
            
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to load script '{}': {}", filePath, e.what());
            return false;
        }
    }

    bool ScriptLoader::LoadDirectory(sol::state& lua, const std::string& directoryPath, std::vector<std::string>& loadedScripts)
    {
        try
        {
            if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath))
            {
                LOG_ERROR("server.eclipse", "Invalid directory path: {}", directoryPath);
                return false;
            }

            auto& pathManager = LuaPathManager::GetInstance();
            pathManager.AddSearchPath(directoryPath);
            pathManager.ApplyPaths(lua);

            std::vector<std::string> files;
            
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
            {
                if (entry.is_regular_file())
                {
                    auto ext = entry.path().extension().string();
                    if (IsValidScriptExtension(ext))
                        files.push_back(entry.path().string());
                }
                else if (entry.is_directory())
                {
                    LoadDirectory(lua, entry.path().string(), loadedScripts);
                }
            }

            std::sort(files.begin(), files.end(), [](const std::string& a, const std::string& b) {
                auto getExtPriority = [](const std::string& path) -> int {
                    std::filesystem::path p(path);
                    auto ext = p.extension().string();
                    if (ext == ".ext") return 0;
                    if (ext == ".lua") return 1;
                    if (ext == ".out") return 2;
                    if (ext == ".moon") return 3;
                    return 4;
                };
                
                int priorityA = getExtPriority(a);
                int priorityB = getExtPriority(b);
                
                return priorityA != priorityB ? priorityA < priorityB : a < b;
            });

            return LoadFiles(lua, files, loadedScripts);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Error in LoadDirectory for '{}': {}", directoryPath, e.what());
            return false;
        }
    }

    std::vector<char> ScriptLoader::CompileToBytecode(sol::state& lua, const std::string& source)
    {
        std::vector<char> bytecode;
        
        try
        {
            sol::load_result loaded = lua.load(source);
            if (!loaded.valid())
                return bytecode;
            
            lua_State* L = lua.lua_state();
            
            auto writer = [](lua_State*, const void* p, size_t sz, void* ud) -> int {
                auto* vec = static_cast<std::vector<char>*>(ud);
                const char* data = static_cast<const char*>(p);
                vec->insert(vec->end(), data, data + sz);
                return 0;
            };
            
            lua_dump(L, writer, &bytecode, 0);
            lua_pop(L, 1);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to compile bytecode: {}", e.what());
            bytecode.clear();
        }
        
        return bytecode;
    }

    std::string ScriptLoader::ReadFileContent(const std::string& path)
    {
        try
        {
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open())
            {
                throw std::runtime_error("Cannot open file: " + path);
            }
            
            file.seekg(0, std::ios::end);
            auto size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::string content;
            content.reserve(size);
            content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            
            return content;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to read file '{}': {}", path, e.what());
            throw;
        }
    }

    bool ScriptLoader::LoadPrecompiledFile(sol::state& lua, const std::string& filePath)
    {
        try
        {
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open())
            {
                LOG_ERROR("server.eclipse", "Cannot open precompiled file: {}", filePath);
                return false;
            }
            
            std::vector<char> bytecode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            
            int result = luaL_loadbuffer(lua.lua_state(), bytecode.data(), bytecode.size(), filePath.c_str());
            if (result == LUA_OK)
            {
                lua_pcall(lua.lua_state(), 0, 0, 0);
                return true;
            }
            
            return false;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to load precompiled file '{}': {}", filePath, e.what());
            return false;
        }
    }

    int ScriptLoader::LoadFiles(sol::state& lua, const std::vector<std::string>& files, std::vector<std::string>& loadedScripts)
    {
        if (files.empty())
            return 0;

        int loadedCount = 0;
        
        for (const auto& filePath : files)
        {
            try
            {
                if (LoadFile(lua, filePath))
                {
                    loadedScripts.push_back(filePath);
                    loadedCount++;
                }
            }
            catch (const std::exception& e)
            {
                LOG_ERROR("server.eclipse", "Failed to load file '{}': {}", filePath, e.what());
            }
        }

        return loadedCount;
    }
}