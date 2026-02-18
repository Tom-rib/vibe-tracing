# SPECIFICATION - Raytracer CPU en C

## 1. Vue d'ensemble

**Objectif** : Développer un raytracer haute performance en C, modulaire et évolutif, avec support des features avancées (réflexion, réfraction, soft shadows, BVH, multithreading).

**Langage** : C99/C11  
**Compilateur** : GCC avec optimisations agressives (`-O3 -march=native -Wall -Wextra`)  
**Output** : PPM (Portable Pixmap)  

---

## 2. Architecture modulaire

```
src/
├── math/              # Bibliothèque mathématique vectorielle
├── core/              # Structures de base (Ray, Color, Image)
├── scene/             # Gestion de la scène (objets, lumières)
├── primitives/        # Sphère, plan, triangle, mesh
├── lighting/          # Calculs d'illumination (Lambert, Phong)
├── acceleration/      # BVH et AABB
├── renderer/          # Boucle de rendu, antialiasing
├── io/                # Parseurs (OBJ), sauvegarde (PPM)
├── utils/             # Alloc, RNG, profiling
└── main.c             # Point d'entrée
```

---

## 3. Features par Phase

### Phase 1 : Base Math & Raycasting (commits 1-6)

#### 1.1 Vector Math Library
- Structure `vec3` alignée 16 bytes
- Operations : add, subtract, dot, cross, normalize, length
- Macros `inline` pour performance
- **Commit** : `feat: add vector math module`

#### 1.2 Ray Structure
- `struct ray { vec3 origin, direction; }`
- Fonction `ray_at(t)` → position sur le rayon
- **Commit** : `feat: add ray structure`

#### 1.3 Camera (Pinhole)
- Position, direction, FOV
- Génération de rayons pour chaque pixel
- **Commit** : `feat: implement basic camera`

#### 1.4 Image Output (PPM)
- Génération fichier PPM P6 (binary)
- Conversion float [0,1] → uint8 [0,255]
- **Commit** : `feat: add PPM image output`

#### 1.5 Sphere Intersection
- Intersection analytique ray-sphère
- Retour du t minimum positif
- Normal calculée analytiquement
- **Commit** : `feat: implement sphere intersection`

#### 1.6 Flat Shading
- Normales + couleur fixe
- **Commit** : `feat: add flat shading`

---

### Phase 2 : Lumière & Shading (commits 7-10)

#### 2.1 Point Lights
- Position, couleur (RGB), intensité
- Tableau de lumières dans la scène
- **Commit** : `feat: add point light`

#### 2.2 Lambert Diffuse
- `cos(theta)` clampé [0,1]
- Interaction couleur objet × couleur lumière
- **Commit** : `feat: implement lambert shading`

#### 2.3 Phong/Blinn-Phong Specular
- Vecteur vue, réflexion, shininess
- **Commit** : `feat: add specular highlights`

#### 2.4 Shadow Rays
- Rayons depuis surface vers lumière
- Test d'occlusion (hard shadows)
- **Commit** : `feat: implement shadow rays`

---

### Phase 3 : Réflexion & Réfraction (commits 11-13)

#### 3.1 Recursive Reflections
- Direction réfléchie = reflect(ray_dir, normal)
- Profondeur max configurable (par défaut 5)
- Accumulation couleur réfléchie
- **Commit** : `feat: add recursive reflections`

#### 3.2 Refraction (Snell's Law)
- IOR (indice optique) par matériau
- Calcul direction réfractée
- Test total internal reflection
- **Commit** : `feat: implement refraction`

#### 3.3 Fresnel Effect
- Schlick's approximation
- IOR-dépendant
- **Commit** : `feat: add fresnel term`

---

### Phase 4 : Géométrie Avancée (commits 14-16)

#### 4.1 Plane Primitive
- Intersection ray-plan infini
- Normal fixe
- **Commit** : `feat: add plane primitive`

#### 4.2 Triangle Intersection
- Algorithme Möller-Trumbore
- UV barycentriques
- **Commit** : `feat: implement triangle intersection`

#### 4.3 Mesh Support (.obj minimal)
- Parser OBJ basique (vertices, faces)
- Pas de textures, UV optionnelles
- **Commit** : `feat: add basic OBJ loader`

---

### Phase 5 : Performance (commits 17-21)

#### 5.1 Bounding Boxes (AABB)
- Axis-Aligned Bounding Box
- Intersection ray-AABB rapide
- **Commit** : `feat: add AABB bounding boxes`

#### 5.2 BVH (Bounding Volume Hierarchy)
- Construction top-down ou bottom-up
- Split axis (SAH ou middle)
- Traversée avec early termination
- **Commit** : `feat: implement BVH`

#### 5.3 Multithreading (pthreads)
- Découpe par tiles ou scanlines
- Synchronisation barrière
- **Commit** : `feat: add multithreaded rendering`

