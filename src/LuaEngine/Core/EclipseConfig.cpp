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
        SetConfigValue(EclipseConfigValues::ENABLED, "Eclipse.Enabled", false, Reloadable::Yes);
        SetConfigValue(EclipseConfigValues::COMPATIBILITY, "Eclipse.Compatibility", true, Reloadable::No);

        // String configurations  
        SetConfigValue(EclipseConfigValues::SCRIPT_PATH, "Eclipse.ScriptPath", std::string("lua_scripts"), Reloadable::No);
        SetConfigValue(EclipseConfigValues::REQUIRE_PATH_EXTRA, "Eclipse.RequirePaths", std::string(""), Reloadable::Yes);
        SetConfigValue(EclipseConfigValues::REQUIRE_CPATH_EXTRA, "Eclipse.RequireCPaths", std::string(""), Reloadable::Yes);
    }
}