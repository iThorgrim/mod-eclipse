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

### The Magic Formula

```cpp
type["MethodName"] = static_cast<signature>(&ClassName::MethodName);
```

That's it! But there's more to the story... 📚

## 🔧 Basic Method Binding

### Regular Methods

For standard methods, use the basic pattern:

```cpp
// C++ Method: bool HasItemCount(uint32 item, uint32 count, bool includeBank) const;
type["HasItemCount"] = static_cast<bool(ClassName::*)(uint32, uint32, bool) const>(&ClassName::HasItemCount);
```

**Lua Usage:**
```lua
if object:HasItemCount(12345, 5, true) then
    print("Object has 5+ of item 12345!")
end
```

### Signature Examples

| C++ Method | Sol2 Signature |
|------------|---------------|
| `bool IsActive() const` | `bool(ClassName::*)() const` |
| `void SetLevel(uint8 level)` | `void(ClassName::*)(uint8)` |
| `uint32 GetHealth() const` | `uint32(ClassName::*)() const` |
| `void MoveTo(float x, float y, float z)` | `void(ClassName::*)(float, float, float)` |

### 🔑 The const Rule

Always include `const` in your signature if the C++ method is const!

```cpp
// ✅ Correct
type["GetLevel"] = static_cast<uint8(ClassName::*)() const>(&ClassName::GetLevel);

// ❌ Wrong - missing const
type["GetLevel"] = static_cast<uint8(ClassName::*)()>(&ClassName::GetLevel);
```

## 🎭 Advanced Cases

### Static Methods

Static methods are special - they don't need the complex casting:

```cpp
// C++: static uint8 GetAttackBySlot(uint8 slot);
type["GetAttackBySlot"] = &ClassName::GetAttackBySlot;
```

**Lua Usage:**
```lua
local attack = ClassName.GetAttackBySlot(5)  -- Note: ClassName.method, not object:method
```

### Method Overloads

When you have multiple methods with the same name but different parameters, use `sol::overload`:

```cpp
// C++ has two versions:
// bool IsValidPos(uint16 pos);
// bool IsValidPos(uint8 x, uint8 y);

type["IsValidPos"] = sol::overload(
    static_cast<bool(*)(uint16)>(&ClassName::IsValidPos),
    static_cast<bool(*)(uint8, uint8)>(&ClassName::IsValidPos)
);
```

**Lua Usage:**
```lua
-- Sol2 automatically chooses the right overload!
local result1 = ClassName.IsValidPos(256)        -- Uses first overload
local result2 = ClassName.IsValidPos(1, 0)       -- Uses second overload
```

### Static Method Overloads

For static method overloads, use function pointer syntax `return_type(*)(params)`:

```cpp
type["StaticMethod"] = sol::overload(
    static_cast<bool(*)(uint16)>(&ClassName::StaticMethod),
    static_cast<bool(*)(uint8, uint8)>(&ClassName::StaticMethod)
);
```

### Instance Method Overloads

For instance method overloads, use member function pointer syntax `return_type(ClassName::*)(params)`:

```cpp
type["InstanceMethod"] = sol::overload(
    static_cast<bool(ClassName::*)(uint16)>(&ClassName::InstanceMethod),
    static_cast<bool(ClassName::*)(uint8, uint8)>(&ClassName::InstanceMethod)
);
```

## 🎪 Wrapper Functions

Sometimes C++ methods aren't Lua-friendly. That's where wrapper functions save the day!

### Problem: Primitive Output Parameters

```cpp
// C++ Method: InventoryResult CanStoreItem(uint8 bag, uint8 slot, ItemContainer& dest, 
//                                          uint32 entry, uint32 count, uint32* no_space_count) const;
```

**Problem:** Lua can't handle `uint32*` parameters.

**Solution:** Create a wrapper that returns a tuple:

```cpp
auto CanStoreItemWrapper = [](ClassName* object, uint8 bag, uint8 slot, ItemContainer& dest, 
                             uint32 entry, uint32 count) -> std::tuple<InventoryResult, uint32>
{
    uint32 no_space_count = 0;
    InventoryResult result = object->CanStoreItem(bag, slot, dest, entry, count, &no_space_count);
    return std::make_tuple(result, no_space_count);
};

type["CanStoreItem"] = CanStoreItemWrapper;
```

**Lua Usage:**
```lua
local result, no_space = object:CanStoreItem(bag, slot, dest, entry, count)
if result == SUCCESS then
    print("Success!")
else
    print("Failed! No space for " .. no_space .. " items")
end
```

### Problem: Object Pointer Arrays

```cpp
// C++ Method: InventoryResult ProcessItems(Item** pItems, int count) const;
```

**Problem:** Lua can't create `Item**` arrays.

**Solution:** Use vector conversion:

```cpp
auto ProcessItemsWrapper = [](ClassName* object, const std::vector<Item*>& items, int count) -> InventoryResult
{
    std::vector<Item*> item_ptrs = items;
    return object->ProcessItems(item_ptrs.data(), count);
};

type["ProcessItems"] = ProcessItemsWrapper;
```

