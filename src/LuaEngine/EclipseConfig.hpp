#ifndef ECLIPSE_CONFIG_HPP
#define ECLIPSE_CONFIG_HPP

namespace Eclipse
{
    enum EclipseConfigBoolValues
    {
        CONFIG_ECLIPSE_ENABLED,
        CONFIG_ECLIPSE_COMPATIBILITY,
        CONFIG_ECLIPSE_BOOL_COUNT
    };

    enum EclipseConfigStringValues
    {
        CONFIG_ECLIPSE_SCRIPT_PATH,
        CONFIG_ECLIPSE_REQUIRE_PATH_EXTRA,
        CONFIG_ECLIPSE_REQUIRE_CPATH_EXTRA,
        CONFIG_ECLIPSE_STRING_COUNT
    };

    class EclipseConfig
    {
    public:
        static EclipseConfig& GetInstance();

        void Initialize();

        bool GetConfig(EclipseConfigBoolValues index) const { return _configBoolValues[index]; }
        const std::string& GetConfig(EclipseConfigStringValues index) const { return _configStringValues[index]; }

        bool IsEclipseEnabled() { return GetConfig(CONFIG_ECLIPSE_ENABLED); }
        bool IsEclipseCompatiblityEnabled() { return GetConfig(CONFIG_ECLIPSE_COMPATIBILITY); }

    private:
        bool _configBoolValues[CONFIG_ECLIPSE_BOOL_COUNT];
        std::string _configStringValues[CONFIG_ECLIPSE_STRING_COUNT];

        void SetConfig(EclipseConfigBoolValues index, bool value) { _configBoolValues[index] = value; }
        void SetConfig(EclipseConfigStringValues index, std::string value) { _configStringValues[index] = value; }

        void SetConfig(EclipseConfigBoolValues index, char const* fieldname, bool defvalue);
        void SetConfig(EclipseConfigStringValues index, char const* fieldname, std::string defvalue);

        EclipseConfig() = default;
        ~EclipseConfig() = default;
        EclipseConfig(const EclipseConfig&) = delete;
        EclipseConfig& operator=(const EclipseConfig&) = delete;
    };
}

#endif // ECLIPSE_CONFIG_HPP