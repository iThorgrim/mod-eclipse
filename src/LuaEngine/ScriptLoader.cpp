#include "ScriptLoader.hpp"
#include "LuaCache.hpp"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

namespace Eclipse
{
    bool ScriptLoader::LoadFile(sol::state& lua, const std::string& filePath)
    {
        auto& cache = GetGlobalCache();
        
        // Try cache first
        if (cache.IsCacheValid(filePath))
        {
            if (cache.LoadFromCache(lua, filePath))
            {
                return true;
            }
            // Cache failed, invalidate and fallback to file
            cache.InvalidateCache(filePath);
        }
        
        // Load from file and compile to bytecode
        try
        {
            std::filesystem::path path(filePath);
            auto ext = path.extension().string();
            std::string source;
            
            // Handle different file types
            if (ext == ".moon")
            {
                // MoonScript: generate Lua code to load the MoonScript file
                source = "return require('moonscript').loadfile([[" + filePath + "]])";
            }
            else if (ext == ".out")
            {
                // Pre-compiled bytecode file
                return LoadPrecompiledFile(lua, filePath);
            }
            else
            {
                // Regular .lua or .ext files
                source = ReadFileContent(filePath);
            }
            
            auto bytecode = CompileToBytecode(lua, source);
            
            if (!bytecode.empty())
            {
                // Load from the fresh bytecode first (before moving)
                int result = luaL_loadbuffer(lua.lua_state(), bytecode.data(), bytecode.size(), filePath.c_str());
                
                // Cache the bytecode (move to avoid copy)
                cache.CacheBytecode(filePath, std::move(bytecode));
                if (result == LUA_OK)
                {
                    lua_pcall(lua.lua_state(), 0, 0, 0);
                    return true;
                }
            }
            
            // Fallback to direct file loading
            if (ext == ".moon")
            {
                lua.script(source); // Execute the moonscript loader
            }
            else
            {
                lua.script_file(filePath);
            }
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
        if (!std::filesystem::exists(directoryPath))
        {
            return false;
        }

        auto scriptFiles = ScanDirectory(directoryPath);
        if (scriptFiles.empty())
        {
            return true;
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        auto& cache = GetGlobalCache();
        
        int loadedCount = 0;
        int cachedCount = 0;
        int compiledCount = 0;
        int precompiledCount = 0;
        
        for (const auto& filePath : scriptFiles)
        {
            bool wasFromCache = cache.IsCacheValid(filePath);
            std::filesystem::path path(filePath);
            auto ext = path.extension().string();
            
            if (LoadFile(lua, filePath))
            {
                loadedScripts.push_back(filePath);
                loadedCount++;
                
                if (wasFromCache)
                    cachedCount++;
                else if (ext == ".out")
                    precompiledCount++;
                else
                    compiledCount++;
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // Store statistics for caller
        lua["_eclipse_stats"] = lua.create_table_with(
            "loaded", loadedCount,
            "cached", cachedCount,
            "compiled", compiledCount,
            "precompiled", precompiledCount,
            "duration_ms", duration.count()
        );
        
        return loadedCount > 0;
    }
    
    std::vector<std::string> ScriptLoader::ScanDirectory(const std::string& path)
    {
        std::vector<std::string> files;
        
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    auto ext = entry.path().extension().string();
                    if (ext == ".ext" || ext == ".lua" || ext == ".moon" || ext == ".out")
                        files.push_back(entry.path().string());
                }
            }
            // Sort by extension priority: .ext -> .lua -> .moon -> .out, then alphabetically
            std::sort(files.begin(), files.end(), [](const std::string& a, const std::string& b) {
                auto getExtPriority = [](const std::string& path) -> int {
                    std::filesystem::path p(path);
                    auto ext = p.extension().string();
                    if (ext == ".ext") return 0;
                    if (ext == ".lua") return 1;
                    if (ext == ".moon") return 2;
                    if (ext == ".out") return 3;
                    return 4;
                };
                
                int priorityA = getExtPriority(a);
                int priorityB = getExtPriority(b);
                
                if (priorityA != priorityB)
                    return priorityA < priorityB;
                    
                return a < b; // Alphabetical within same priority
            });
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to scan directory '{}': {}", path, e.what());
        }
        
        return files;
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
            
            // Writer function for lua_dump
            auto writer = [](lua_State*, const void* p, size_t sz, void* ud) -> int {
                auto* vec = static_cast<std::vector<char>*>(ud);
                const char* data = static_cast<const char*>(p);
                vec->insert(vec->end(), data, data + sz);
                return 0;
            };
            
            lua_dump(L, writer, &bytecode, 0);
            lua_pop(L, 1); // Remove function from stack
        }
        catch (const std::exception&)
        {
            bytecode.clear();
        }
        
        return bytecode;
    }
    
    std::string ScriptLoader::ReadFileContent(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file: " + path);
        }
        
        // Get file size and reserve string capacity
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::string content;
        content.reserve(size);
        content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        
        return content;
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
            
            std::vector<char> bytecode((std::istreambuf_iterator<char>(file)), 
                                     std::istreambuf_iterator<char>());
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
}