#include "LuaCache.hpp"
#include <filesystem>

namespace Eclipse
{
    LuaCache::LuaCache(size_t maxSize, size_t maxMemory)
        : maxSize_(maxSize), maxMemory_(maxMemory)
    {
        LOG_DEBUG("server.eclipse", "LuaCache initialized with max size: {}, max memory: {} MB", 
                  maxSize_, maxMemory_ / (1024 * 1024));
    }

    std::optional<bool> LuaCache::LoadScript(sol::state& lua, const std::string& path)
    {
        if (!std::filesystem::exists(path))
        {
            return std::nullopt; // File doesn't exist
        }

        auto it = cache_.find(path);
        if (it == cache_.end())
        {
            stats_.missCount++;
            
            // Try to compile and cache
            bool success = CompileAndCache(lua, path);
            return success;
        }

        auto& [entry, lruIt] = it->second;
        
        // Check if file was modified
        if (IsFileNewer(path, entry.lastModified))
        {
            stats_.missCount++;
            
            // File changed, recompile
            InvalidateScript(path);
            bool success = CompileAndCache(lua, path);
            return success;
        }

        // Cache hit
        stats_.hitCount++;
        entry.accessCount++;
        entry.lastAccess = std::chrono::steady_clock::now();
        UpdateLRU(path);

        if (!entry.isSuccessful)
        {
            return false; // Cached failure
        }

        if (entry.bytecode.empty())
        {
            return false; // No bytecode stored
        }

        try
        {
            // Load from bytecode cache
            auto result = lua.script(std::string(entry.bytecode.begin(), entry.bytecode.end()));
            LOG_TRACE("server.eclipse", "Loaded script from cache: {}", path);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to execute cached bytecode for '{}': {}", path, e.what());
            // Invalidate bad cache entry
            InvalidateScript(path);
            return false;
        }
    }

    void LuaCache::StoreScript(const std::string& path, bool success)
    {
        if (cache_.size() >= maxSize_)
        {
            EvictLRU();
        }

        auto timestamp = GetFileTimestamp(path);
        
        // Create entry (empty bytecode for failed compilations)
        CacheEntry entry;
        entry.lastModified = timestamp;
        entry.isSuccessful = success;
        entry.lastAccess = std::chrono::steady_clock::now();

        if (success)
        {
            // For successful compilation, we would store bytecode here
            // For now, we just mark it as successful
            LOG_TRACE("server.eclipse", "Cached successful script: {}", path);
        }
        else
        {
            LOG_TRACE("server.eclipse", "Cached failed script: {}", path);
        }

        // Add to LRU list
        lruList_.push_front(path);
        cache_[path] = std::make_pair(std::move(entry), lruList_.begin());

        stats_.totalSize = cache_.size();
        stats_.memoryUsage = CalculateMemoryUsage();

        // Check if we need to evict due to memory pressure
        EvictIfNeeded();
    }

    void LuaCache::InvalidateScript(const std::string& path)
    {
        auto it = cache_.find(path);
        if (it != cache_.end())
        {
            lruList_.erase(it->second.second);
            cache_.erase(it);
            
            stats_.totalSize = cache_.size();
            stats_.memoryUsage = CalculateMemoryUsage();
            
            LOG_TRACE("server.eclipse", "Invalidated cache for script: {}", path);
        }
    }

    void LuaCache::Clear()
    {
        cache_.clear();
        lruList_.clear();
        ResetStats();
        LOG_DEBUG("server.eclipse", "LuaCache cleared");
    }

    void LuaCache::LogStats() const
    {
        auto stats = GetStats();
        LOG_INFO("server.eclipse", "[LuaCache] Hit ratio: {:.2f}%, Size: {}/{}, Memory: {:.2f}/{:.2f} MB, Evictions: {}", 
                 stats.hitRatio() * 100.0, 
                 stats.totalSize, maxSize_,
                 stats.memoryUsage / (1024.0 * 1024.0),
                 maxMemory_ / (1024.0 * 1024.0),
                 stats.evictions);
    }

    void LuaCache::ResetStats()
    {
        stats_.hitCount = 0;
        stats_.missCount = 0;
        stats_.evictions = 0;
        stats_.totalSize = cache_.size();
        stats_.memoryUsage = CalculateMemoryUsage();
    }

    std::filesystem::file_time_type LuaCache::GetFileTimestamp(const std::string& path) const
    {
        try
        {
            return std::filesystem::last_write_time(path);
        }
        catch (const std::filesystem::filesystem_error&)
        {
            return std::filesystem::file_time_type{};
        }
    }

    bool LuaCache::IsFileNewer(const std::string& path, const std::filesystem::file_time_type& cachedTime) const
    {
        auto currentTime = GetFileTimestamp(path);
        return currentTime > cachedTime;
    }

    void LuaCache::UpdateLRU(const std::string& path)
    {
        auto it = cache_.find(path);
        if (it != cache_.end())
        {
            // Move to front of LRU list
            lruList_.erase(it->second.second);
            lruList_.push_front(path);
            it->second.second = lruList_.begin();
        }
    }

    void LuaCache::EvictLRU()
    {
        if (!lruList_.empty())
        {
            std::string lruPath = lruList_.back();
            lruList_.pop_back();
            cache_.erase(lruPath);
            
            stats_.evictions++;
            stats_.totalSize = cache_.size();
            stats_.memoryUsage = CalculateMemoryUsage();
            
            LOG_TRACE("server.eclipse", "Evicted LRU cache entry: {}", lruPath);
        }
    }

    void LuaCache::EvictIfNeeded()
    {
        // Evict by size
        while (cache_.size() > maxSize_)
        {
            EvictLRU();
        }

        // Evict by memory usage
        while (CalculateMemoryUsage() > maxMemory_ && !cache_.empty())
        {
            EvictLRU();
        }
    }

    size_t LuaCache::CalculateMemoryUsage() const
    {
        size_t totalMemory = 0;
        for (const auto& [path, entryPair] : cache_)
        {
            const auto& entry = entryPair.first;
            totalMemory += entry.bytecode.size();
            totalMemory += path.size(); // Approximate string overhead
            totalMemory += sizeof(CacheEntry);
        }
        return totalMemory;
    }

    bool LuaCache::CompileAndCache(sol::state& lua, const std::string& path)
    {
        try
        {
            auto result = lua.script_file(path);
            StoreScript(path, true);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to compile script '{}': {}", path, e.what());
            StoreScript(path, false);
            return false;
        }
    }
    
    // Legacy compatibility methods for ScriptLoader
    bool LuaCache::IsCacheValid(const std::string& path) const
    {
        auto it = cache_.find(path);
        if (it == cache_.end())
        {
            return false;
        }
        
        const auto& entry = it->second.first;
        return !IsFileNewer(path, entry.lastModified);
    }
    
    bool LuaCache::LoadFromCache(sol::state& lua, const std::string& path)
    {
        auto result = LoadScript(lua, path);
        return result.value_or(false);
    }
    
    void LuaCache::CacheBytecode(const std::string& path, std::vector<char>&& bytecode)
    {
        // For legacy compatibility, we just mark it as successful
        StoreScript(path, true);
    }
    
    // Global cache singleton for legacy compatibility
    LuaCache& GetGlobalCache()
    {
        static LuaCache globalCache;
        return globalCache;
    }
}