#include "LuaCache.hpp"
#include <fstream>

namespace Eclipse
{
    LuaCache& GetGlobalCache()
    {
        static LuaCache globalCache;
        return globalCache;
    }
    
    bool LuaCache::IsCacheValid(const std::string& path) const
    {
        auto it = cache.find(path);
        if (it == cache.end()) return false;
            
        try {
            auto currentTime = GetFileTimestamp(path);
            return it->second.lastModified >= currentTime;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    bool LuaCache::LoadFromCache(sol::state& lua, const std::string& path)
    {
        auto it = cache.find(path);
        if (it == cache.end()) return false;
            
        try {
            const auto& bytecode = it->second.bytecode;
            int result = luaL_loadbuffer(lua.lua_state(), bytecode.data(), bytecode.size(), path.c_str());
            
            if (result == LUA_OK) {
                lua_pcall(lua.lua_state(), 0, 0, 0);
                return true;
            }
        } catch (const std::exception&) {
            cache.erase(it);
        }
        
        return false;
    }
    
    void LuaCache::CacheBytecode(const std::string& path, std::vector<char>&& bytecode)
    {
        try {
            auto timestamp = GetFileTimestamp(path);
            cache.emplace(path, BytecodeEntry{std::move(bytecode), timestamp});
        } catch (const std::exception& e) {
            LOG_ERROR("server.eclipse", "Failed to cache bytecode for {}: {}", path, e.what());
        }
    }
    
    std::filesystem::file_time_type LuaCache::GetFileTimestamp(const std::string& path) const
    {
        if (!std::filesystem::exists(path))
            throw std::runtime_error("File does not exist: " + path);
        return std::filesystem::last_write_time(path);
    }
    
    void LuaCache::InvalidateCache(const std::string& path)
    {
        cache.erase(path);
    }
    
    void LuaCache::Clear()
    {
        cache.clear();
        LOG_INFO("server.eclipse", "[Eclipse]: Bytecode cache cleared");
    }
    
    size_t LuaCache::GetCacheSize() const
    {
        return cache.size();
    }
    
    void LuaCache::LogCacheStats() const
    {
        size_t totalSize = 0;
        for (const auto& [path, entry] : cache)
            totalSize += entry.bytecode.size();
        
        LOG_INFO("server.eclipse", "[Eclipse]: Cache stats: {} entries, {} bytes total", 
                cache.size(), totalSize);
    }
}