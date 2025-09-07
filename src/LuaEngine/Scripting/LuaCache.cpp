#include "LuaCache.hpp"
#include <boost/filesystem.hpp>

namespace Eclipse
{
    LuaCache& LuaCache::GetInstance()
    {
        static LuaCache instance;
        return instance;
    }

    std::chrono::system_clock::time_point LuaCache::GetFileWriteTime(const std::string& filePath) const
    {
        boost::system::error_code ec;
        auto writeTime = boost::filesystem::last_write_time(filePath, ec);

        if (ec)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to get write time for file {}: {}", filePath, ec.message());
            return std::chrono::system_clock::time_point{};
        }

        // Convert boost time to chrono time_point
        return std::chrono::system_clock::from_time_t(writeTime);
    }

    std::optional<std::vector<char>> LuaCache::GetBytecode(const std::string& filePath)
    {
        auto it = cache_.find(filePath);
        if (it == cache_.end())
        {
            return std::nullopt;
        }

        auto& entry = it->second;

        if (IsScriptModified(filePath))
        {
            InvalidateScript(filePath);
            return std::nullopt;
        }

        if (!entry.compilationSuccess)
        {
            return std::nullopt;
        }

        return entry.bytecode;
    }

    void LuaCache::StoreBytecode(const std::string& filePath, std::vector<char>&& bytecode, bool success)
    {
        auto writeTime = GetFileWriteTime(filePath);

        CacheEntry entry(std::move(bytecode), writeTime);
        entry.compilationSuccess = success;

        cache_.emplace(filePath, std::move(entry));
        timestampCache_[filePath] = writeTime;

        LOG_DEBUG("server.eclipse", "[Eclipse]: Cached script: {} (success: {})", filePath, success);
    }

    void LuaCache::InvalidateScript(const std::string& filePath)
    {
        cache_.erase(filePath);
        timestampCache_.erase(filePath);
        LOG_TRACE("server.eclipse", "[Eclipse]: Invalidated cache for script: {}", filePath);
    }

    void LuaCache::InvalidateAllScripts()
    {
        cache_.clear();
        timestampCache_.clear();
    }

    bool LuaCache::IsScriptModified(const std::string& filePath) const
    {
        auto it = cache_.find(filePath);
        if (it == cache_.end()) {
            return true;
        }

        auto timestampIt = timestampCache_.find(filePath);
        std::chrono::system_clock::time_point currentWriteTime;

        if (timestampIt != timestampCache_.end()) {
            currentWriteTime = timestampIt->second;
        } else {
            currentWriteTime = GetFileWriteTime(filePath);
            timestampCache_[filePath] = currentWriteTime;
        }

        return (currentWriteTime == std::chrono::system_clock::time_point{} || 
                currentWriteTime != it->second.lastWriteTime);
    }

    std::vector<std::string> LuaCache::GetModifiedScripts() const
    {
        std::vector<std::string> modifiedScripts;
        modifiedScripts.reserve(cache_.size());

        for (const auto& [filePath, entry] : cache_) {
            if (IsScriptModified(filePath)) {
                modifiedScripts.emplace_back(filePath);
            }
        }

        return modifiedScripts;
    }

    std::vector<std::string> LuaCache::GetAllCachedScripts() const
    {
        std::vector<std::string> scripts;
        scripts.reserve(cache_.size());

        for (const auto& [filePath, entry] : cache_) {
            if (entry.compilationSuccess) {
                scripts.emplace_back(filePath);
            }
        }

        return scripts;
    }

    void LuaCache::Clear()
    {
        InvalidateAllScripts();
    }
}