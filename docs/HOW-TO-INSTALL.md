# How to install mod-eclipse

Welcome to this guide, which will help you install **mod-eclipse**.

> [!NOTE]
> If you have already cloned AzerothCore, please skip to the step 2.

### Installation Steps

1. **Clone AzerothCore**
```bash
git clone https://github.com/azerothcore/azerothcore-wotlk.git
```

2. **Go to the `modules` folder**
```bash
cd azerothcore-wotlk/modules
```

3. **Clone mod-eclipse**
```bash
git clone https://github.com/iThorgrim/mod-eclipse.git
```

4. **Apply the CMake patch**
```bash
git apply mod-eclipse/cmake-patch.diff
```
This patch replaces `ELUNA` with `ECLIPSE` in the `CMakeLists.txt` file of your modules folder, which is required for **mod-eclipse** to work properly.

5. **Configure the build**

Go to the `build` folder (at the root of your AzerothCore source), then add the following parameter to your cmake command:

```bash
-DLUA_VERSION=lua54
```

> [!IMPORTANT]
> You can choose different Lua versions (`lua51` to `lua54`). **However, LuaJIT is not yet supported.** Please do not create an issue regarding LuaJIT until it is officially supported.
