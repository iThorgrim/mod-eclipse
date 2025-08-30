#include "EclipseIncludes.hpp"
#include "EclipseConfig.hpp"
#include "Config.h"

namespace Eclipse
{
    EclipseConfig& EclipseConfig::GetInstance()
    {
        static EclipseConfig instance;
        return instance;
    }

    void EclipseConfig::Initialize()
    {
        // Bool
        SetConfig(CONFIG_ECLIPSE_ENABLED, "Eclipse.Enabled", false);
        SetConfig(CONFIG_ECLIPSE_COMPATIBILITY, "Eclipse.Compatibility", true);

        // String
        SetConfig(CONFIG_ECLIPSE_SCRIPT_PATH, "Eclipse.ScriptPath", "lua_scripts");
        SetConfig(CONFIG_ECLIPSE_REQUIRE_PATH_EXTRA, "Eclipse.RequirePaths", "");
        SetConfig(CONFIG_ECLIPSE_REQUIRE_CPATH_EXTRA, "Eclipse.RequireCPaths", "");
    }

    void EclipseConfig::SetConfig(EclipseConfigBoolValues index, char const* fieldname, bool defvalue)
    {
        SetConfig(index, sConfigMgr->GetOption<bool>(fieldname, defvalue));
    }

    void EclipseConfig::SetConfig(EclipseConfigStringValues index, char const* fieldname, std::string defvalue)
    {
        SetConfig(index, sConfigMgr->GetOption<std::string>(fieldname, defvalue));
    }
}