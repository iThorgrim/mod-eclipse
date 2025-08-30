#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <list>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>
#include <optional>

namespace Eclipse
{
    struct CacheEntry
    {
        std::vector<char> bytecode;
        std::filesystem::file_time_type lastModified;
        size_t accessCount = 0;
        std::chrono::steady_clock::time_point lastAccess;
        bool isSuccessful = true; // Track compilation success/failure
        
        CacheEntry() = default;
        CacheEntry(std::vector<char>&& data, const std::filesystem::file_time_type& time)
            : bytecode(std::move(data)), lastModified(time), lastAccess(std::chrono::steady_clock::now()) {}
    };

    class LuaCache
    {
    public:
        static constexpr size_t DEFAULT_MAX_SIZE = 1000;       // Max number of cached scripts
        static constexpr size_t DEFAULT_MAX_MEMORY = 50 * 1024 * 1024; // 50MB max memory
        
        LuaCache(size_t maxSize = DEFAULT_MAX_SIZE, size_t maxMemory = DEFAULT_MAX_MEMORY);
        ~LuaCache() = default;
        
        // Core cache operations with better interface
        std::optional<bool> LoadScript(sol::state& lua, const std::string& path);
        void StoreScript(const std::string& path, bool success);
        void InvalidateScript(const std::string& path);
        void Clear();
        
        // Legacy compatibility methods for ScriptLoader
        bool IsCacheValid(const std::string& path) const;
        bool LoadFromCache(sol::state& lua, const std::string& path);
        void InvalidateCache(const std::string& path) { InvalidateScript(path); }
        void CacheBytecode(const std::string& path, std::vector<char>&& bytecode);
        
        // Cache configuration
        void SetMaxSize(size_t maxSize) { maxSize_ = maxSize; EvictIfNeeded(); }
        void SetMaxMemory(size_t maxMemory) { maxMemory_ = maxMemory; EvictIfNeeded(); }
        
        // Cache statistics
        struct CacheStats {
            size_t hitCount = 0;
            size_t missCount = 0;
            size_t evictions = 0;
            size_t totalSize = 0;
            size_t memoryUsage = 0;
            double hitRatio() const { 
                return hitCount + missCount > 0 ? static_cast<double>(hitCount) / (hitCount + missCount) : 0.0; 
            }
        };
        
        CacheStats GetStats() const { return stats_; }
        void LogStats() const;
        void ResetStats();
        
    private:
        // LRU implementation using list + unordered_map
        using LRUList = std::list<std::string>;
        using CacheMap = std::unordered_map<std::string, std::pair<CacheEntry, LRUList::iterator>>;
        
        CacheMap cache_;
        LRUList lruList_;
        
        // Configuration
        size_t maxSize_;
        size_t maxMemory_;
        mutable CacheStats stats_;
        
        // Helper methods
        std::filesystem::file_time_type GetFileTimestamp(const std::string& path) const;
        bool IsFileNewer(const std::string& path, const std::filesystem::file_time_type& cachedTime) const;
        void UpdateLRU(const std::string& path);
        void EvictLRU();
        void EvictIfNeeded();
        size_t CalculateMemoryUsage() const;
        bool CompileAndCache(sol::state& lua, const std::string& path);
    };
    
    // Configuration helper
    struct CacheConfig {
        size_t maxSize = LuaCache::DEFAULT_MAX_SIZE;
        size_t maxMemory = LuaCache::DEFAULT_MAX_MEMORY;
        bool enableStats = true;
    };
    
    // Global cache singleton for legacy compatibility
    LuaCache& GetGlobalCache();
}

#endif // ECLIPSE_LUA_CACHE_HPP