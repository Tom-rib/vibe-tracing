# PROJECT_STRUCTURE.md - Architecture du Raytracer

## 📁 Arborescence complète

```
raytracer/
├── Makefile                    # Build system
├── .gitignore                  # Git ignore patterns
├── README.md                   # Intro projet
├── SPECIFICATION.md            # Spec technique (SOURCE OF TRUTH)
├── COPILOT.md                  # Stratégie vibe coding
├── PROJECT_STRUCTURE.md        # Ce fichier
├── SESSION.md                  # Résultats finaux (généré fin)
├── src/
│   ├── main.c                  # Point d'entrée
│   ├── config.h                # Constantes globales (résolution, max depth, etc.)
│   ├── math/
│   │   ├── vec3.h
│   │   └── vec3.c              # Vector3 operations (Phase 1.1)
│   ├── core/
│   │   ├── ray.h
│   │   ├── ray.c               # Ray structure (Phase 1.2)
│   │   ├── camera.h
│   │   ├── camera.c            # Pinhole camera (Phase 1.3)
│   │   ├── image.h
│   │   ├── image.c             # Image buffer + PPM output (Phase 1.4)
│   │   ├── material.h          # Material definition
│   │   └── color.h             # Color utilities
│   ├── primitives/
│   │   ├── sphere.h
│   │   ├── sphere.c            # Sphere intersection (Phase 1.5)
│   │   ├── plane.h
│   │   ├── plane.c             # Plane intersection (Phase 4.1)
│   │   ├── triangle.h
│   │   ├── triangle.c          # Triangle intersection (Phase 4.2)
│   │   ├── mesh.h
│   │   └── mesh.c              # Mesh/OBJ loading (Phase 4.3)
│   ├── scene/
│   │   ├── scene.h
│   │   └── scene.c             # Scene management (objects + lights)
│   ├── lighting/
│   │   ├── lighting.h
│   │   ├── lighting.c          # Lighting calculations (Phase 2)
│   │   ├── light.h             # Light structure
│   │   └── light.c
│   ├── acceleration/
│   │   ├── aabb.h
│   │   ├── aabb.c              # AABB structure (Phase 5.1)
│   │   ├── bvh.h
│   │   └── bvh.c               # BVH tree (Phase 5.2)
│   ├── renderer/
│   │   ├── renderer.h
│   │   ├── renderer.c          # Main render loop
│   │   ├── raycaster.h
│   │   ├── raycaster.c         # Ray-scene intersection
│   │   ├── sampler.h
│   │   └── sampler.c           # Sampling strategies (AA, DoF, etc)
│   ├── io/
│   │   ├── obj_loader.h
│   │   └── obj_loader.c        # OBJ parser (Phase 4.3)
│   ├── utils/
│   │   ├── allocator.h
│   │   ├── allocator.c         # Memory pooling (Phase 5.5)
│   │   ├── random.h
│   │   └── random.c            # RNG utilities
│   ├── textures/               # [Phase 6 optional]
│   │   ├── texture.h
│   │   ├── texture.c           # Texture sampling
│   │   ├── procedural.h
│   │   └── procedural.c        # Procedural textures
│   └── environment/            # [Phase 6 optional]
│       ├── environment.h
│       └── environment.c       # Skybox / environment mapping
├── data/
│   ├── reference.ppm           # Image de référence initiale
│   ├── models/
│   │   ├── bunny.obj           # Test mesh (petit)
│   │   └── teapot.obj          # Test mesh (moyen)
│   └── textures/               # [Phase 6+]
│       ├── checkerboard.ppm
│       └── environment.hdr
├── build/
│   └── (artefacts compilés)
└── tests/                      # [Bonus]
    ├── test_vec3.c
    ├── test_ray.c
    └── run_tests.sh

```

---

## 📄 Fichiers clés par phase

### Phase 1 : Base Math & Raycasting

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/math/vec3.h/c` | Phase 1.1 | Aucune |
| `src/core/ray.h/c` | Phase 1.2 | vec3 |
| `src/core/camera.h/c` | Phase 1.3 | ray, vec3 |
| `src/core/image.h/c` | Phase 1.4 | Aucune (I/O) |
| `src/primitives/sphere.h/c` | Phase 1.5 | ray, vec3 |
| `src/main.c` (minimal flat shade) | Phase 1.6 | Tous Phase 1 |

### Phase 2 : Lumière & Shading

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/core/material.h` | Phase 2.0 | Aucune |
| `src/lighting/light.h/c` | Phase 2.1 | vec3 |
| `src/lighting/lighting.c` | Phase 2.2-2.4 | light, material, ray |
| `src/scene/scene.h/c` | Phase 2.0 | lights, spheres |

