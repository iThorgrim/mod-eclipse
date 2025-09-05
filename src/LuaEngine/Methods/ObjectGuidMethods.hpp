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
        uint64 GetRawValue(ObjectGuid* guid)
        {
            return guid->GetRawValue();
        }

        /**
         * Get the high part of the GUID
         */
        uint32 GetHigh(ObjectGuid* guid)
        {
            return static_cast<uint32>(guid->GetHigh());
        }

        /**
         * Get the entry part of the GUID (for entities that have entries)
         */
        uint32 GetEntry(ObjectGuid* guid)
        {
            return guid->GetEntry();
        }

        /**
         * Get the counter part of the GUID
         */
        uint32 GetCounter(ObjectGuid* guid)
        {
            return guid->GetCounter();
        }

        /**
         * Check if the GUID is empty
         */
        bool IsEmpty(ObjectGuid* guid)
        {
            return guid->IsEmpty();
        }

        /**
         * Check if the GUID represents a creature
         */
        bool IsCreature(ObjectGuid* guid)
        {
            return guid->IsCreature();
        }

        /**
         * Check if the GUID represents a pet
         */
        bool IsPet(ObjectGuid* guid)
        {
            return guid->IsPet();
        }

        /**
         * Check if the GUID represents a vehicle
         */
        bool IsVehicle(ObjectGuid* guid)
        {
            return guid->IsVehicle();
        }

        /**
         * Check if the GUID represents a creature or pet
         */
        bool IsCreatureOrPet(ObjectGuid* guid)
        {
            return guid->IsCreatureOrPet();
        }

        /**
         * Check if the GUID represents a creature or vehicle
         */
        bool IsCreatureOrVehicle(ObjectGuid* guid)
        {
            return guid->IsCreatureOrVehicle();
        }

        /**
         * Check if the GUID represents any type of creature (creature, pet, or vehicle)
         */
        bool IsAnyTypeCreature(ObjectGuid* guid)
        {
            return guid->IsAnyTypeCreature();
        }

        /**
         * Check if the GUID represents a player
         */
        bool IsPlayer(ObjectGuid* guid)
        {
            return guid->IsPlayer();
        }

        /**
         * Check if the GUID represents a unit (player or any creature type)
         */
        bool IsUnit(ObjectGuid* guid)
        {
            return guid->IsUnit();
        }

        /**
         * Check if the GUID represents an item
         */
        bool IsItem(ObjectGuid* guid)
        {
            return guid->IsItem();
        }

        /**
         * Check if the GUID represents a game object
         */
        bool IsGameObject(ObjectGuid* guid)
        {
            return guid->IsGameObject();
        }

        /**
         * Check if the GUID represents a dynamic object
         */
        bool IsDynamicObject(ObjectGuid* guid)
        {
            return guid->IsDynamicObject();
        }

        /**
         * Check if the GUID represents a corpse
         */
        bool IsCorpse(ObjectGuid* guid)
        {
            return guid->IsCorpse();
        }

        /**
         * Check if the GUID represents a transport
         */
        bool IsTransport(ObjectGuid* guid)
        {
            return guid->IsTransport();
        }

        /**
         * Check if the GUID represents a MO transport
         */
        bool IsMOTransport(ObjectGuid* guid)
        {
            return guid->IsMOTransport();
        }

        /**
         * Check if the GUID represents any type of game object
         */
        bool IsAnyTypeGameObject(ObjectGuid* guid)
        {
            return guid->IsAnyTypeGameObject();
        }

        /**
         * Check if the GUID represents an instance
         */
        bool IsInstance(ObjectGuid* guid)
        {
            return guid->IsInstance();
        }

        /**
         * Check if the GUID represents a group
         */
        bool IsGroup(ObjectGuid* guid)
        {
            return guid->IsGroup();
        }

        /**
         * Get the TypeID of the GUID
         */
        uint32 GetTypeId(ObjectGuid* guid)
        {
            return static_cast<uint32>(guid->GetTypeId());
        }

        /**
         * Get the type name as string
         */
        std::string GetTypeName(ObjectGuid* guid)
        {
            return std::string(guid->GetTypeName());
        }

        /**
         * Convert GUID to string representation
         */
        std::string ToString(ObjectGuid* guid)
        {
            return guid->ToString();
        }

        /**
         * Clear the GUID (set to empty)
         */
        void Clear(ObjectGuid* guid)
        {
            guid->Clear();
        }

        /**
         * Set the GUID to a raw 64-bit value
         */
        void Set(ObjectGuid* guid, uint64 rawValue)
        {
            guid->Set(rawValue);
        }

        /**
         * Create an ObjectGuid from raw value
         */
        ObjectGuid CreateFromRaw(uint64 rawValue)
        {
            return ObjectGuid(rawValue);
        }

        /**
         * Compare two ObjectGuids for equality
         */
        bool IsEqual(ObjectGuid* guid1, ObjectGuid* guid2)
        {
            return *guid1 == *guid2;
        }

        /**
         * Compare two ObjectGuids for inequality
         */
        bool IsNotEqual(ObjectGuid* guid1, ObjectGuid* guid2)
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