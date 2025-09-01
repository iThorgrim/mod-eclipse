# 🚀 mod-eclipse Installation Guide

Welcome to the **mod-eclipse** installation guide! This comprehensive tutorial will walk you through installing the Eclipse Lua Engine for AzerothCore. Let's get your server powered by modern Lua! ⚡

## 📋 Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Installation](#quick-installation)
- [Detailed Steps](#detailed-steps)
- [Configuration Options](#configuration-options)
- [Troubleshooting](#troubleshooting)
- [What's Next](#whats-next)

## 🎯 Prerequisites

Before we start, make sure you have:

- ✅ **Git** installed on your system
- ✅ **CMake 3.16+** for building
- ✅ **C++17 compatible compiler**
- ✅ Basic knowledge of terminal/command line

> **💡 Pro Tip:** If you're new to AzerothCore, check out their [installation guide](https://www.azerothcore.org/wiki/installation) first!

## ⚡ Quick Installation

Already have AzerothCore? Here's the express lane:

```bash
# Navigate to your modules folder
cd azerothcore-wotlk/modules

# Clone mod-eclipse
git clone https://github.com/iThorgrim/mod-eclipse.git

# Apply the CMake patch
git apply mod-eclipse/cmake-patch.diff

# Add Lua version to your cmake build
# In your build folder:
cmake .. -DLUA_VERSION=lua54 [your other cmake flags]
```

Done! 🎉 Skip to [What's Next](#whats-next) if everything worked.

## 🔧 Detailed Steps

### Step 1: Clone AzerothCore 📥

> **📝 Note:** Already have AzerothCore? Skip to Step 2!

First, let's get the AzerothCore source code:

```bash
git clone https://github.com/azerothcore/azerothcore-wotlk.git
cd azerothcore-wotlk
```

This might take a few minutes depending on your internet connection. ☕

### Step 2: Navigate to Modules 📂

```bash
cd modules
```

This is where all AzerothCore modules live. Think of it as the plugin directory!

### Step 3: Clone mod-eclipse 🌟

```bash
git clone https://github.com/iThorgrim/mod-eclipse.git
```

You should now see a new `mod-eclipse` folder in your modules directory.

### Step 4: Apply the CMake Patch 🔧

This is the **magic step** that makes everything work:

```bash
git apply mod-eclipse/cmake-patch.diff
```

**What does this do?** 🤔  
This patch replaces `ELUNA` references with `ECLIPSE` in your CMake configuration, ensuring mod-eclipse integrates properly with the build system.

**If the patch fails:**
```bash
# Check if there are any conflicts
git status
# If needed, manually edit CMakeLists.txt to replace ELUNA with ECLIPSE
```

### Step 5: Configure Your Build 🏗️

Navigate to your build directory:

```bash
cd ../build  # Go back to AzerothCore root, then to build folder
```

Add the Lua version parameter to your cmake command:

```bash
cmake .. -DLUA_VERSION=lua54 [your existing cmake parameters]
```

## ⚙️ Configuration Options

### Lua Version Selection

Choose your Lua adventure! 🎮

| Version | Status | Recommended |
|---------|--------|-------------|
| `lua51` | ✅ Supported | For legacy scripts |
| `lua52` | ✅ Supported | Balanced choice |
| `lua53` | ✅ Supported | Modern features |
| `lua54` | ✅ Supported | **Recommended** 🌟 |
| `luajit` | ✅ Supported | **High Performance** ⚡ |

**Example configurations:**

```bash
# For Lua 5.4 (recommended for features)
cmake .. -DLUA_VERSION=lua54

# For LuaJIT (recommended for performance)
cmake .. -DLUA_VERSION=luajit

# For Lua 5.2 (legacy compatibility)
cmake .. -DLUA_VERSION=lua52

# Debug build with Lua 5.3
cmake .. -DLUA_VERSION=lua53
```

## 🚨 Troubleshooting

### Common Issues and Solutions

#### ❌ "patch does not apply"
**Problem:** The CMake patch failed to apply.
```bash
# Check what's wrong
git status
git diff

# Manual fix: edit modules/CMakeLists.txt
# Replace all instances of "ELUNA" with "ECLIPSE"
```

#### ❌ "LUA_VERSION not found"
**Problem:** CMake can't find the specified Lua version.
```bash
# Make sure you're in the build directory
pwd  # Should end with /build
```

#### ❌ "Lua scripts not working"
**Problem:** Scripts aren't executing.
1. ✅ Check server logs for Eclipse loading messages
2. ✅ Verify Lua script syntax
3. ✅ Ensure scripts are in the correct directory
4. ✅ Check file permissions

### Getting Help 🆘

Still stuck? Here's where to get help:

- 📖 **Documentation:** Check the other guides in this folder
- 🐛 **Issues:** [GitHub Issues](https://github.com/iThorgrim/mod-eclipse/issues)
- 💬 **Community:** AzerothCore Discord server
- 📝 **Wiki:** AzerothCore community wiki

## 🎉 What's Next?

Congratulations! You've successfully installed mod-eclipse! 🎊

### Immediate Next Steps:

1. **📚 Read the Lua Binding Guide**
   ```
   docs/HOW-TO-ADD-METHOD.md
   ```
   Learn how to expose C++ methods to Lua

2. **🔍 Explore the Examples**
   ```
   lua_scripts/examples/
   ```
   See mod-eclipse in action

3. **⚙️ Configure Your Server**
   - Set up your Lua script directories
   - Configure Eclipse settings in etc/modules/mod_eclipse.conf
   - Test with a simple "Hello World" script

4. **🚀 Start Scripting!**
   ```lua
   -- Your first Eclipse script
   print("Hello Eclipse! 🌟")
   ```

### Advanced Topics:

- **🎯 Player Event Hooks** - React to player actions
- **🏰 Creature AI Scripting** - Create dynamic NPCs  
- **🌍 World Event Management** - Control server events
- **📊 Database Integration** - Store persistent data

### Best Practices:

- ✅ **Test scripts in development** before production
- ✅ **Use version control** for your Lua scripts
- ✅ **Document your custom functions** for team collaboration
- ✅ **Follow Eclipse coding conventions** for consistency
- ✅ **Monitor server performance** when adding scripts

## 📚 Additional Resources

- 🔗 **AzerothCore Wiki:** [azerothcore.org/wiki](https://www.azerothcore.org/wiki/)
- 🔗 **Lua 5.4 Manual:** [lua.org/manual/5.4/](https://www.lua.org/manual/5.4/)
- 🔗 **Sol2 Documentation:** [sol2.rtfd.io](https://sol2.readthedocs.io/)
- 🔗 **Eclipse GitHub:** [github.com/iThorgrim/mod-eclipse](https://github.com/iThorgrim/mod-eclipse)

---

**Happy scripting with mod-eclipse!** 🚀✨

*Need help? Found a bug? Want to contribute? Check out our GitHub repository and join the community!*