### Phase 3 : Réflexion & Réfraction

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/renderer/raycaster.c` | Phase 3.1-3.3 | scene, lighting |

### Phase 4 : Géométrie Avancée

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/primitives/plane.h/c` | Phase 4.1 | ray, vec3 |
| `src/primitives/triangle.h/c` | Phase 4.2 | ray, vec3 |
| `src/io/obj_loader.h/c` | Phase 4.3 | mesh, triangle |

### Phase 5 : Performance

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/acceleration/aabb.h/c` | Phase 5.1 | vec3 |
| `src/acceleration/bvh.h/c` | Phase 5.2 | aabb, scene |
| `src/renderer/renderer.c` | Phase 5.3 | pthreads |
| `src/utils/allocator.h/c` | Phase 5.5 | Aucune |

### Phase 6 : Features Avancées

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/renderer/sampler.h/c` | Phase 6.1-6.2 | random, ray |
| `src/textures/*` | Phase 6.4-6.5 | Optional |
| `src/environment/*` | Phase 6.6 | Optional |

### Phase 7 : SIMD & Optimization

| Fichier | Statut | Dépendances |
|---------|--------|-------------|
| `src/math/vec3_simd.c` | Phase 7.1 | vec3.h (refactor) |

---

## 🔗 Dépendances Inter-modules

```
main.c
  ├→ renderer.h
  │   ├→ raycaster.h
  │   │   ├→ scene.h
  │   │   │   ├→ sphere.h
  │   │   │   ├→ plane.h
  │   │   │   ├→ triangle.h
  │   │   │   └→ light.h
  │   │   ├→ lighting.h
  │   │   └→ acceleration/
  │   │       ├→ aabb.h
  │   │       └→ bvh.h
  │   └→ sampler.h
  ├→ camera.h
  ├→ image.h
  ├→ obj_loader.h [Phase 4+]
  └→ utils/ (allocator, random)

Patterns :
- Aucune dépendance circulaire
- Modules indépendants (math, io, utils)
- Core = base pour primitives
- Renderer = orchestrateur principal
```

---

## 📝 Conventions de nommage

### Fichiers
- Header : `modulename.h`
- Source : `modulename.c`
- Tests : `test_modulename.c`

### Structures
```c
// Nom = type_noun (ex: sphere, plane, mesh)
typedef struct {
    vec3 center;
    float radius;
    material mat;
} sphere;

// Typedef'd
typedef struct sphere sphere;
```

### Fonctions
```c
// Nom = module_verb_object (ex: sphere_intersect, vec3_normalize)
bool sphere_intersect(const ray *r, const sphere *s, hit_record *out);
vec3 vec3_normalize(vec3 v);

// Getters/setters pas nécessaires pour structs simples
// Alloc = module_create, Free = module_destroy
sphere* sphere_create(vec3 center, float radius);
void sphere_destroy(sphere *s);
```

### Macros
```c
// UPPERCASE_WITH_UNDERSCORES
#define MAX_DEPTH 10
#define VEC3_ZERO { 0.0f, 0.0f, 0.0f, 0.0f }
```

### Variables globales
```c
// static + descriptive name
static int g_num_spheres = 0;
static sphere g_spheres[MAX_OBJECTS];
```

---

## 🏗️ Config.h - Constantes globales

```c
// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Rendu
#define IMAGE_WIDTH         800
#define IMAGE_HEIGHT        600
#define MAX_DEPTH           5
#define NUM_THREADS         4

// Rendu avancé
#define NUM_AA_SAMPLES      4
#define NUM_LIGHT_SAMPLES   1    // Phase 6+
#define ADAPTIVE_THRESHOLD  0.01 // Phase 7+

// Scene
#define MAX_OBJECTS         1000
#define MAX_LIGHTS          16
#define MAX_TRIANGLES       1000000

// Performance
#define BVH_MAX_DEPTH       32
#define USE_BVH             1
#define USE_SIMD            0    // Phase 7+

#endif
```

---

## 🏢 Module Responsibilities

### math/
- ✅ vec3 operations (add, dot, cross, normalize, etc.)
- ✅ Inline pour performance
- ❌ Pas d'allocation
- ❌ Pas de I/O

### core/
- ✅ Ray, Camera, Image, Material, Color
- ✅ Structures de base
- ❌ Pas de rendu
- ❌ Pas de mathématiques spéciales

### primitives/
- ✅ Sphere, Plane, Triangle, Mesh
- ✅ Fonctions intersect(ray, primitive) → hit_record
- ❌ Pas de shading
- ❌ Pas d'accélération

