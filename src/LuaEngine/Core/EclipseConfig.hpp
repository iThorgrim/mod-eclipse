#ifndef ECLIPSE_CONFIG_HPP
#define ECLIPSE_CONFIG_HPP

#include "ConfigValueCache.h"

namespace Eclipse
{
    enum class EclipseConfigValues : uint32
    {
        // Boolean configurations
        ENABLED = 0,
        COMPATIBILITY,
        PARALLEL_COMPILATION,
        // String configurations  
        SCRIPT_PATH,
        REQUIRE_PATH_EXTRA,
        REQUIRE_CPATH_EXTRA,

        
        CONFIG_VALUE_COUNT
    };

    class EclipseConfig final : public ConfigValueCache<EclipseConfigValues>
    {
    public:
        static EclipseConfig& GetInstance();

        void Initialize(bool reload = false);

        bool IsEclipseEnabled() const { return GetConfigValue<bool>(EclipseConfigValues::ENABLED); }
        bool IsCompatibilityEnabled() const { return GetConfigValue<bool>(EclipseConfigValues::COMPATIBILITY); }
        bool IsParallelCompilationEnabled() const { return GetConfigValue<bool>(EclipseConfigValues::PARALLEL_COMPILATION); }
        
        std::string_view GetScriptPath() const { return GetConfigValue(EclipseConfigValues::SCRIPT_PATH); }
        std::string_view GetRequirePathExtra() const { return GetConfigValue(EclipseConfigValues::REQUIRE_PATH_EXTRA); }
        std::string_view GetRequireCPathExtra() const { return GetConfigValue(EclipseConfigValues::REQUIRE_CPATH_EXTRA); }

    protected:
        void BuildConfigCache() override;

    private:
        EclipseConfig();
        ~EclipseConfig() = default;
        EclipseConfig(const EclipseConfig&) = delete;
        EclipseConfig& operator=(const EclipseConfig&) = delete;
    };
}

#endif // ECLIPSE_CONFIG_HPP