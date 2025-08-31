#include "EclipseIncludes.hpp"
#include "EclipseConfig.hpp"

namespace Eclipse
{
    EclipseConfig& EclipseConfig::GetInstance()
    {
        static EclipseConfig instance;
        return instance;
    }
    
    EclipseConfig::EclipseConfig() : ConfigValueCache<EclipseConfigValues>(EclipseConfigValues::CONFIG_VALUE_COUNT)
    {
    }

    void EclipseConfig::Initialize(bool reload)
    {
        ConfigValueCache<EclipseConfigValues>::Initialize(reload);
    }

    void EclipseConfig::BuildConfigCache()
    {
        // Boolean configurations
        SetConfigValue<bool>(EclipseConfigValues::ENABLED, "Eclipse.Enabled", false);
        SetConfigValue<bool>(EclipseConfigValues::COMPATIBILITY, "Eclipse.Compatibility", true);
        SetConfigValue<bool>(EclipseConfigValues::PARALLEL_COMPILATION, "Eclipse.ParallelCompilation", true);

        // String configurations  
        SetConfigValue<std::string>(EclipseConfigValues::SCRIPT_PATH, "Eclipse.ScriptPath", "lua_scripts");
        SetConfigValue<std::string>(EclipseConfigValues::REQUIRE_PATH_EXTRA, "Eclipse.RequirePaths", "");
        SetConfigValue<std::string>(EclipseConfigValues::REQUIRE_CPATH_EXTRA, "Eclipse.RequireCPaths", "");
    }
}