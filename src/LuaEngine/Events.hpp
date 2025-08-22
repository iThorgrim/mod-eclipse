#ifndef ECLIPSE_EVENTS_HPP
#define ECLIPSE_EVENTS_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    enum PlayerEvents : uint32
    {
        PLAYER_EVENT_ON_LOGIN = 1,
        PLAYER_EVENT_ON_LOGOUT = 2,
        PLAYER_EVENT_ON_LOOT_ITEM = 3,
        PLAYER_EVENT_ON_LEVEL_CHANGED = 4,
        PLAYER_EVENT_ON_KILL_PLAYER = 5,
    };
}

#endif // ECLIPSE_EVENTS_HPP