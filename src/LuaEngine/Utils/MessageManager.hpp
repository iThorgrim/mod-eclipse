#ifndef ECLIPSE_MESSAGE_MANAGER_HPP
#define ECLIPSE_MESSAGE_MANAGER_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

namespace Eclipse
{
    struct StateMessage
    {
        int32 fromStateId;
        int32 toStateId;
        std::string messageType;
        sol::object data;

        StateMessage(int32 from, int32 to, const std::string& type, sol::object obj)
            : fromStateId(from), toStateId(to), messageType(type), data(obj) {}
    };

    class MessageManager
    {
    public:
        static MessageManager& GetInstance();

        void SendMessage(int32 fromStateId, int32 toStateId, const std::string& messageType, sol::object data);
        void BroadcastMessage(int32 fromStateId, const std::string& messageType, sol::object data);
        void RegisterMessageEvent(int32 stateId, const std::string& messageType, sol::function callback);
        void ProcessMessages(int32 stateId);
        void ClearStateHandlers(int32 stateId);

    private:
        MessageManager() = default;
        ~MessageManager() = default;
        MessageManager(const MessageManager&) = delete;
        MessageManager& operator=(const MessageManager&) = delete;

        // Message queue: stateId -> vector of messages
        std::unordered_map<int32, std::vector<StateMessage>> messageQueue;

        // Event handlers: stateId -> messageType -> vector of callbacks
        std::unordered_map<int32, std::unordered_map<std::string, std::vector<sol::function>>> messageHandlers;

        void DeliverMessage(const StateMessage& message);
    };
}

#endif // ECLIPSE_MESSAGE_MANAGER_HPP