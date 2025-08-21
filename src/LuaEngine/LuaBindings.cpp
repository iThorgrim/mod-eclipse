#include "LuaBindings.hpp"
#include "Log.h"

namespace Eclipse
{
    void LuaBindings::RegisterCore(sol::state& lua)
    {
        // Mark Eclipse as loaded
        lua["_ECLIPSE_VERSION"] = "1.0.0";
        lua["_ECLIPSE_LOADED"] = true;
    }
    
    void LuaBindings::RegisterLogging(sol::state& lua)
    {
        lua["print"] = [](const std::string& msg) {
            LOG_INFO("server.eclipse.lua", "{}", msg);
        };
        
        lua["log"] = [](const std::string& msg) {
            LOG_INFO("server.eclipse.lua", "{}", msg);
        };
        
        lua["error"] = [](const std::string& msg) {
            LOG_ERROR("server.eclipse.lua", "{}", msg);
        };
        
        lua["debug"] = [](const std::string& msg) {
            LOG_DEBUG("server.eclipse.lua", "{}", msg);
        };
        
        lua["warn"] = [](const std::string& msg) {
            LOG_WARN("server.eclipse.lua", "{}", msg);
        };
    }
}