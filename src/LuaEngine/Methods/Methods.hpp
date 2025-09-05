#ifndef ECLIPSE_METHODS_HPP
#define ECLIPSE_METHODS_HPP

#include "WorldObjectMethods.hpp"
#include "UnitMethods.hpp"
#include "PlayerMethods.hpp"
#include "GlobalMethods.hpp"
#include "ObjectGuidMethods.hpp"

namespace Eclipse
{
    namespace Methods
    {
        template<typename T>
        void RegisterWorldObjectMethods(sol::usertype<T>& type)
        {
            WorldObjectMethods::RegisterWorldObjectMethods(type);
        }

        template<typename T>
        void RegisterUnitMethods(sol::usertype<T>& type)
        {
            WorldObjectMethods::RegisterWorldObjectMethods(type);
            UnitMethods::RegisterUnitMethods(type);
        }

        template<typename T>
        void RegisterPlayerMethods(sol::usertype<T>& type)
        {
            WorldObjectMethods::RegisterWorldObjectMethods(type);
            UnitMethods::RegisterUnitMethods(type);
            PlayerMethods::RegisterPlayerMethods(type);
        }

        void RegisterAll(sol::state& lua)
        {
            auto player_type = lua.new_usertype<Player>("Player");
            RegisterPlayerMethods(player_type);

            auto unit_type = lua.new_usertype<Unit>("Unit");
            RegisterUnitMethods(unit_type);

            auto creature_type = lua.new_usertype<Creature>("Creature");
            RegisterUnitMethods(creature_type);

            auto objectguid_type = lua.new_usertype<ObjectGuid>("ObjectGuid");
            ObjectGuidMethods::RegisterObjectGuidMethods(objectguid_type);

            Eclipse::RegisterEventKeysToLua(lua);
        }
    }
}

#endif // ECLIPSE_METHODS_HPP