#pragma once

#include <string>
#include <memory>
#include "Common.h"

namespace Eclipse
{
    class EclipseLogger
    {
    public:
        static EclipseLogger& GetInstance();

        // Core logging methods
        void LogInfo(const std::string& message);
        void LogWarn(const std::string& message);
        void LogError(const std::string& message);
        void LogDebug(const std::string& message);
        void LogTrace(const std::string& message);

        // Specialized logging for Lua errors
        void LogLuaError(const std::string& scriptPath, const std::string& error);
        void LogLuaCompilationError(const std::string& scriptPath, const std::string& error);
        void LogLuaExecutionError(const std::string& scriptPath, const std::string& error);

        // Script loading logs
        void LogScriptLoad(const std::string& scriptPath, bool success);
        void LogScriptReload(const std::string& scriptPath);
        void LogScriptNotFound(const std::string& scriptPath, bool silent = false);

        // State management logs
        void LogStateInitialization(int32 mapId, bool success);
        void LogStateShutdown(int32 mapId);
        void LogStateReload(int32 mapId);

        // Performance and statistics logs
        void LogLoadStatistics(int32 mapId, uint32 totalScripts, uint32 compiled, uint32 cached, uint32 precompiled, uint32 duration);
        void LogCacheStatistics(size_t scriptCount, uint32 duration);

        // Engine lifecycle logs
        void LogEngineStartup();
        void LogEngineShutdown();

        // State timing accumulation
        void AddStateInitializationTime(uint32 microseconds);
        void LogTotalInitializationTime();

        // Configuration and setup logs
        void LogConfigurationApplied(const std::string& setting, const std::string& value);
        void LogPathsInitialized(size_t pathCount);

    private:
        EclipseLogger() = default;
        ~EclipseLogger() = default;
        EclipseLogger(const EclipseLogger&) = delete;
        EclipseLogger& operator=(const EclipseLogger&) = delete;

        // Internal helper methods
        std::pair<uint32, std::string> FormatDuration(uint32 microseconds);
        std::string FormatScriptPath(const std::string& fullPath);

        // State timing accumulation
        uint32 totalInitializationTimeUs = 0;

        static constexpr const char* LOG_CATEGORY = "server.eclipse";
    };

    // Convenience macros for easier usage throughout the codebase
    #define ECLIPSE_LOG_INFO(msg) Eclipse::EclipseLogger::GetInstance().LogInfo(msg)
    #define ECLIPSE_LOG_WARN(msg) Eclipse::EclipseLogger::GetInstance().LogWarn(msg)
    #define ECLIPSE_LOG_ERROR(msg) Eclipse::EclipseLogger::GetInstance().LogError(msg)
    #define ECLIPSE_LOG_DEBUG(msg) Eclipse::EclipseLogger::GetInstance().LogDebug(msg)
    #define ECLIPSE_LOG_TRACE(msg) Eclipse::EclipseLogger::GetInstance().LogTrace(msg)

    #define ECLIPSE_LOG_LUA_ERROR(script, error) Eclipse::EclipseLogger::GetInstance().LogLuaError(script, error)
    #define ECLIPSE_LOG_SCRIPT_LOAD(script, success) Eclipse::EclipseLogger::GetInstance().LogScriptLoad(script, success)
    #define ECLIPSE_LOG_STATE_INIT(mapId, success) Eclipse::EclipseLogger::GetInstance().LogStateInitialization(mapId, success)
}