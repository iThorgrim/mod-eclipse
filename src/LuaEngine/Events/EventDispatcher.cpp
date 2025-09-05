#include "EventDispatcher.hpp"
#include "MapStateManager.hpp"

namespace Eclipse
{
    EventDispatcher& EventDispatcher::GetInstance()
    {
        static EventDispatcher instance;
        return instance;
    }

    template<typename T>
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(T* object)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(2);

        auto& manager = MapStateManager::GetInstance();

        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        if (object)
        {
            Map* objectMap = nullptr;

            if constexpr (std::is_same_v<T, Player>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, Map>)
            {
                objectMap = object;
            }
            else if constexpr (std::is_same_v<T, Creature>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, GameObject>)
            {
                objectMap = object->GetMap();
            }
            else if constexpr (std::is_same_v<T, Item>)
            {
                if (Player* owner = object->GetOwner())
                {
                    objectMap = owner->GetMap();
                }
            }

            if (objectMap)
            {
                uint32 mapId = objectMap->GetId();
                auto* mapEngine = manager.GetStateForMap(mapId);

                if (mapEngine && mapEngine != globalEngine)
                {
                    engines.emplace_back(mapEngine);
                }
            }
        }

        return engines;
    }

    template<>
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<ObjectGuid>(ObjectGuid* object)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(1);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        return engines;
    }

    // Wrapper type implementations
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(const PlayerGuid& playerGuid)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(1);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        return engines;
    }

    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(const CreatureEntry& creatureEntry)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(1);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        return engines;
    }

    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(const GameObjectEntry& gameObjectEntry)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(1);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        return engines;
    }

    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(const ItemEntry& itemEntry)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(1);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        return engines;
    }

    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(const MapId& mapId)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(2);

        auto& manager = MapStateManager::GetInstance();
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.emplace_back(globalEngine);
        }

        auto* mapEngine = manager.GetStateForMap(mapId.mapId);
        if (mapEngine && mapEngine != globalEngine)
        {
            engines.emplace_back(mapEngine);
        }

        return engines;
    }

    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<Player>(Player*);
    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<const Player>(const Player*);
    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<Creature>(Creature*);
    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<GameObject>(GameObject*);
    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<Item>(Item*);
    template std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines<Map>(Map*);
}

