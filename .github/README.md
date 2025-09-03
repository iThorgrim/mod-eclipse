# 🌙 Eclipse - High-Performance Lua Engine for AzerothCore

[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![AzerothCore](https://img.shields.io/badge/AzerothCore-Compatible-brightgreen.svg)](https://www.azerothcore.org/)
[![Status](https://img.shields.io/badge/Status-Production%20Ready-brightgreen.svg)](https://github.com/iThorgrim/mod-eclipse)
[![Performance](https://img.shields.io/badge/Performance-25%25%20Faster-success.svg)](#-performance-benchmarks)
[![LuaJIT](https://img.shields.io/badge/LuaJIT-Supported-orange.svg)](#-lua-version-support)

Eclipse is a **next-generation Lua scripting engine** designed specifically for AzerothCore, delivering **superior performance**, **advanced caching**, and **modern architecture**. Built with Sol2 and optimized for production environments.

---

## ⚡ Why Choose Eclipse?

| 🚀 **Performance** | 🧠 **Intelligence** | 🛡️ **Reliability** |
|-------------------|---------------------|-------------------|
| 25% faster than Eluna | Smart caching system | Production-tested |
| LuaJIT optimization | Hot reload capability | Comprehensive error handling |
| Memory efficient | MD5-based validation | Thread-safe architecture |

## 🎯 Key Features

### 🚀 **Performance Optimized**
- **25% faster execution** than existing solutions
- **LuaJIT support** for maximum performance
- **Optimized Sol2 configuration** with JIT-aware settings
- **Memory efficient** with superior garbage collection

### 🧠 **Smart & Modern**
- **Smart caching system** with MD5-based script validation
- **Hot reload capabilities** - modify scripts without server restart
- **Multi-state architecture** with global compiler and per-map instances
- **Dynamic path management** for automatic `require()` discovery

### 🛡️ **Production Ready**
- **Comprehensive error handling** with graceful failure recovery
- **Thread-safe message system** for inter-state communication
- **ConfigValueCache integration** with AzerothCore's native configuration
- **In-game management commands** with `.eclipse` suite

### 🎮 **Developer Friendly**
- **Extensive API bindings** for Player, Creature, GameObject, and more
- **Event system integration** for seamless hook management  
- **Comprehensive documentation** and examples
- **Easy migration path** from existing Lua solutions

---

## 🏗️ Architecture Overview

```
mod-eclipse/
├── 📁 src/
│   ├── 🧠 LuaEngine/
│   │   ├── 🏗️ Core/
│   │   │   ├── LuaEngine.cpp      # Main engine orchestrator  
│   │   │   ├── LuaState.cpp       # Sol2 state management
│   │   │   └── EclipseConfig.cpp  # ConfigValueCache integration
│   │   ├── 📜 Scripting/
│   │   │   ├── ScriptLoader.cpp   # Script discovery & loading
│   │   │   ├── LuaCompiler.cpp    # MoonScript compilation
│   │   │   ├── LuaCache.cpp       # MD5-based bytecode cache
│   │   │   └── LuaPathManager.cpp # Dynamic require() paths
│   │   ├── 🛠️ Utils/
│   │   │   └── MessageManager.cpp # Inter-state messaging
│   │   └── 🔗 Methods/
│   │       ├── Methods.cpp        # Core Lua bindings
│   │       ├── PlayerMethods.hpp  # Player API bindings
│   │       ├── CreatureMethods.hpp # Creature API bindings
│   │       └── GlobalMethods.cpp  # Engine management functions
│   └── 📚 sol/                    # Optimized Sol2 configuration
├── 📖 docs/                       # Documentation & guides
│   ├── HOW-TO-INSTALL.md         # Installation guide
│   └── HOW-TO-ADD-METHOD.md      # Method binding tutorial
└── 🧪 lua_scripts/examples/      # Example scripts
```

---

## 🚀 Quick Start

### 📦 Installation

**Step 1: Clone the Module**
```bash
cd azerothcore-wotlk/modules/
git clone https://github.com/iThorgrim/mod-eclipse.git
```

**Step 2: Apply CMake Patch**
```bash
git apply mod-eclipse/cmake_patch.diff
```

**Step 3: Configure & Build**
```bash
cd ../build
cmake .. -DLUA_VERSION=luajit  # Or lua54 for features
make -j$(nproc)
```

> 📚 **Need detailed instructions?** Check out our [Installation Guide](docs/HOW-TO-INSTALL.md)

### 🎮 First Script

Create `lua_scripts/hello_eclipse.lua`:
```lua
print("🌟 Hello Eclipse! Server is powered by high-performance Lua!")

-- Your first Eclipse script is ready!
-- Check server logs to see this message on startup
```

---

## 🎯 Implementation Status

### ✅ **Production Features**
- [x] **🏗️ Multi-state Architecture** - Global compiler (-1) + per-map instances  
- [x] **🧠 Advanced Caching** - MD5-based script validation with hot reload
- [x] **⚙️ ConfigValueCache Integration** - AzerothCore-native configuration  
- [x] **⚡ Sol2 Performance Optimization** - JIT-aware, zero-overhead configuration
- [x] **📁 Dynamic Path Management** - Automatic `require()` path discovery
- [x] **🔄 Thread-safe Message System** - Inter-state communication
- [x] **🛡️ Comprehensive Error Handling** - Graceful failure recovery
- [x] **🎮 In-game Management Commands** - `.eclipse` command suite
- [x] **🎯 Player API Bindings** - Complete Player method exposure
- [x] **🏰 Creature API Bindings** - Comprehensive Creature scripting
- [x] **🗃️ GameObject Bindings** - Full GameObject manipulation

### 🚧 **In Development**  
- [ ] 🔍 **Advanced Debugging Tools** - Profiling and script analysis
- [ ] 📊 **Performance Monitoring** - Real-time performance metrics
- [ ] 🎨 **Visual Script Editor** - Web-based script management
- [ ] 🌐 **REST API Integration** - External service communication

---

## 📊 Performance Benchmarks

### 🧪 **Test Environment**
- **💻 CPU:** AMD Ryzen 9 9950X
- **🧠 RAM:** 64GB DDR5-6400
- **🌟 Eclipse:** AzerothCore + mod-eclipse (LuaJIT)
- **📊 Baseline:** ElunaTrinityWotlk (LuaJIT)

### 🏆 **Benchmark Results**

| Test Category | Eclipse | Eluna | Performance Gain |
|---------------|---------|-------|:----------------:|
| **⚡ Overall Runtime** | 1.73s | 2.31s | **🚀 +25% faster** |
| **🧠 Memory Efficiency** | +13.6 KB | +52.6 KB | **💾 -74% memory usage** |
| **🔄 Function Calls** | 464M ops/s | 284M ops/s | **⚡ +63% faster** |
| **🎯 Metamethods** | 510M ops/s | 225M ops/s | **🎯 +127% faster** |
| **📊 Table Operations** | 113K ops/s | 70K ops/s | **📈 +61% faster** |
| **📝 String Processing** | 80K ops/s | 65K ops/s | **📝 +23% faster** |

> *Benchmarks performed using comprehensive stress tests with 500,000 iterations per test category.*

### 🎯 **Key Performance Advantages**

- **🔧 Sol2 Optimizations**: JIT-aware configuration with disabled safety checks in release builds
- **🧠 Smart Caching**: Bytecode cache with MD5 validation eliminates redundant compilation  
- **💾 Memory Efficiency**: Superior garbage collection and memory management
- **🏗️ Clean Architecture**: Optimized state management with separation of concerns

---

## 🎮 Lua Version Support

| Version | Status | Performance | Use Case |
|---------|:------:|:-----------:|----------|
| **LuaJIT** | ✅ | ⚡⚡⚡ **Best** | Production servers |
| **Lua 5.4** | ✅ | ⚡⚡ Great | Modern features |
| **Lua 5.3** | ✅ | ⚡⚡ Good | Stable choice |
| **Lua 5.2** | ✅ | ⚡ Basic | Legacy support |
| **Lua 5.1** | ✅ | ⚡ Basic | Compatibility |

> 🎯 **Recommendation**: Use **LuaJIT** for production environments to maximize performance benefits.

---

## 🔧 Configuration

### ⚙️ **mod_eclipse.conf Settings**

```ini
###################################################################################################
# ECLIPSE LUA ENGINE
###################################################################################################

# Enable/disable Eclipse engine
# Default: 1 (enabled)
Eclipse.Enabled = 1

# Script directory path (relative to server binary)
# Default: "lua_scripts"
Eclipse.ScriptPath = "lua_scripts"

# Additional require() search paths (semicolon separated)
# Default: "" (empty)
Eclipse.RequirePaths = ""

# Additional C library search paths (semicolon separated)  
# Default: "" (empty)
Eclipse.RequireCPaths = ""
```

### 🎮 **In-Game Commands**

| Command | Description | Example |
|---------|-------------|---------|
| `.eclipse reload` | 🔄 Hot reload all scripts | `.eclipse reload` |

---

## 🔧 API Reference  

### 🌟 **Core Functions**

```lua
-- 📝 Logging (Compatible with Eluna)
print(message)              -- ℹ️ Info level logging
log(message)                -- ℹ️ Info level logging  
error(message)              -- ❌ Error level logging
debug(message)              -- 🐛 Debug level logging
warn(message)               -- ⚠️ Warning level logging

-- 🌟 Eclipse-Specific Functions
ReloadScripts()             -- 🔄 Hot reload scripts
SendMessage(state, msg)     -- 💬 Inter-state messaging
GetScriptPath()             -- 📁 Get current script directory
```

### 🎯 **Player API Examples**

```lua
-- 🎮 Player Management
player:TeleportTo(0, -8949.95, -132.493, 83.5312, 0)
player:SendBroadcastMessage("🌟 Welcome to Eclipse!")
player:ModifyMoney(10000)  -- Add 1 gold

-- 🎒 Inventory Management
local itemCount = player:GetItemCount(12345)
player:AddItem(12345, 5)   -- Add 5 items with ID 12345

-- ⚔️ Combat & Stats
player:SetLevel(80)
player:GiveXP(1000)
local health = player:GetHealth()
```

### 🏰 **Creature API Examples**

```lua
-- 🏰 Creature Spawning & Management
local creature = player:SpawnCreature(123, x, y, z, o, 3, 300)
creature:SendUnitSay("🌟 Eclipse-powered NPC!", 0)
creature:CastSpell(creature, 12345)

-- 🎯 AI & Behavior
creature:AttackStart(player)
creature:MoveToPlayer(player)
creature:SetFacingToObject(player)
```

---

## 🛠️ Development

### 🏗️ **Building from Source**

**Prerequisites:**
- ✅ **C++17** compatible compiler (GCC 8+, Clang 7+, MSVC 2019+)
- ✅ **AzerothCore** development environment
- ✅ **CMake** 3.16+
- ✅ **Git** for version control

### 🤝 **Contributing**

We welcome contributions! Here's how to get started:

1. **🍴 Fork** the repository
2. **🌿 Create** your feature branch (`git checkout -b feature/amazing-feature`)
3. **💾 Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **📤 Push** to the branch (`git push origin feature/amazing-feature`)
5. **🔄 Open** a Pull Request

### 📋 **Development Guidelines**

- ✅ Follow C++17 best practices
- ✅ Write comprehensive tests
- ✅ Document new features
- ✅ Maintain Sol2 compatibility
- ✅ Ensure thread safety

---

## 📚 Documentation & Resources

### 📖 **Guides & Tutorials**
- 🚀 **[Installation Guide](docs/HOW-TO-INSTALL.md)** - Step-by-step installation
- 🔧 **[Method Binding Tutorial](docs/HOW-TO-ADD-METHOD.md)** - Expose C++ methods to Lua
- 🧪 **[Example Scripts](lua_scripts/examples/)** - Ready-to-use script examples

### 🔗 **External Resources**
- 📚 **[Sol2 Documentation](https://sol2.readthedocs.io/)** - Sol2 binding library
- 🌐 **[AzerothCore Wiki](https://www.azerothcore.org/wiki/)** - Server core documentation
- 🔥 **[LuaJIT Documentation](https://luajit.org/)** - LuaJIT performance guide
- 📖 **[Lua 5.4 Manual](https://www.lua.org/manual/5.4/)** - Official Lua reference

---

## ⚠️ Current Limitations

### 🚧 **Known Limitations**
- 🎮 **Extended API Coverage**: Some advanced game mechanics still in development
- 🔍 **Debugging Tools**: Advanced profiling features planned for next release
- 📊 **Performance Monitoring**: Real-time metrics dashboard coming soon

---

## 📄 License

This project is licensed under the **GNU General Public License v2.0** - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

### 🌟 **Core Team & Contributors**
- **🏗️ AzerothCore Team** - For the amazing WoW server core
- **⚡ Sol3 Team** - For the excellent modern Lua binding library  
- **🎯 Eluna Project** - For inspiring this modern approach to Lua scripting
- **👥 Community** - For feedback, testing, and continuous support

### 🚀 **Special Thanks**
- All beta testers who helped optimize performance
- Contributors who provided valuable feedback and bug reports
- The AzerothCore community for embracing modern Lua scripting

---

<div align="center">

## 🌟 Ready to Power Up Your Server?

**[📖 Installation Guide](docs/HOW-TO-INSTALL.md) • [🔧 API Documentation](docs/HOW-TO-ADD-METHOD.md) • [🐛 Report Issues](https://github.com/iThorgrim/mod-eclipse/issues) • [💬 Discussions](https://github.com/iThorgrim/mod-eclipse/discussions)**

*Built with ❤️ for the AzerothCore community*

---

⭐ **Star this repository if Eclipse powers your server!** ⭐

</div>
