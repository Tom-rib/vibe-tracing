# 🎨 Raytracer - C Implementation

Un **raytracer CPU haute-performance en C**, construit progressivement avec un agent IA en utilisant la méthodologie du **vibe coding**.

**Status** : 🚀 Phase 1 - Préparation  
**Langage** : C99/C11  
**Compilation** : GCC avec optimisations agressives (`-O3 -march=native`)  
**Output** : PPM (Portable Pixmap)

---

## 🎯 Objectif

Implémenter un raytracer complet avec :
- ✅ Raycasting basique
- ✅ Lumières et shading (Lambert, Phong)
- ✅ Réflexions et réfractions récursives
- ✅ Géométrie avancée (sphères, plans, triangles, meshes)
- ✅ Structures d'accélération (BVH)
- ✅ Multithreading (pthreads)
- ✅ Anti-aliasing et Depth of Field
- ✅ Textures et environnement mapping
- ✅ Optimisations SIMD

---

## 🚀 Quick Start

### Prérequis

```bash
# Linux/macOS
gcc --version  # GCC 10+ ou Clang
make --version # GNU Make
```

### Compilation & Exécution

```bash
# Build optimisé
make clean && make

# Exécuter
make run
```

Cela génère `output.ppm` dans le répertoire courant.

### Visualiser l'image

```bash
# Linux
feh output.ppm

# macOS
open output.ppm

# Ou n'importe quel viewer PPM
```

---

## 📚 Documentation

Trois documents principaux :

1. **[SPECIFICATION.md](./SPECIFICATION.md)** - Spécification technique complète
   - Détails de chaque phase (1-7)
   - Structures de données clés
   - Critères de succès
   - Git strategy

2. **[COPILOT.md](./COPILOT.md)** - Stratégie de vibe coding
   - Workflow avec l'agent IA
   - Prompts structurés par phase
   - Checklist avant chaque commit
   - Token budget strategy

3. **[PROJECT_STRUCTURE.md](./PROJECT_STRUCTURE.md)** - Architecture du projet
   - Arborescence complète
   - Responsabilités des modules
   - Conventions de nommage
   - Ordre de développement

---

## 🏗️ Architecture

```
src/
├── math/           # vec3 et opérations vectorielles
├── core/           # ray, camera, image, material
├── primitives/     # sphere, plane, triangle, mesh
├── scene/          # gestion scene (objets + lumières)
├── lighting/       # calculs d'illumination
├── acceleration/   # AABB, BVH
├── renderer/       # boucle rendu principale
├── io/             # parseurs, I/O
└── utils/          # alloc, RNG, timing
```

---

## 📊 Phases de développement

| Phase | Contenu | Commits |
|-------|---------|---------|
| 1 | Math + Ray + Sphere | 6 |
| 2 | Lumières + Shading | 4 |
| 3 | Réflexions + Réfractions | 3 |
| 4 | Géométrie avancée | 3 |
| 5 | Performance (BVH, MT) | 5 |
| 6 | Features avancées | 7 |
| 7 | SIMD + Optimisations | 3 |
| **Total** | **Raytracer complet** | **~31 commits** |

---

## 💻 Compilation Détails

### Flags GCC

```bash
-O3                # Optimisations max
-march=native      # Instructions CPU natives
-std=c99           # Standard C99
-Wall -Wextra      # Warnings stricts
-ffast-math        # Math rapides (approximations OK)
-lm                # Math library
-lpthread          # Pthreads pour multithreading
```

### Debug

```bash
make debug  # Build avec symboles de debug
gdb ./build/bin/raytracer
```

### Profiling

```bash
make profile-run  # Profile avec gprof
cat profile.txt
```

---

## 🎬 Workflow Vibe Coding

1. **Lire** `SPECIFICATION.md` et `PROJECT_STRUCTURE.md`
2. **Identifier** la phase courante
3. **Rédiger prompt** pour l'agent IA (voir `COPILOT.md`)
4. **Valider** code et tests
5. **Committer** : `git commit -m "feat: description"`
6. **Répéter** pour chaque feature

---

## 📈 Progression attendue

```
Phase 1 : Sphère blanche sur fond noir
    ↓
Phase 2 : Sphères illuminées avec shadows
    ↓
Phase 3 : Réflexions et réfractions visibles
    ↓
Phase 4 : Meshes OBJ chargés et rendus
    ↓
Phase 5 : Rendu rapide avec BVH + MT
    ↓
Phase 6 : Textures, DoF, soft shadows
    ↓
Phase 7 : Optimisations SIMD
```

---

## 🎨 Exemple usage attendu (Phase 1)

```c
// src/main.c
#include "src/core/camera.h"
#include "src/core/image.h"
#include "src/primitives/sphere.h"

int main() {
    // Créer caméra et image
    camera cam = camera_create(800, 600, 90.0f);
    image img = image_create(800, 600);
    
    // Créer scène simple
    sphere s = {
        .center = {0, 0, -5, 0},
        .radius = 1.0f,
        .mat = {{1, 1, 1, 0}, 0.5f, 0, 1.5f}
    };
    
    // Rendu pixel par pixel
    for (int y = 0; y < 600; y++) {
        for (int x = 0; x < 800; x++) {
            ray r = camera_ray(&cam, x, y);
            // ... raytracing ...
            img.pixels[y * 800 + x] = color_to_uint32(result);
        }
    }
    
    // Sauvegarder
    image_write_ppm(&img, "output.ppm");
    return 0;
}
```

---

## 🔧 Troubleshooting

### Compilation échoue
```bash
# Vérifier GCC
gcc --version

# Nettoyer et rebuild
make clean
make -j4  # Compilation parallèle
```

### Segfault au runtime
```bash
# Build avec debug symbols
make debug

# Lancer avec GDB
gdb ./build/bin/raytracer
(gdb) run
```

### Image noire
- [ ] Camera bien orientée vers la scène ?
- [ ] Sphères en avant de la caméra ?
- [ ] PPM writer correct ?

---

## 📖 Ressources

- **C99/C11 Standard** : https://en.cppreference.com/w/c
- **Raytracing Theory** : "Ray Tracing in One Weekend" by Peter Shirley
- **SIMD** : Intel Intrinsics Guide
- **Profiling** : gprof/perf documentation

---

## 📝 Git Workflow

```bash
# Démarrer une feature
git checkout -b feat/vector-math

# ... code ...

# Valider et committer
git add src/math/vec3.*
git commit -m "feat: add vector math module"

# Merger
git checkout main
git merge feat/vector-math

# Ou directement
git commit -m "feat: add vector math module"
```

---

## 🎓 Compétences visées

- **Vibe Coding** : Collaboration efficace avec agents IA
- **Prompting** : Rédiger des prompts clairs et structurés
- **C Performance** : Optimisations, SIMD, multithreading
- **Architecture** : Découpage modulaire et maintenable
- **Ray Tracing** : Algorithmes et implémentation

---

## ✅ Checklist pré-commit

- [ ] Code compile sans warnings
- [ ] Tests validés
- [ ] Git log clair
- [ ] Pas d'allocation inutile
- [ ] Performance validée

---

## 📞 Support

Pour clarifications ou blocages, voir :
- `SPECIFICATION.md` - Source de vérité technique
- `COPILOT.md` - Stratégie de développement
- `PROJECT_STRUCTURE.md` - Architecture et conventions

---

**Happy vibe coding! 🚀**

*"Premium Is Scarce. Skill Is Not."* - Claude
