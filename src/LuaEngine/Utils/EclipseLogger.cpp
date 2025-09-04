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

    void EclipseLogger::LogInfo(const std::string& message)
    {
        LOG_INFO(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogWarn(const std::string& message)
    {
        LOG_WARN(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogError(const std::string& message)
    {
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogDebug(const std::string& message)
    {
        LOG_DEBUG(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogTrace(const std::string& message)
    {
        LOG_TRACE(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogLuaError(const std::string& scriptPath, const std::string& error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        std::string message = "Lua Error in '" + formattedPath + "': " + error;
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogLuaCompilationError(const std::string& scriptPath, const std::string& error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        std::string message = "Compilation failed for '" + formattedPath + "': " + error;
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogLuaExecutionError(const std::string& scriptPath, const std::string& error)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        std::string message = "Execution failed for '" + formattedPath + "': " + error;
        LOG_ERROR(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogScriptLoad(const std::string& scriptPath, bool success)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        if (success)
        {
            LogTrace("Successfully loaded script: " + formattedPath);
        }
        else
        {
            LogError("Failed to load script: " + formattedPath);
        }
    }

    void EclipseLogger::LogScriptReload(const std::string& scriptPath)
    {
        std::string formattedPath = FormatScriptPath(scriptPath);
        LogDebug("Reloading script: " + formattedPath);
    }

    void EclipseLogger::LogScriptNotFound(const std::string& scriptPath, bool silent)
    {
        if (!silent)
        {
            std::string formattedPath = FormatScriptPath(scriptPath);
            LogDebug("Script not found (ignored): " + formattedPath);
        }
    }

    void EclipseLogger::LogStateInitialization(int32 mapId, bool success)
    {
        if (success)
        {
            if (mapId == -1)
            {
                LogInfo("Global compiler state (-1) initialized successfully");
            }
            else
            {
                LogInfo("Map state " + std::to_string(mapId) + " initialized successfully");
            }
        }
        else
        {
            std::string message = "Failed to initialize state " + std::to_string(mapId);
            LogError(message);
        }
    }

    void EclipseLogger::LogStateShutdown(int32 mapId)
    {
        if (mapId == -1)
        {
            LogDebug("Global compiler state (-1) shut down");
        }
        else
        {
            LogDebug("Map state " + std::to_string(mapId) + " shut down");
        }
    }

    void EclipseLogger::LogStateReload(int32 mapId)
    {
        LogInfo("Scripts reloaded for state " + std::to_string(mapId));
    }

    void EclipseLogger::LogLoadStatistics(int32 mapId, uint32 totalScripts, uint32 compiled, uint32 cached, uint32 precompiled, uint32 duration)
    {
        auto [formattedDuration, unit] = FormatDuration(duration);
        std::string message = "Loaded " + std::to_string(totalScripts) + " scripts (" + std::to_string(compiled) + " compiled, " + std::to_string(cached) + " cached, " + std::to_string(precompiled) + " pre-compiled) in " + std::to_string(formattedDuration) + unit + " for state " + std::to_string(mapId);

        LOG_INFO(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogCacheStatistics(size_t scriptCount, uint32 duration)
    {
        auto [formattedDuration, unit] = FormatDuration(duration);
        std::string message = "Cache loaded " + std::to_string(scriptCount) + " scripts in " + std::to_string(formattedDuration) + unit;

        LOG_INFO(LOG_CATEGORY, "[Eclipse]: {}", message);
    }

    void EclipseLogger::LogEngineStartup()
    {
        LogInfo("Eclipse Lua Engine starting up...");
    }

    void EclipseLogger::LogEngineShutdown()
    {
        LogInfo("Eclipse Lua Engine shutting down...");
    }

    void EclipseLogger::AddStateInitializationTime(uint32 microseconds)
    {
        totalInitializationTimeUs += microseconds;
    }

    void EclipseLogger::LogTotalInitializationTime()
    {
        auto [duration, unit] = FormatDuration(totalInitializationTimeUs);
        std::string message = "Eclipse total initialization time: " + std::to_string(duration) + unit;

        LOG_INFO(LOG_CATEGORY, "[Eclipse]: {}", message);
        totalInitializationTimeUs = 0;
    }

    void EclipseLogger::LogConfigurationApplied(const std::string& setting, const std::string& value)
    {
        LogDebug("Configuration applied: " + setting + " = " + value);
    }

    void EclipseLogger::LogPathsInitialized(size_t pathCount)
    {
        LogDebug("Lua paths initialized (" + std::to_string(pathCount) + " paths)");
    }

    std::pair<uint32, std::string> EclipseLogger::FormatDuration(uint32 microseconds)
    {
        if (microseconds >= 1000)
        {
            uint32 milliseconds = microseconds / 1000;
            return {milliseconds, "ms"};
        }
        return {microseconds, "μs"};
    }

    std::string EclipseLogger::FormatScriptPath(const std::string& fullPath)
    {
        try
        {
            std::filesystem::path path(fullPath);
            return path.filename().string();
        }
        catch (...)
        {
            return fullPath;
        }
    }
}