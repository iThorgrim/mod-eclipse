<div align="center">

# Eclipse - Lua Engine for AzerothCore

*Modern Lua scripting engine for AzerothCore*

[![Discord](https://img.shields.io/badge/Discord-Join%20Us-7289DA?style=for-the-badge&logo=discord&logoColor=white)](https://discord.com/invite/ZKSVREE7)
[![Lua](https://img.shields.io/badge/Lua-5.4-2C2D72?style=for-the-badge&logo=lua&logoColor=white)](http://www.lua.org/manual/5.4/)
[![AzerothCore](https://img.shields.io/badge/AzerothCore-Compatible-darkgreen?style=for-the-badge)](http://www.azerothcore.org/)

---
</div>

> [!IMPORTANT]  
> Eclipse is a **modern Lua scripting engine** designed for AzerothCore.  
> Built with **Sol2**, it offers caching, hot reload, multi-state architecture, and full API integration.

## 🚀 Overview

Eclipse enables developers and administrators to extend AzerothCore with Lua scripts in a clean, modular, and production-ready way.

### ✨ Key Features
- 📂 **Caching system** with timestamp validation
- 🔄 **Hot reload** without server restart  
- 🧩 **Multi-state architecture** (global + per-map)  
- 📁 **Dynamic path management** for `require()`  
- 🛡️ **Error handling** with safe recovery  
- 🧵 **Thread-safe messaging**  
- ⚙️ **ConfigValueCache integration**  
- 🎮 **In-game commands** via `.eclipse`  
- 🔗 **Extensive API bindings** (Player, Creature, GameObject, etc.)  

---

## 📂 Architecture Overview

```
mod-eclipse/
├── src/
│   ├── LuaEngine/
│   │   ├── Core/
│   │   ├── Scripting/
│   │   ├── Utils/
│   │   └── Methods/
│   └── sol/
├── docs/
└── lua_scripts/examples/
```

---

## ⚡ Installation

```bash
cd azerothcore-wotlk/modules/
git clone https://github.com/iThorgrim/mod-eclipse.git
cd ../build
cmake .. -DLUA_VERSION=luajit
make -j$(nproc)
```

➡️ See the [Installation Guide](docs/HOW-TO-INSTALL.md) for details.

---

## 📝 First Script

Create `lua_scripts/hello_eclipse.lua`:

```lua
print("Hello Eclipse! Lua scripting is now enabled.")
```

---

## ✅ Implementation Status

- 🧩 Multi-state architecture  
- 📂 Timestamp-based caching with hot reload
- ⚙️ ConfigValueCache integration  
- 🔗 Sol2 integration  
- 📁 Dynamic path management  
- 🧵 Thread-safe messaging  
- 🛡️ Error handling with recovery  
- 🎮 `.eclipse` command suite  
- 👤 Player, Creature, GameObject API bindings  

---

## 🔧 Configuration

`mod_eclipse.conf`:

```ini
Eclipse.Enabled = 1
Eclipse.Compatibility = false
Eclipse.ScriptPath = "lua_scripts"
Eclipse.RequirePaths = ""
Eclipse.RequireCPaths = ""
```

---

## 🎮 In-Game Commands

| Command           | Description        |
|-------------------|--------------------|
| `.eclipse reload` | Reload all scripts |

---

## 📚 Documentation

- [Installation Guide](docs/HOW-TO-INSTALL.md)  
- [Method Binding Tutorial](docs/HOW-TO-ADD-METHOD.md)  
- [Example Scripts](lua_scripts/examples/)  

---

## 🤝 Contributing

1. Fork the repository  
2. Create a branch  
3. Commit changes  
4. Push branch  
5. Open a Pull Request  

---

## 🌟 Acknowledgments

- AzerothCore team  
- Sol2 project  
- Eluna project  
- Community contributors  

---

<div align="center">
⭐ Star this repository if Eclipse powers your server ⭐
</div>
