# GETTING_STARTED.md - Démarrage en 5 minutes

Bienvenue dans le projet Raytracer ! Voici comment démarrer **immédiatement**.

## ✅ Étape 1 : Vérifier les prérequis

```bash
gcc --version        # GCC 10+
make --version       # GNU Make
```

## 🏗️ Étape 2 : Compiler

```bash
cd raytracer_c

# Option A : Avec Make (recommandé)
make clean && make

# Option B : Avec le script build.sh
./build.sh all

# Option C : Avec Makefile directement
make run    # Build + exécute d'un coup
```

## 🧪 Étape 3 : Tester

### Tests unitaires (Phase 1)
```bash
./test.sh phase1
```

Output attendu:
```
✓ Test vec3_create passed
✓ Test vec3_add passed
✓ Test sphere_intersect passed
... (plus de tests)
Summary: 20+ tests passed
```

### Rendu (Phase 1 complet)
```bash
./build/bin/raytracer output.ppm
```

Output attendu:
```
=== Raytracer - Phase 1 ===
Creating image (800 × 600)...
Setting up camera (FOV 90.0°)...
Setting up scene...
Rendering 800 × 600 pixels...
100 / 480000 pixels done (0.0%)
...
Render time: 1.23 seconds
```

### Visualiser l'image
```bash
# Linux
feh output.ppm

# macOS
open output.ppm

# Ou n'importe quel viewer
```

**Image attendue** : Sphère blanche/grise au centre, fond noir.

---

## 🎯 Résultat Phase 1

Si vous avez une image `.ppm` avec une sphère shaded : **Succès ! Phase 1 est complète.** 🎉

```
output.ppm
├─ 800×600 pixels
├─ Sphère blanche au centre
├─ Fond noir
└─ ✓ Compilable et renderable
```

---

## 📁 Structure créée

```
raytracer_c/
├── src/
│   ├── main.c                    # Point d'entrée (Phase 1)
│   ├── config.h                  # Constantes globales
│   ├── math/
│   │   ├── vec3.h               # ✓ Vector math
│   │   └── vec3.c
│   ├── core/
│   │   ├── ray.h                # ✓ Ray structure
│   │   ├── ray.c
│   │   ├── camera.h             # ✓ Pinhole camera
│   │   ├── camera.c
│   │   ├── image.h              # ✓ PPM output
│   │   ├── image.c
│   │   ├── color.h              # ✓ Color utilities
│   │   └── material.h           # ✓ Material properties
│   ├── primitives/
│   │   ├── sphere.h             # ✓ Sphere intersection
│   │   └── sphere.c
│   └── utils/
│       ├── allocator.h/c        # Memory pooling (Phase 5+)
│       └── random.h/c           # RNG (Phase 6+)
├── Makefile                      # Build system
├── build.sh                      # Build script
├── test.sh                       # Test script
├── test_phase1.c                 # Unit tests
├── .gitignore
├── README.md                     # Intro projet
├── SPECIFICATION.md              # Spec technique ⭐
├── COPILOT.md                    # Vibe coding guide ⭐
├── PROJECT_STRUCTURE.md          # Architecture ⭐
├── PHASE_1_QUICKSTART.md         # Phase 1 details ⭐
├── GIT_WORKFLOW.md               # Git strategy
└── GETTING_STARTED.md            # Ce fichier
```

**✓ = Fichiers Phase 1 créés et compilables**

---

## 🔧 Troubleshooting

### Compilation échoue: "undefined reference to `sqrt`"
```bash
# Solution: -lm flag manquant
# Vérifier Makefile ou build.sh incluent -lm
make clean && make
```

### PPM ne s'ouvre pas
```bash
# Vérifier le fichier
file output.ppm
# Doit être: PPM image data, width 800, height 600, maxval 255

# Ouvrir avec un viewer spécifique
hexdump -C output.ppm | head  # Vérifier header "P6"
```

### Image complètement noire
- Vérifier que sphere est à z=-5 et camera à z=0
- Vérifier que ray_at() fonctionne (voir tests)
- Vérifier que intersection ray-sphere fonctionne (voir tests)

---

## 📚 Prochaines étapes

Après Phase 1 réussie :

1. **Lire PHASE_1_QUICKSTART.md** pour détails
2. **Lire COPILOT.md** pour prompts Phase 2
3. **Phase 2** : Ajouter lumières et shading

```bash
# Phase 2 commencera par :
# feat: add point light support
# feat: implement lambert diffuse shading
# feat: add phong specular highlights
# feat: implement shadow rays
```

---

## 💡 Tips

- **Tester souvent** : `./test.sh phase1` avant chaque commit
- **Commiter régulièrement** : `git commit -m "feat: description"`
- **Visualiser l'image** : Vérifier la sortie visuellement à chaque phase
- **Lire COPILOT.md** : Prompts prêts à copier-coller pour l'agent IA

---

## 🎬 Exemple de session complète

```bash
# 1. Setup
cd raytracer_c
git init
git add .
git commit -m "init: project setup"

# 2. Test Phase 1
./test.sh phase1
# ✓ All tests passed

# 3. Render
make run
# ✓ Image saved to output.ppm

# 4. Visualize
feh output.ppm
# ✓ Sphère blanche visible

# 5. Commit
git add src/
git commit -m "feat: complete phase 1 raytracing"

# 6. Next: Phase 2
cat COPILOT.md | grep -A 20 "Phase 2"
```

---

## ✨ Succès !

Si vous avez :
- ✅ Compilé sans erreurs
- ✅ Tests Phase 1 passent
- ✅ Image .ppm générée avec sphère visible

**Vous êtes prêt pour Phase 2 !** 🚀

Questions ? Voir **SPECIFICATION.md**, **COPILOT.md**, ou **PROJECT_STRUCTURE.md**.

Happy raytracing! 🎨
