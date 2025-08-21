#include "MessageManager.hpp"
#include "MapStateManager.hpp"
#include "Log.h"

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
        
        LOG_DEBUG("server.eclipse", "Message '{}' queued from state {} to state {}", 
                 messageType, fromStateId, toStateId);
        
        ProcessMessages(toStateId);
    }

    void MessageManager::BroadcastMessage(int32 fromStateId, const std::string& messageType, sol::object data)
    {
        auto& stateManager = MapStateManager::GetInstance();
        
        if (fromStateId != -1)
        {
            SendMessage(fromStateId, -1, messageType, data);
        }
        
        LOG_DEBUG("server.eclipse", "Message '{}' broadcasted from state {}", messageType, fromStateId);
    }

    void MessageManager::RegisterMessageEvent(int32 stateId, const std::string& messageType, sol::function callback)
    {
        if (callback.valid())
        {
            messageHandlers[stateId][messageType].push_back(callback);
            LOG_DEBUG("server.eclipse", "Message handler registered for '{}' on state {}", messageType, stateId);
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
                LOG_ERROR("server.eclipse", "Error in message handler: {}", e.what());
            }
        }
        
        LOG_DEBUG("server.eclipse", "Message '{}' delivered to state {} from state {}", 
                 message.messageType, message.toStateId, message.fromStateId);
    }

    void MessageManager::ClearStateHandlers(int32 stateId)
    {
        messageHandlers.erase(stateId);
        messageQueue.erase(stateId);
        LOG_DEBUG("server.eclipse", "Cleared all message handlers for state {}", stateId);
    }

    void MessageManager::RegisterBindings(sol::state& lua, int32 stateId)
    {
        // Send message to specific state
        lua["SendStateMessage"] = [this, stateId](int32 toStateId, const std::string& messageType, sol::object data) {
            this->SendMessage(stateId, toStateId, messageType, data);
        };
        
        // Broadcast message to all states
        lua["BroadcastMessage"] = [this, stateId](const std::string& messageType, sol::object data) {
            this->BroadcastMessage(stateId, messageType, data);
        };
    }
}