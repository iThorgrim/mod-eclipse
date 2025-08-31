#include "MessageManager.hpp"
#include "MapStateManager.hpp"

namespace Eclipse
{
    MessageManager& MessageManager::GetInstance()
    {
        static MessageManager instance;
        return instance;
    }

    void MessageManager::SendMessage(int32 fromStateId, int32 toStateId, const std::string& messageType, sol::object data)
    {
        StateMessage message(fromStateId, toStateId, messageType, data);
        messageQueue[toStateId].push_back(message);
        
        
        ProcessMessages(toStateId);
    }

    void MessageManager::RegisterMessageEvent(int32 stateId, const std::string& messageType, sol::function callback)
    {
        if (callback.valid())
        {
            auto& handlers = messageHandlers[stateId][messageType];
            handlers.reserve(4);
            handlers.push_back(callback);
        }
    }

    void MessageManager::ProcessMessages(int32 stateId)
    {
        auto queueIt = messageQueue.find(stateId);
        if (queueIt == messageQueue.end() || queueIt->second.empty())
        {
            return;
        }
        
        auto& messages = queueIt->second;
        
        for (const auto& message : messages)
        {
            DeliverMessage(message);
        }
        
        messages.clear();
        // Note: shrink_to_fit() removed - too expensive for frequent operations
    }

    void MessageManager::DeliverMessage(const StateMessage& message)
    {
        auto stateIt = messageHandlers.find(message.toStateId);
        if (stateIt == messageHandlers.end())
        {
            return;
        }
        
        auto typeIt = stateIt->second.find(message.messageType);
        if (typeIt == stateIt->second.end())
        {
            return;
        }
        
        for (auto& handler : typeIt->second)
        {
            try
            {
                if (handler.valid())
                {
                    handler(message.fromStateId, message.data);
                }
            }
            catch (const std::exception& e)
            {
                LOG_ERROR("server.eclipse", "[Eclipse]: Error in message handler: {}", e.what());
            }
        }
        
    }

    void MessageManager::ClearStateHandlers(int32 stateId)
    {
        messageHandlers.erase(stateId);
        messageQueue.erase(stateId);
    }
}