# 🌙 Eclipse - Modern Lua Engine for AzerothCore

[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![AzerothCore](https://img.shields.io/badge/AzerothCore-Compatible-brightgreen.svg)](https://www.azerothcore.org/)
[![Status](https://img.shields.io/badge/Status-POC-orange.svg)](https://github.com/your-repo/mod-eclipse)

## 📁 Project Structure

```
mod-eclipse/
├── src/
│   ├── LuaEngine/           # Core engine components
│   │   ├── LuaEngine.cpp    # Main engine orchestrator
│   │   ├── LuaManager.cpp   # Thread-safe singleton
│   │   ├── LuaBindings.cpp  # Lua ↔ C++ bindings
│   │   ├── LuaCache.cpp     # Performance caching system
│   │   └── ScriptLoader.cpp # Script management
│   ├── EclipseScript.cpp    # AzerothCore integration
│   ├── EclipseLoader.cpp    # Module entry point
│   └── sol/                 # Sol3 header-only library
└── README.md
```

## 🚀 Quick Start

### Installation

1. Clone this module to your AzerothCore modules directory:
```bash
cd azerothcore-wotlk/modules/
git clone https://github.com/your-repo/mod-eclipse.git
```

2. Recompile AzerothCore:
```bash
cd build
make -j$(nproc)
```

## 🎯 Current Implementation Status

### ✅ Completed Features
- [x] Sol3 integration with header-only library
- [x] Thread-safe singleton pattern
- [x] Automatic script loading from directory
- [x] Function caching for performance
- [x] LuaJIT optimizations
- [x] Modular architecture with separated concerns
- [x] Basic Lua bindings (logging functions)
- [x] In-game reload command
- [x] Comprehensive error handling

### 🚧 In Development
- [ ] Hot reloading system
- [ ] AzerothCore game object bindings (Player, Creature, etc.)
- [ ] Event system integration
- [ ] Configuration file support

### 🗓️ Planned Features
- [ ] Performance profiling tools
- [ ] Automatic script compilation

## 🔧 API Reference

### Core Functions

```lua
-- Logging
print(message)    -- Info level logging
log(message)      -- Info level logging
error(message)    -- Error level logging
debug(message)    -- Debug level logging
warn(message)     -- Warning level logging

-- Engine Information
_ECLIPSE_LOADED   -- Boolean: true if Eclipse is active
_ECLIPSE_VERSION  -- String: current version
_ECLIPSE_OPTIMIZED -- Boolean: true if optimizations are active
```

### Performance Features

Eclipse automatically caches frequently called Lua functions and provides JIT compilation when LuaJIT is available, delivering significant performance improvements over traditional Lua engines.

## 🛠️ Development

### Building from Source

- C++17 compatible compiler
- AzerothCore development environment

### Contributing

This is currently a proof of concept. Contributions, feedback, and testing are welcome!

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📋 Requirements

- **AzerothCore** master branch
- **C++17** compiler (GCC 8+, Clang 7+, MSVC 2019+)
- **CMake** 3.16+
- **Lua 5.1+** or **LuaJIT** (optional, for performance)

## ⚠️ Known Limitations

- **POC Status**: Not production-ready
- **Limited Bindings**: Only basic logging functions currently implemented
- **No Eluna Compatibility**: Scripts need to be rewritten for Eclipse
- **Minimal Error Recovery**: Script errors may require engine restart

## 📄 License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **AzerothCore Team** - For the amazing WoW server core
- **Sol3** - For the excellent modern Lua binding library
- **Eluna** - For inspiring this modern approach to Lua scripting
- **Community** - For feedback and testing

---

<div align="center">

**[Documentation](docs/) • [Issues](issues/) • [Discussions](discussions/)**

*Built with ❤️ for the AzerothCore community*

</div>