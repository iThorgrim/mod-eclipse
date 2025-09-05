#ifndef ECLIPSE_MESSAGE_MANAGER_HPP
#define ECLIPSE_MESSAGE_MANAGER_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <shared_mutex>

namespace Eclipse
{
    struct StateMessage
    {
        int32 fromStateId;
        int32 toStateId;
        std::string messageType;
        sol::object data;

        StateMessage(int32 from, int32 to, std::string type, sol::object obj)
            : fromStateId(from), toStateId(to), messageType(std::move(type)), data(std::move(obj)) {}
    };

    class MessageManager
    {
    public:
        static MessageManager& GetInstance();

        void SendMessage(int32 fromStateId, int32 toStateId, std::string messageType, sol::object data);
        void BroadcastMessage(int32 fromStateId, std::string messageType, sol::object data);
        void RegisterMessageEvent(int32 stateId, std::string messageType, sol::function callback);
        void ProcessMessages(int32 stateId);
        void ClearStateHandlers(int32 stateId);

    private:
        MessageManager() = default;
        ~MessageManager() = default;
        MessageManager(const MessageManager&) = delete;
        MessageManager& operator=(const MessageManager&) = delete;

        // Message queue: stateId -> vector of messages
        std::unordered_map<int32, std::vector<StateMessage>> messageQueue;
        mutable std::shared_mutex messageQueueMutex;

        // Event handlers: stateId -> messageType -> vector of callbacks
        std::unordered_map<int32, std::unordered_map<std::string, std::vector<sol::function>>> messageHandlers;
        mutable std::shared_mutex messageHandlersMutex;

        void DeliverMessage(const StateMessage& message);
    };
}

#endif // ECLIPSE_MESSAGE_MANAGER_HPP