### scene/
- ✅ Container des objets (spheres, planes, triangles)
- ✅ Container des lumières
- ✅ Fonction scene_intersect(ray) → closest hit_record
- ❌ Pas de rendu directement

### lighting/
- ✅ Calculs d'illumination (Lambert, Phong, Fresnel)
- ✅ Shadow rays
- ❌ Pas d'allocation
- ❌ Pas de gestion scene (reçoit en paramètre)

### acceleration/
- ✅ AABB, BVH (structure et build)
- ✅ aabb_intersect(ray) → bool
- ✅ bvh_find_intersection(ray) → hit_record
- ❌ Pas d'alloc dynamique si possible (pool?)

### renderer/
- ✅ Main render loop
- ✅ Raycasting (ray-tracing principal)
- ✅ Sampler (AA, DoF, etc.)
- ❌ Pas d'I/O (sauf final image write)

### io/
- ✅ OBJ parser
- ✅ PPM writer
- ❌ Pas de logique rendering

### utils/
- ✅ Memory pooling
- ✅ RNG
- ✅ Timing/profiling
- ❌ Pas de structures specifiques au raytracer

---

## 🔨 Makefile Structure

```makefile
CC = gcc
CFLAGS = -O3 -march=native -std=c99 -Wall -Wextra
LDFLAGS = -lm -lpthread

SRCDIR = src
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin

# Sources
SOURCES = $(wildcard $(SRCDIR)/**/*.c) $(SRCDIR)/main.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/raytracer

# Targets
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

profile: CFLAGS += -pg
profile: clean all
	./$(TARGET) output.ppm
	gprof ./$(TARGET) gmon.out > profile.txt

run: all
	./$(TARGET) output.ppm
	@echo "Image saved to output.ppm"

.PHONY: all clean profile run
```

---

## 📊 Include Organization

### pattern bien organisé
```c
// Fichier : src/renderer/raycaster.c

// Standard C
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Project math (aucune dépendance)
#include "../math/vec3.h"

// Core (dépend math)
#include "../core/ray.h"
#include "../core/camera.h"
#include "../core/material.h"

// Primitives (dépend core)
#include "../primitives/sphere.h"
#include "../primitives/plane.h"

// Scene
#include "../scene/scene.h"

// Lighting
#include "../lighting/lighting.h"

// Acceleration
#include "../acceleration/bvh.h"

// Own module
#include "raycaster.h"
```

---

## ✅ Validation Phase par Phase

Après chaque phase, vérifier :

1. **Compilation** : `make clean && make` sans warnings
2. **Linker** : Pas d'undefined symbols
3. **Runtime** : Pas de segfault
4. **Output** : Image PPM valide et visible
5. **Git** : Commit propre et lisible

### Phase 1 - Image rose simple
- [ ] main.c lance rayon pour chaque pixel
- [ ] Sphère blanche au centre
- [ ] Fond noir
- [ ] PPM sauvegardé correctement

### Phase 2 - Lumières et shadows
- [ ] Sphère illuminée (Lambert visible)
- [ ] Specular highlights (Phong)
- [ ] Shadows sombres derrière objets

### Phase 3 - Réflexion
- [ ] Sphère miroir visible dans autre sphère
- [ ] Profondeur contrôlée

### Phase 4 - Meshes
- [ ] Mesh OBJ chargeable sans erreur
- [ ] Triangles hittables et visibles

### Phase 5 - Performance
- [ ] BVH < 2× temps brute-force
- [ ] Multithreading 2-4× speedup

---

## 🎯 Ordre de développement recommandé

**Strict order** :
1. `src/math/vec3.*`
2. `src/core/ray.*`
3. `src/core/camera.*`
4. `src/core/image.*`
5. `src/primitives/sphere.*`
6. `src/main.c` (Phase 1.6)
7. `src/core/material.h` + `src/lighting/light.*`
8. `src/scene/scene.*`
9. `src/lighting/lighting.*`
10. ... (suite selon phases)

**Pas d'ordre pour** (indépendants) :
- `src/core/color.h`
- `src/config.h`
- `src/utils/*`

---

## 📦 Après Phase 1 : Structure minimale compilable

```
src/
├── main.c
├── config.h
├── math/
│   ├── vec3.h
│   └── vec3.c
├── core/
│   ├── ray.h
│   ├── ray.c
│   ├── camera.h
│   ├── camera.c
│   ├── image.h
│   ├── image.c
│   └── color.h
└── primitives/
    ├── sphere.h
    └── sphere.c

Makefile
.gitignore
README.md
SPECIFICATION.md
COPILOT.md
PROJECT_STRUCTURE.md
```

Ce minimal compila et génère une image PPM ! ✅

---

Enjoy la structure ! 🏗️
