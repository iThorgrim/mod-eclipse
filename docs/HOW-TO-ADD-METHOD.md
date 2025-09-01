# 🚀 Lua Method Binding Guide

Welcome to the **Eclipse Lua Engine** method binding guide! This comprehensive tutorial will teach you how to expose C++ class methods to Lua using Sol2. Let's make your C++ methods accessible from Lua scripts! 🎯

## 📋 Table of Contents

- [Getting Started](#getting-started)
- [Basic Method Binding](#basic-method-binding)
- [Advanced Cases](#advanced-cases)
- [Wrapper Functions](#wrapper-functions)
- [Examples from the Wild](#examples-from-the-wild)
- [Troubleshooting](#troubleshooting)
- [Best Practices](#best-practices)

## 🎬 Getting Started

Method bindings are organized in dedicated files like `PlayerMethods.hpp`, `CreatureMethods.hpp`, etc. These files contain everything you need to expose class methods to Lua scripts.

### The Magic Formula (NEW APPROACH)

**Eclipse now uses wrapper functions for ALL methods to ensure complete API consistency:**

```cpp
// 1. Create wrapper function:
/**
 *
 */
ReturnType MethodName(Object* object, params...)
{
    return object->MethodName(params...);
}

// 2. Register wrapper:
type["MethodName"] = &MethodName;
```

That's it! Clean, simple, and completely consistent! 📚

## 🔧 Basic Method Binding

### Regular Methods

For standard methods, create wrapper functions:

```cpp
// C++ Method: bool HasItemCount(uint32 item, uint32 count, bool includeBank) const;

// Wrapper function:
/**
 *
 */
bool HasItemCount(Player* player, uint32 item, uint32 count, bool includeBank)
{
    return player->HasItemCount(item, count, includeBank);
}

// Registration:
type["HasItemCount"] = &HasItemCount;
```

**Lua Usage:**
```lua
if player:HasItemCount(12345, 5, true) then
    print("Player has 5+ of item 12345!")
end
```

### Wrapper Function Examples

| C++ Method | Wrapper Function |
|------------|-----------------|
| `bool IsActive() const` | `bool IsActive(Player* player) { return player->IsActive(); }` |
| `void SetLevel(uint8 level)` | `void SetLevel(Player* player, uint8 level) { player->SetLevel(level); }` |
| `uint32 GetHealth() const` | `uint32 GetHealth(Player* player) { return player->GetHealth(); }` |
| `void MoveTo(float x, float y, float z)` | `void MoveTo(Player* player, float x, float y, float z) { player->MoveTo(x, y, z); }` |

### 🔑 The Wrapper Rule

Always create wrapper functions for ALL methods!

```cpp
/**
 *
 */
uint8 GetLevel(Player* player)
{
    return player->GetLevel();
}

type["GetLevel"] = &GetLevel;
```

## 🎭 Advanced Cases

### Static Methods with Wrapper Functions

For consistency, ALL methods (including static ones) use wrapper functions to ensure uniform `object:Method()` API:

```cpp
// C++: static uint8 GetAttackBySlot(uint8 slot);
// Wrapper function:
/**
 *
 */
uint8 GetAttackBySlot(Player* player, uint8 slot)
{
    (void)player; // Unused parameter for static methods
    return Player::GetAttackBySlot(slot);
}

// Registration:
type["GetAttackBySlot"] = &GetAttackBySlot;
```

**Lua Usage:**
```lua
local attack = player:GetAttackBySlot(5)  -- Consistent player:method syntax
```

### Method Overloads with Wrapper Functions

When you have multiple methods with the same name but different parameters, create descriptively named wrapper functions:

```cpp
// C++ has two versions:
// bool IsValidPos(uint16 pos);
// bool IsValidPos(uint8 bag, uint8 slot);

// Create two wrapper functions with descriptive names:
/**
 *
 */
bool IsValidPosBySlot(Player* player, uint16 pos, bool explicit_pos)
{
    return player->IsValidPos(pos, explicit_pos);
}

/**
 *
 */
bool IsValidPosByBagSlot(Player* player, uint8 bag, uint8 slot, bool explicit_pos)
{
    return player->IsValidPos(bag, slot, explicit_pos);
}

// Register with sol::overload:
type["IsValidPos"] = sol::overload(
    &IsValidPosBySlot,
    &IsValidPosByBagSlot
);
```

**Lua Usage:**
```lua
-- Sol2 automatically chooses the right overload!
local result1 = player:IsValidPos(256, true)        -- Uses first wrapper
local result2 = player:IsValidPos(1, 0, true)       -- Uses second wrapper
```

### Static Method Overloads

For static method overloads, follow the same pattern but use `(void)object;`:

```cpp
/**
 *
 */
bool IsInventoryPosBySlot(Player* player, uint16 pos)
{
    (void)player; // Unused parameter
    return Player::IsInventoryPos(pos);
}

/**
 *
 */
bool IsInventoryPosByBagSlot(Player* player, uint8 bag, uint8 slot)
{
    (void)player; // Unused parameter
    return Player::IsInventoryPos(bag, slot);
}

type["IsInventoryPos"] = sol::overload(
    &IsInventoryPosBySlot,
    &IsInventoryPosByBagSlot
);
```

## 🎪 Wrapper Functions

Sometimes C++ methods aren't Lua-friendly. That's where wrapper functions save the day!

### Problem: Primitive Output Parameters

```cpp
// C++ Method: InventoryResult CanStoreItem(uint8 bag, uint8 slot, ItemContainer& dest, uint32 entry, uint32 count, uint32* no_space_count) const;
```

**Problem:** Lua can't handle `uint32*` parameters.

**Solution:** Create a wrapper that returns a tuple:

```cpp
std::tuple<InventoryResult, uint32> CanStoreNewItem(Player* player, uint8 bag, uint8 slot, ItemPosCountVec& dest, uint32 entry, uint32 count)
{
    uint32 no_space_count = 0;
    InventoryResult result = player->CanStoreNewItem(bag, slot, dest, entry, count, &no_space_count);
    return std::make_tuple(result, no_space_count);
}
};

type["CanStoreNewItem"] = &CanStoreNewItem;
```

**Lua Usage:**
```lua
local result, no_space = player:CanStoreNewItem(bag, slot, dest, entry, count)
if result == SUCCESS then
    print("Success!")
else
    print("Failed! No space for " .. no_space .. " items")
end
```

### Problem: Object Pointer Arrays

```cpp
// C++ Method: InventoryResult CanStoreItems(Item** pItems, int count) const;
```

**Problem:** Lua can't create `Item**` arrays.

**Solution:** Use vector conversion:

```cpp
InventoryResult CanStoreItems(Player* player, const std::vector<Item*>& items, int count)
{
    std::vector<Item*> item_ptrs = items;
    return player->CanStoreItems(item_ptrs.data(), count);
}

type["CanStoreItems"] = &CanStoreItems;
```

**Lua Usage:**
```lua
local items = {item1, item2, item3}
local result = player:ProcessItems(items, 3)
```

## 🌟 Examples from the Wild

Here are some real examples from the new Eclipse method binding approach:

### Simple Getters/Setters
```cpp
// Wrapper functions:
/**
 *
 */
bool IsGameMaster(Player* player)
{
    return player->IsGameMaster();
}

/**
 *
 */
void SetGameMaster(Player* player, bool gm)
{
    player->SetGameMaster(gm);
}

// Registration:
type["IsGameMaster"] = &IsGameMaster;
type["SetGameMaster"] = &SetGameMaster;
```

### Complex Methods
```cpp
/**
 *
 */
bool TeleportTo(Player* player, uint32 mapId, float x, float y, float z, float orientation, uint32 options, Unit* target, bool forced)
{
    return player->TeleportTo(mapId, x, y, z, orientation, options, target, forced);
}

type["TeleportTo"] = &TeleportTo;
```

### Overloaded Methods
```cpp
// Multiple wrapper functions:
/**
 *
 */
Item* GetItemByPosSlot(Player* player, uint16 pos)
{
    return player->GetItemByPos(pos);
}

/**
 *
 */
Item* GetItemByPosBagSlot(Player* player, uint8 bag, uint8 slot)
{
    return player->GetItemByPos(bag, slot);
}

// Registration with sol::overload:
type["GetItemByPos"] = sol::overload(
    &GetItemByPosSlot,
    &GetItemByPosBagSlot
);
```

## 🚨 Troubleshooting

### Common Issues with Wrapper Functions

#### "unused parameter 'player'" warning
**Problem:** Compiler warning for static method wrappers.
```cpp
// ❌ Warning
bool IsInventoryPos(Player* player, uint16 pos)
{
    return Player::IsInventoryPos(pos);
}

// ✅ Correct (suppress warning)
bool IsInventoryPos(Player* player, uint16 pos)
{
    (void)player; // Unused parameter
    return Player::IsInventoryPos(pos);
}
```

#### "forward declaration of 'SomeClass'" error
**Problem:** Missing include for a type used in the wrapper function signature.
```cpp
// Add the missing include at the top of PlayerMethods.hpp
#include "SomeClass.h"
```

#### Wrapper function compilation error
**Problem:** Wrapper function signature doesn't match the actual C++ method.
```cpp
// ❌ Wrong parameter types or count
bool HasItem(Player* player, uint32 item)
{
    return player->HasItem(item, 1, false); // Method actually needs 3 parameters
}

// ✅ Correct - match the real C++ method signature
bool HasItem(Player* player, uint32 item, uint32 count, bool includeBank)
{
    return player->HasItem(item, count, includeBank);
}
```

## ✨ Best Practices

### 1. 📝 Document Your Methods
Always add JavaDoc-style comments above your wrapper functions:

```cpp
/**
 * Teleports the player to the specified location
 * @param player The player instance  
 * @param mapId The map ID to teleport to
 * @param x X coordinate
 * @param y Y coordinate  
 * @param z Z coordinate
 * @param orientation Player orientation
 */
bool TeleportTo(Player* player, uint32 mapId, float x, float y, float z, float orientation)
{
    return player->TeleportTo(mapId, x, y, z, orientation);
}
```

### 2. 🎯 Use Descriptive Names
Choose clear, descriptive names for overloaded wrapper functions:

```cpp
// ✅ Good - descriptive names
bool GetItemByPosSlot(Player* player, uint16 pos) { /* ... */ }
bool GetItemByPosBagSlot(Player* player, uint8 bag, uint8 slot) { /* ... */ }

// ❌ Bad - generic names
bool GetItemByPos1(Player* player, uint16 pos) { /* ... */ }
bool GetItemByPos2(Player* player, uint8 bag, uint8 slot) { /* ... */ }
```

### 3. 🔍 Group Related Methods
Organize your wrapper functions and registrations in logical groups:

```cpp
bool GetItemCount(Player* player, uint32 item) { /* ... */ }
void MoveTo(Player* player, float x, float y, float z) { /* ... */ }
bool CanStoreItem(Player* player, uint8 bag, uint8 slot) { /* ... */ }
bool TeleportTo(Player* player, uint32 mapId, float x, float y, float z) { /* ... */ }

// ========== LUA REGISTRATION ==========

// Getters
type["GetItemCount"] = &GetItemCount;

// Booleans
type["CanStoreItem"] = &CanStoreItem;

// Actions
type["MoveTo"] = &MoveTo;
type["TeleportTo"] = &TeleportTo;
```

### 4. 🧪 Test Your Bindings
Always test your wrapper functions with simple Lua scripts:

```lua
-- Test script
print("Player level: " .. player:GetLevel())
print("Player is GM: " .. tostring(player:IsGameMaster()))
print("Player can store item: " .. tostring(player:CanStoreItem(255, 0, {}, someItem, false)))
```

### 5. 📂 File Organization
Keep method bindings organized in dedicated files:
- `PlayerMethods.hpp` - Player-specific methods
- `CreatureMethods.hpp` - Creature-specific methods  
- `ItemMethods.hpp` - Item-specific methods
- etc.

## 📚 Quick Reference

### Method Types Cheat Sheet

| Method Type | Wrapper Function | Registration |
|-------------|-----------------|-------------|
| Instance method | `ReturnType Method(Player* player, params) { return player->Method(params); }` | `type["Method"] = &Method;` |
| Static method | `ReturnType Method(Player* player, params) { (void)player; return Player::Method(params); }` | `type["Method"] = &Method;` |
| Instance overload | `ReturnType Method1(...) { ... } ReturnType Method2(...) { ... }` | `sol::overload(&Method1, &Method2)` |
| Static overload | `ReturnType Method1(...) { (void)player; ... } ReturnType Method2(...) { (void)player; ... }` | `sol::overload(&Method1, &Method2)` |

### When to Use Wrapper Functions

**ALL methods now use wrapper functions for consistency:**

- ✅ **Always use wrappers** - provides uniform `player:Method()` API
- ✅ Instance methods - direct `player->Method()` call
- ✅ Static methods - use `(void)player;` and `Player::Method()` call  
- ✅ Overloaded methods - create descriptively named wrappers
- ✅ Complex signatures - wrapper functions handle any complexity

## 🎉 Congratulations!

You're now ready to bind C++ class methods to Lua using Eclipse's new unified approach! Remember:

- ✅ **Always create wrapper functions** for ALL methods
- ✅ Use `/**\n *\n */` JavaDoc comments above each wrapper  
- ✅ Use `(void)player;` for static method wrappers
- ✅ Use descriptive names for overloaded method wrappers
- ✅ Register with simple `type["Method"] = &WrapperFunction;`
- ✅ Everything uses `player:Method()` syntax in Lua!

**The result: A completely uniform, predictable API that's easy to use and maintain!** 🚀✨

---

*Need help? Check the existing bindings in the `Methods/` directory for more examples, or ask the development team!*