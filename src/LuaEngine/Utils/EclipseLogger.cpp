#include "EclipseLogger.hpp"
#include "EclipseIncludes.hpp"
#include <iostream>
#include <filesystem>

namespace Eclipse
{
    EclipseLogger& EclipseLogger::GetInstance()
    {
        static EclipseLogger instance;
        return instance;
    }

    void EclipseLogger::LogInfo(std::string_view message)
    {
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogWarn(std::string_view message)
    {
        LOG_WARN(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogError(std::string_view message)
    {
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogDebug(std::string_view message)
    {
        LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogTrace(std::string_view message)
    {
        LOG_TRACE(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogLuaError(std::string_view scriptPath, std::string_view error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: Lua Error in '{}': {}", formattedPath, error);
    }

    void EclipseLogger::LogLuaCompilationError(std::string_view scriptPath, std::string_view error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: Compilation failed for '{}': {}", formattedPath, error);
    }

    void EclipseLogger::LogLuaExecutionError(std::string_view scriptPath, std::string_view error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: Execution failed for '{}': {}", formattedPath, error);
    }

    void EclipseLogger::LogScriptLoad(std::string_view scriptPath, bool success)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        if (success)
        {
            LOG_TRACE(LOG_CATEGORY, "[Eclipse]: Successfully loaded script: {}", formattedPath);
        }
        else
        {
            LOG_ERROR(LOG_CATEGORY, "[Eclipse]: Failed to load script: {}", formattedPath);
        }
    }

    void EclipseLogger::LogScriptReload(std::string_view scriptPath)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Reloading script: {}", formattedPath);
    }

    void EclipseLogger::LogScriptNotFound(std::string_view scriptPath, bool silent)
    {
        if (!silent)
        {
            std::string formattedPath = FormatScriptPath(scriptPath);
            LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Script not found (ignored): {}", formattedPath);
        }
    }

    void EclipseLogger::LogStateInitialization(int32 mapId, bool success)
    {
        if (success)
        {
            if (mapId == -1)
            {
                LOG_INFO(LOG_CATEGORY, "[Eclipse]: Global compiler state (-1) initialized successfully");
            }
            else
            {
                LOG_INFO(LOG_CATEGORY, "[Eclipse]: Map state {} initialized successfully", mapId);
            }
        }
        else
        {
            LOG_ERROR(LOG_CATEGORY, "[Eclipse]: Failed to initialize state {}", mapId);
        }
    }

    void EclipseLogger::LogStateShutdown(int32 mapId)
    {
        if (mapId == -1)
        {
            LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Global compiler state (-1) shut down");
        }
        else
        {
            LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Map state {} shut down", mapId);
        }
    }

    void EclipseLogger::LogStateReload(int32 mapId)
    {
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Scripts reloaded for state {}", mapId);
    }

    void EclipseLogger::LogLoadStatistics(int32 mapId, uint32 totalScripts, uint32 compiled, uint32 cached, uint32 precompiled, uint32 duration)
    {
        auto [formattedDuration, unit] = FormatDuration(duration);
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Loaded {} scripts ({} compiled, {} cached, {} pre-compiled) in {}{} for state {}", 
                 totalScripts, compiled, cached, precompiled, formattedDuration, unit, mapId);
    }

    void EclipseLogger::LogCacheStatistics(size_t scriptCount, uint32 duration)
    {
        auto [formattedDuration, unit] = FormatDuration(duration);
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Cache loaded {} scripts in {}{}", scriptCount, formattedDuration, unit);
    }

    void EclipseLogger::LogEngineStartup()
    {
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Eclipse Lua Engine starting up...");
    }

    void EclipseLogger::LogEngineShutdown()
    {
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Eclipse Lua Engine shutting down...");
    }

    void EclipseLogger::AddStateInitializationTime(uint32 microseconds)
    {
        totalInitializationTimeUs += microseconds;
    }

    void EclipseLogger::LogTotalInitializationTime()
    {
        auto [duration, unit] = FormatDuration(totalInitializationTimeUs);
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: Eclipse total initialization time: {}{}", duration, unit);
        totalInitializationTimeUs = 0;
    }

    void EclipseLogger::LogConfigurationApplied(std::string_view setting, std::string_view value)
    {
        LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Configuration applied: {} = {}", setting, value);
    }

    void EclipseLogger::LogPathsInitialized(size_t pathCount)
    {
        LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: Lua paths initialized ({} paths)", pathCount);
    }

    std::pair<uint32, std::string_view> EclipseLogger::FormatDuration(uint32 microseconds)
    {
        if (microseconds >= 1000)
        {
            uint32 milliseconds = microseconds / 1000;
            return {milliseconds, "ms"};
        }
        return {microseconds, "μs"};
    }

    std::string EclipseLogger::FormatScriptPath(std::string_view fullPath)
    {
        try
        {
            std::filesystem::path path(fullPath);
            return path.filename().string();
        }
        catch (...)
        {
            return std::string(fullPath);
        }
    }
}