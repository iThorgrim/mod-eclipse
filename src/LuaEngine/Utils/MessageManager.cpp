#include "MessageManager.hpp"
#include "MapStateManager.hpp"

namespace Eclipse
{
    MessageManager& MessageManager::GetInstance()
    {
        static MessageManager instance;
        return instance;
    }

    void MessageManager::SendMessage(int32 fromStateId, int32 toStateId, std::string messageType, sol::object data)
    {
        {
            std::unique_lock<std::shared_mutex> lock(messageQueueMutex);
            messageQueue[toStateId].emplace_back(fromStateId, toStateId, std::move(messageType), std::move(data));
        }
        ProcessMessages(toStateId);
    }

    void MessageManager::BroadcastMessage(int32 fromStateId, std::string messageType, sol::object data)
    {
        std::vector<int32> stateIds;
        
        {
            std::shared_lock<std::shared_mutex> lock(messageHandlersMutex);
            stateIds.reserve(messageHandlers.size());
            for (const auto& [stateId, handlers] : messageHandlers) {
                if (handlers.find(messageType) != handlers.end()) {
                    stateIds.emplace_back(stateId);
                }
            }
        }
        
        for (int32 stateId : stateIds) {
            SendMessage(fromStateId, stateId, messageType, data);
        }
    }

    void MessageManager::RegisterMessageEvent(int32 stateId, std::string messageType, sol::function callback)
    {
        if (callback.valid())
        {
            std::unique_lock<std::shared_mutex> lock(messageHandlersMutex);
            auto& handlers = messageHandlers[stateId][std::move(messageType)];
            handlers.reserve(4);
            handlers.emplace_back(std::move(callback));
        }
    }

    void MessageManager::ProcessMessages(int32 stateId)
    {
        std::vector<StateMessage> messagesToProcess;
        
        {
            std::unique_lock<std::shared_mutex> lock(messageQueueMutex);
            auto queueIt = messageQueue.find(stateId);
            if (queueIt == messageQueue.end() || queueIt->second.empty())
            {
                return;
            }
            messagesToProcess = std::move(queueIt->second);
            queueIt->second.clear();
        }

        for (const auto& message : messagesToProcess)
        {
            DeliverMessage(message);
        }
    }

    void MessageManager::DeliverMessage(const StateMessage& message)
    {
        std::vector<sol::function> handlersToCall;
        
        {
            std::shared_lock<std::shared_mutex> lock(messageHandlersMutex);
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
            
            handlersToCall = typeIt->second;
        }

        for (auto& handler : handlersToCall)
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
        {
            std::unique_lock<std::shared_mutex> lock(messageHandlersMutex);
            messageHandlers.erase(stateId);
        }
        {
            std::unique_lock<std::shared_mutex> lock(messageQueueMutex);
            messageQueue.erase(stateId);
        }
    }

}