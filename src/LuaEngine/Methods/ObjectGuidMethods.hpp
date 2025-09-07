#ifndef ECLIPSE_OBJECT_METHODS_HPP
#define ECLIPSE_OBJECT_METHODS_HPP

#include "EclipseIncludes.hpp"
#include "ObjectGuid.h"

namespace Eclipse
{
    namespace ObjectGuidMethods
    {
        /**
         * Get the raw 64-bit value of the ObjectGuid
         */
        inline uint64 GetRawValue(ObjectGuid* guid)
        {
            return guid->GetRawValue();
        }

        /**
         * Get the high part of the GUID
         */
        inline uint32 GetHigh(ObjectGuid* guid)
        {
            return static_cast<uint32>(guid->GetHigh());
        }

        /**
         * Get the entry part of the GUID (for entities that have entries)
         */
        inline uint32 GetEntry(ObjectGuid* guid)
        {
            return guid->GetEntry();
        }

        /**
         * Get the counter part of the GUID
         */
        inline uint32 GetCounter(ObjectGuid* guid)
        {
            return guid->GetCounter();
        }

        /**
         * Check if the GUID is empty
         */
        inline bool IsEmpty(ObjectGuid* guid)
        {
            return guid->IsEmpty();
        }

        /**
         * Check if the GUID represents a creature
         */
        inline bool IsCreature(ObjectGuid* guid)
        {
            return guid->IsCreature();
        }

        /**
         * Check if the GUID represents a pet
         */
        inline bool IsPet(ObjectGuid* guid)
        {
            return guid->IsPet();
        }

        /**
         * Check if the GUID represents a vehicle
         */
        inline bool IsVehicle(ObjectGuid* guid)
        {
            return guid->IsVehicle();
        }

        /**
         * Check if the GUID represents a creature or pet
         */
        inline bool IsCreatureOrPet(ObjectGuid* guid)
        {
            return guid->IsCreatureOrPet();
        }

        /**
         * Check if the GUID represents a creature or vehicle
         */
        inline bool IsCreatureOrVehicle(ObjectGuid* guid)
        {
            return guid->IsCreatureOrVehicle();
        }

        /**
         * Check if the GUID represents any type of creature (creature, pet, or vehicle)
         */
        inline bool IsAnyTypeCreature(ObjectGuid* guid)
        {
            return guid->IsAnyTypeCreature();
        }

        /**
         * Check if the GUID represents a player
         */
        inline bool IsPlayer(ObjectGuid* guid)
        {
            return guid->IsPlayer();
        }

        /**
         * Check if the GUID represents a unit (player or any creature type)
         */
        inline bool IsUnit(ObjectGuid* guid)
        {
            return guid->IsUnit();
        }

        /**
         * Check if the GUID represents an item
         */
        inline bool IsItem(ObjectGuid* guid)
        {
            return guid->IsItem();
        }

        /**
         * Check if the GUID represents a game object
         */
        inline bool IsGameObject(ObjectGuid* guid)
        {
            return guid->IsGameObject();
        }

        /**
         * Check if the GUID represents a dynamic object
         */
        inline bool IsDynamicObject(ObjectGuid* guid)
        {
            return guid->IsDynamicObject();
        }

        /**
         * Check if the GUID represents a corpse
         */
        inline bool IsCorpse(ObjectGuid* guid)
        {
            return guid->IsCorpse();
        }

        /**
         * Check if the GUID represents a transport
         */
        inline bool IsTransport(ObjectGuid* guid)
        {
            return guid->IsTransport();
        }

        /**
         * Check if the GUID represents a MO transport
         */
        inline bool IsMOTransport(ObjectGuid* guid)
        {
            return guid->IsMOTransport();
        }

        /**
         * Check if the GUID represents any type of game object
         */
        inline bool IsAnyTypeGameObject(ObjectGuid* guid)
        {
            return guid->IsAnyTypeGameObject();
        }

        /**
         * Check if the GUID represents an instance
         */
        inline bool IsInstance(ObjectGuid* guid)
        {
            return guid->IsInstance();
        }

        /**
         * Check if the GUID represents a group
         */
        inline bool IsGroup(ObjectGuid* guid)
        {
            return guid->IsGroup();
        }

        /**
         * Get the TypeID of the GUID
         */
        inline uint32 GetTypeId(ObjectGuid* guid)
        {
            return static_cast<uint32>(guid->GetTypeId());
        }

        /**
         * Get the type name as string
         */
        inline std::string GetTypeName(ObjectGuid* guid)
        {
            return std::string(guid->GetTypeName());
        }

        /**
         * Convert GUID to string representation
         */
        inline std::string ToString(ObjectGuid* guid)
        {
            return guid->ToString();
        }

        /**
         * Clear the GUID (set to empty)
         */
        inline void Clear(ObjectGuid* guid)
        {
            guid->Clear();
        }

        /**
         * Set the GUID to a raw 64-bit value
         */
        inline void Set(ObjectGuid* guid, uint64 rawValue)
        {
            guid->Set(rawValue);
        }

        /**
         * Create an ObjectGuid from raw value
         */
        inline ObjectGuid CreateFromRaw(uint64 rawValue)
        {
            return ObjectGuid(rawValue);
        }

        /**
         * Compare two ObjectGuids for equality
         */
        inline bool IsEqual(ObjectGuid* guid1, ObjectGuid* guid2)
        {
            return *guid1 == *guid2;
        }

        /**
         * Compare two ObjectGuids for inequality
         */
        inline bool IsNotEqual(ObjectGuid* guid1, ObjectGuid* guid2)
        {
            return *guid1 != *guid2;
        }
        // ========== REGISTRATION ==========

        template<typename T>
        void RegisterObjectGuidMethods(sol::usertype<T>& type)
        {
            // Basic info getters
            type["GetRawValue"] = &GetRawValue;
            type["GetHigh"] = &GetHigh;
            type["GetEntry"] = &GetEntry;
            type["GetCounter"] = &GetCounter;
            type["GetTypeId"] = &GetTypeId;
            type["GetTypeName"] = &GetTypeName;
            type["ToString"] = &ToString;

            // Type checking methods
            type["IsEmpty"] = &IsEmpty;
            type["IsCreature"] = &IsCreature;
            type["IsPet"] = &IsPet;
            type["IsVehicle"] = &IsVehicle;
            type["IsCreatureOrPet"] = &IsCreatureOrPet;
            type["IsCreatureOrVehicle"] = &IsCreatureOrVehicle;
            type["IsAnyTypeCreature"] = &IsAnyTypeCreature;
            type["IsPlayer"] = &IsPlayer;
            type["IsUnit"] = &IsUnit;
            type["IsItem"] = &IsItem;
            type["IsGameObject"] = &IsGameObject;
            type["IsDynamicObject"] = &IsDynamicObject;
            type["IsCorpse"] = &IsCorpse;
            type["IsTransport"] = &IsTransport;
            type["IsMOTransport"] = &IsMOTransport;
            type["IsAnyTypeGameObject"] = &IsAnyTypeGameObject;
            type["IsInstance"] = &IsInstance;
            type["IsGroup"] = &IsGroup;

            type["Clear"] = &Clear;
            type["Set"] = &Set;

            type["IsEqual"] = &IsEqual;
            type["IsNotEqual"] = &IsNotEqual;
        }
    }
}

#endif // ECLIPSE_OBJECT_METHODS_HPP