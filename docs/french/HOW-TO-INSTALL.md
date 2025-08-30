# Comment installer mod-eclipse

Bienvenue sur ce guide qui a pour but de vous apprendre à installer **mod-eclipse**.

> [!NOTE]
> Si vous avez déjà cloné AzerothCore, passez directement à l’étape 2.

### Étapes d'installation

1. **Cloner AzerothCore**
```bash
git clone https://github.com/azerothcore/azerothcore-wotlk.git
```

2. **Aller dans le dossier `modules`**
```bash
cd azerothcore-wotlk/modules
```

3. **Cloner mod-eclipse**
```bash
git clone https://github.com/iThorgrim/mod-eclipse.git
```

4. **Appliquer le patch CMake**
```bash
git apply mod-eclipse/cmake-patch.diff
```

Ce patch permet de remplacer `ELUNA` par `ECLIPSE` dans le fichier `CMakeLists.txt` de votre dossier modules, ce qui est nécessaire pour que **mod-eclipse** fonctionne correctement.

5. **Configurer la compilation**
Rendez-vous dans le dossier `build` (à la racine du projet AzerothCore), puis ajoutez le paramètre suivant à votre commande cmake :
```bash
-DLUA_VERSION=lua54
```

> [!IMPORTANT]
> Vous pouvez choisir d’autres versions de Lua (`lua51` à `lua54`). 
> **Cependant, LuaJIT n’est pas encore supporté.**
> Merci de ne pas créer d’issue concernant LuaJIT tant qu’il n’est pas officiellement pris en charge.