**Lua Usage:**
```lua
local items = {item1, item2, item3}
local result = object:ProcessItems(items, 3)
```

## 🌟 Examples from the Wild

Here are some real examples from method binding files:

### Simple Getters/Setters
```cpp
type["IsActive"] = static_cast<bool(Player::*)() const>(&Player::IsActive);
type["SetGameMaster"] = static_cast<void(Player::*)(bool)>(&Player::SetGameMaster);
```

### Complex Methods
```cpp
type["TeleportTo"] = static_cast<bool(Player::*)(uint32, float, float, float, float, uint32, Unit*, bool)>(&Player::TeleportTo);
```

### Overloaded Methods
```cpp
type["GetItemByPos"] = sol::overload(
    static_cast<Item*(Player::*)(uint16) const>(&Player::GetItemByPos),
    static_cast<Item*(Player::*)(uint8, uint8) const>(&Player::GetItemByPos)
);
```

## 🚨 Troubleshooting

### Common Errors and Solutions

#### "no type named 'function_pointer_type'"
**Problem:** Wrong signature, usually missing `const` or wrong parameter types.
```cpp
// ❌ Wrong
type["GetLevel"] = static_cast<uint8(ClassName::*)()>(&ClassName::GetLevel);

// ✅ Correct (added const)
type["GetLevel"] = static_cast<uint8(ClassName::*)() const>(&ClassName::GetLevel);
```

#### "static_cast from X to Y is not allowed"
**Problem:** Trying to cast a static method as an instance method.
```cpp
// ❌ Wrong
type["GetAttackBySlot"] = static_cast<uint8(ClassName::*)(uint8)>(&ClassName::GetAttackBySlot);

// ✅ Correct
type["GetAttackBySlot"] = &ClassName::GetAttackBySlot;
```

#### "forward declaration of 'SomeClass'"
**Problem:** Missing include for a type used in the method signature.
```cpp
// Add the missing include at the top
#include "SomeClass.h"
```

## ✨ Best Practices

### 1. 📝 Document Your Methods
Always add JavaDoc-style comments above your bindings:

```cpp
/**
 * Teleports the object to the specified location
 * @param mapId The map ID to teleport to
 * @param x X coordinate
 * @param y Y coordinate  
 * @param z Z coordinate
 * @param orientation Object orientation
 */
type["TeleportTo"] = static_cast<bool(ClassName::*)(uint32, float, float, float, float)>(&ClassName::TeleportTo);
```

### 2. 🎯 Use Descriptive Names
Choose clear, descriptive names for wrapper functions:

```cpp
// ✅ Good
auto CanStoreItemWrapper = [](ClassName* object, ...) { /* ... */ };

// ❌ Bad
auto Wrapper1 = [](ClassName* object, ...) { /* ... */ };
```

### 3. 🔍 Group Related Methods
Organize your methods in logical groups:

```cpp
// ========== INVENTORY METHODS ==========
type["GetItemCount"] = /* ... */;
type["CanStoreItem"] = /* ... */;
type["ProcessItems"] = /* ... */;

// ========== MOVEMENT METHODS ==========
type["MoveTo"] = /* ... */;
type["TeleportTo"] = /* ... */;
```

### 4. 🧪 Test Your Bindings
Always test your method bindings with simple Lua scripts:

```lua
-- Test script
local object = GetObject() -- however you get the object
print("Object level: " .. object:GetLevel())
print("Object is active: " .. tostring(object:IsActive()))
```

### 5. 📂 File Organization
Keep method bindings organized in dedicated files:
- `PlayerMethods.hpp` - Player-specific methods
- `CreatureMethods.hpp` - Creature-specific methods  
- `ItemMethods.hpp` - Item-specific methods
- etc.

## 📚 Quick Reference

### Method Types Cheat Sheet

| Method Type | Syntax | Example |
|-------------|--------|---------|
| Instance method | `return_type(Class::*)(params)` | `bool(Player::*)(uint32) const` |
| Static method | `&Class::Method` | `&Player::GetAttackBySlot` |
| Instance overload | `sol::overload(cast1, cast2)` | See examples above |
| Static overload | `sol::overload(cast1, cast2)` | See examples above |

### When to Use Wrappers

- ✅ Methods with primitive output parameters (`uint32*`, `int*`, `bool*`)
- ✅ Methods with object pointer arrays (`Item**`, `Creature**`)  
- ✅ Methods with complex signatures that Lua can't handle
- ❌ Simple methods that Sol2 can bind directly

## 🎉 Congratulations!

You're now ready to bind C++ class methods to Lua like a pro! Remember:

- ✅ Use `static_cast` for instance methods
- ✅ Use `&Class::Method` for static methods  
- ✅ Include `const` when needed
- ✅ Use `sol::overload` for method overloads
- ✅ Create wrappers for complex parameter types
- ✅ Document everything!

Happy coding! 🚀✨

---

*Need help? Check the existing bindings in the `Methods/` directory for more examples, or ask the development team!*