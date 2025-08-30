#ifndef ECLIPSE_PLAYER_METHODS_HPP
#define ECLIPSE_PLAYER_METHODS_HPP

#include "EclipseIncludes.hpp"
#include "UnitMethods.hpp"

namespace Eclipse
{
    namespace PlayerMethods
    {
        void SendMessage(Player* player, const std::string& message)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("{}", message);
        }
        
        void SendNotification(Player* player, const std::string& message)
        {
            ChatHandler(player->GetSession()).SendNotification("{}", message);
        }
        
        uint8 GetClass(Player* player)
        {
            return player->getClass();
        }
        
        uint8 GetRace(Player* player)
        {                
            return player->getRace();
        }
        
        uint8 GetGender(Player* player)
        {                
            return player->getGender();
        }

        void GiveLevel(Player* player, uint8 level)
        {
            player->GiveLevel(level);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);
        }
        
        // ========== LUA REGISTRATION ==========
        template<typename T>
        void RegisterPlayerMethods(sol::usertype<T>& type)
        {
            type["GiveLevel"] = &GiveLevel;
            type["GetClass"] = &GetClass;
            type["GetRace"] = &GetRace;
            type["GetGender"] = &GetGender;
            type["SendMessage"] = &SendMessage;
            type["SendNotification"] = &SendNotification;
        }
    }
}

#endif // ECLIPSE_PLAYER_METHODS_HPP