#### 5.4 Russian Roulette Termination
- Probabilité d'arrêt basée sur couleur
- Unbiased importance sampling
- **Commit** : `feat: optimize recursion with russian roulette`

#### 5.5 Memory Pooling
- Allocateur de pool pour structures fréquentes
- Réduction malloc/free
- **Commit** : `perf: add memory pool allocator`

---

### Phase 6 : Features Avancées (commits 22-28)

#### 6.1 Anti-aliasing (SSAA/Jittered)
- Jitter aléatoire dans pixel
- Multiple samples par pixel
- **Commit** : `feat: add anti-aliasing`

#### 6.2 Depth of Field
- Rayon de lens aléatoire
- Focal distance configurable
- **Commit** : `feat: implement depth of field`

#### 6.3 Soft Shadows
- Area lights (disque ou carré)
- Multiple samples vers lumière
- **Commit** : `feat: add soft shadows`

#### 6.4 Texture Mapping
- UV par sommet/triangle
- Interpolation UV
- **Commit** : `feat: implement texture mapping`

#### 6.5 Procedural Textures
- Checkerboard, bruit Perlin
- **Commit** : `feat: add procedural textures`

#### 6.6 Environment Mapping
- Skybox équirectangulaire ou cube
- Background pour rayons non-intersectant
- **Commit** : `feat: add environment mapping`

---

### Phase 7 : Optimisations SIMD (commits 29-31)

#### 7.1 SIMD (SSE/AVX)
- Vec3 en AVX (4 vec3 parallèles)
- Opérations batch
- **Commit** : `perf: add SIMD optimizations`

#### 7.2 Tile-based Rendering
- Cache-friendly tiling
- **Commit** : `perf: implement tile-based rendering`

#### 7.3 Adaptive Sampling
- Variance-based sampling density
- **Commit** : `feat: add adaptive sampling`

---

## 4. Structures de données clés

### vec3
```c
typedef struct {
    float x, y, z;
    float _pad;  // Alignement 16 bytes
} vec3;
```

### Ray
```c
typedef struct {
    vec3 origin;
    vec3 direction;
} ray;
```

### Material
```c
typedef struct {
    vec3 color;
    float roughness;
    float metallic;
    float ior;  // Indice optique
} material;
```

### Hit Record
```c
typedef struct {
    vec3 position;
    vec3 normal;
    float t;
    material mat;
} hit_record;
```

### Sphere
```c
typedef struct {
    vec3 center;
    float radius;
    material mat;
} sphere;
```

---

## 5. Critères de succès par phase

| Phase | Critère minimal |
|-------|-----------------|
| 1 | Image PPM avec sphères shaded |
| 2 | Multiple lumières, shadows visibles |
| 3 | Reflets et réfractions fonctionnels |
| 4 | Mesh OBJ chargeable et renderable |
| 5 | BVH < 2× temps phase 4, multithreading 2-4× speedup |
| 6 | Textures, DoF, soft shadows visibles |
| 7 | 30-50% gain SIMD mesuré |

---

## 6. Compilation et Exécution

### Build
```bash
make clean
make
```

### Flags GCC recommandés
```
-O3 -march=native -Wall -Wextra -std=c99 -lm -lpthread
```

### Exécution
```bash
./raytracer output.ppm
```

### Profiling
```bash
make profile
perf report
```

---

## 7. Contraintes de performance

- **Mémoire** : Pas d'alloc dans boucle rendu
- **Cache** : Structures compactes, alignement 16/32 bytes
- **Branches** : Éviter if/else prévisible dans hot paths
- **Malloc** : Pooling pour petites allocations fréquentes
- **Compilation** : `-Ofast -march=native` acceptable si exactitude numérique acceptable

---

## 8. Dépendances externes

**Interdites** : OpenGL, Vulkan, CUDA, libraires graphiques  
**Autorisées** : pthreads (POSIX standard), math.h, stdio.h

---

## 9. Tests et validation

- Image de référence avec sphères colorées
- Comparaison progressive phase par phase
- Validation numérique (intersection analytique)
- Benchmark temps rendu et memory usage

---

## 10. Git Strategy

**1 feature = 1 commit** (ou branche)

Exemple :
```
git checkout -b feat/vector-math
# ... code ...
git commit -m "feat: add vector math module"
git checkout main
git merge feat/vector-math
```

Ou directement :
```
git commit -m "feat: add vector math module"
```

---

## Résumé des phases

```
Phase 1 : Math + Ray + Sphere + Flat Shading
    ↓
Phase 2 : Lumières + Lambert + Shadows
    ↓
Phase 3 : Reflections + Refraction + Fresnel
    ↓
Phase 4 : Primitives (Plane, Triangle, Mesh)
    ↓
Phase 5 : Acceleration (AABB, BVH, Threading)
    ↓
Phase 6 : Advanced (AA, DoF, Textures, Env)
    ↓
Phase 7 : SIMD + Tile-based + Adaptive
```

Chaque phase est **indépendante et testable**.
