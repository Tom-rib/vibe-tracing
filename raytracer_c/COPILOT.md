# COPILOT.md - Stratégie Vibe Coding Raytracer C

## 🎯 Mission

Développer un **raytracer CPU en C performant et modulaire** en collaborant étroitement avec un agent IA (Claude, Copilot, etc.), en respectant un découpage Git strict : **1 feature = 1 commit**.

---

## 📋 Workflow Principal

### Étape 1 : Préparation et contexte
1. Lire `SPECIFICATION.md` en entier
2. Valider l'architecture dans `PROJECT_STRUCTURE.md`
3. Comprendre les phases et dépendances

### Étape 2 : Par phase (7 phases = ~7 sessions)
1. **Identifier** la phase courante et ses commits
2. **Écrire un prompt** détaillé pour l'agent
3. **Valider** le code et les tests
4. **Committer** proprement : `git commit -m "feat: ..."`

### Étape 3 : Optimisation continue
- Profiler avec `gprof` ou `perf`
- Identifier bottlenecks
- Committer les optimisations : `git commit -m "perf: ..."`

---

## 🚀 Prompts par Phase

### Phase 1 : Base Math & Raycasting

#### Commit 1.1 : Vector Math Library
```
Crée la bibliothèque mathématique vectorielle en C.

Critères :
- Structure vec3 alignée 16 bytes (+ _pad float)
- Fonctions : add, sub, dot, cross, normalize, length, scale
- Toutes les fonctions doivent être inline pour performance
- Header vec3.h bien organisé
- Pas d'allocation dynamique
- Tests manuels : calcul dot, cross, normalize sur (1,2,3)

Commence par vec3.h, puis vec3.c.
Compile avec : gcc -O3 -march=native -std=c99 -Wall -Wextra
```

#### Commit 1.2 : Ray Structure
```
Crée la structure ray et les opérations basiques.

Critères :
- struct ray { vec3 origin, direction }
- ray_at(ray, t) → position sur le rayon
- Validation : ray_at(ray, 0) == origin, ray_at(ray, 1) == origin + direction
- Header ray.h simple et clair

Après que vec3 soit validée.
```

#### Commit 1.3 : Camera Pinhole
```
Implémente une caméra perspective simple.

Critères :
- Camera position, forward, up, FOV (vertical, en degrés)
- camera_ray(x, y) → ray pour pixel (x, y)
- Validation : rayon central doit être forward
- Resolution 800×600 par défaut
- Ne pas allocuer d'image ici, juste les rayons

Dépend de : ray + vec3.
```

#### Commit 1.4 : Image Output (PPM)
```
Implémente la sauvegarde d'image PPM (format P6 binary).

Critères :
- Structure image { uint8 *pixels, width, height }
- image_write_ppm(filename) → fichier valide
- Test : générer image rose (255, 0, 0) et vérifier fichier
- Conversion float [0,1] → uint8 [0,255] avec clamp

Dépend de : rien (I/O pur).
```

#### Commit 1.5 : Sphere Intersection
```
Implémente l'intersection ray-sphère analytique.

Critères :
- struct sphere { vec3 center, float radius, material mat }
- sphere_intersect(ray, sphere, hit_record *out) → bool
- Retourner le t minimum positif
- Calculer normal = (hit_position - center) / radius
- Validation : rayon au centre devrait hit à ±radius

Dépend de : ray, vec3.
```

#### Commit 1.6 : Flat Shading
```
Implémente le shading plat.

Critères :
- Pour chaque pixel : lancer rayon, tester intersection sphères
- Couleur = couleur sphère si intersect, noir sinon
- Appliquer un peu de diffuse simplement (ex: 0.5 + 0.5 * dot(normal, view_dir))
- Output PPM final
- Image 800×600

Dépend de : tout Phase 1.
```

---

### Phase 2 : Lumière & Shading

#### Commit 2.1 : Point Lights
```
Ajoute les lumières ponctuelles à la scène.

Critères :
- struct light { vec3 position, vec3 color, float intensity }
- Tableau de lumières (max 16)
- Pas d'allocation dynamique, array static
- Structure scène contient lights[]

Dépend de : Phase 1 complète.
```

#### Commit 2.2 : Lambert Diffuse
```
Implémente l'illumination Lambert diffuse.

Critères :
- Pour chaque lumière : calculer cos(theta) entre normal et light_dir
- dot(normal, light_dir) clampé [0, 1]
- Couleur finale = objet_color × light_color × dot × intensity
- Cumuler contributions de toutes lumières
- Validation : sphère blanche illuminée correctement

Dépend de : lights.
```

#### Commit 2.3 : Specular (Phong/Blinn-Phong)
```
Ajoute les highlights spéculaires (Phong).

Critères :
- Vecteur vue (caméra → surface)
- Vecteur réfléchi = reflect(light_dir, normal)
- Specular = pow(dot(reflect, view), shininess) × light
- shininess = 32 (par défaut, configurable)
- Cumul : diffuse + specular

Dépend de : Lambert.
```

#### Commit 2.4 : Shadow Rays
```
Implémente les ombres dures (hard shadows).

Critères :
- Rayon depuis surface vers lumière (epsilon offset)
- Tester intersection avant la lumière
- Si occludé : pas de contribution lumière
- Shadows sombres et nettes visibles

Dépend de : Lambert + Phong.
```

---

### Phase 3 : Réflexion & Réfraction

#### Commit 3.1 : Recursive Reflections
```
Ajoute les réflexions récursives.

Critères :
- reflect_dir = dir - 2 * dot(dir, normal) * normal
- Lancer rayon réfléchi, limiter profondeur max = 5
- Couleur finale = couleur_directe + reflection_strength × reflected_color
- Validation : sphère miroir visible dans autre sphère

Dépend de : Phase 2 complète.
```

#### Commit 3.2 : Refraction (Snell's Law)
```
Implémente la réfraction (Snell's law).

Critères :
- struct material { color, roughness, metallic, ior }
- Snell : eta1 * sin(theta1) = eta2 * sin(theta2)
- refract_dir basé sur IOR du matériau
- Test total internal reflection (TIR)
- Validation : verre IOR=1.5 visible

Dépend de : reflection.
```

#### Commit 3.3 : Fresnel Effect
```
Ajoute l'effet Fresnel (approximation de Schlick).

Critères :
- F0 = ((1 - ior) / (1 + ior))^2
- F = F0 + (1 - F0) * (1 - cos(theta))^5
- Mélanger : F * reflect + (1 - F) * refract
- Validation : angle rasant = plus réfléchissant

Dépend de : reflection + refraction.
```

---

### Phase 4 : Géométrie Avancée

#### Commit 4.1 : Plane Primitive
```
Ajoute un primitive plan infini.

Critères :
- struct plane { vec3 normal, float d, material mat }
- plane_intersect(ray, plane) → hit_record ou miss
- Validation : plan horizontal devrait être visible

Dépend de : Phase 3 complète.
```

#### Commit 4.2 : Triangle Intersection
```
Implémente l'algo Möller-Trumbore.

Critères :
- Intersection ray-triangle rapide
- Retourner UV barycentriques
- struct triangle { vec3 v0, v1, v2, material mat }
- Validation : triangle du mesh devrait être hittable

Dépend de : ray, vec3.
```

#### Commit 4.3 : OBJ Loader (Minimal)
```
Parseur OBJ basique.

Critères :
- Charger vertices (v x y z)
- Charger faces (f v1 v2 v3 ou v1//vn1 v2//vn2 v3//vn3)
- Créer liste de triangles
- Pas de gestion des groupes/matériaux
- Validation : bunny.obj ou teapot.obj chargeables

Dépend de : triangle.
```

---

### Phase 5 : Performance

#### Commit 5.1 : AABB Bounding Boxes
```
Implémente les boîtes englobantes alignées.

Critères :
- struct aabb { vec3 min, max }
- aabb_intersect(ray) → bool (rapide)
- Construire AABB pour sphères, triangles
- Validation : AABB doit être tight

Dépend de : Phase 4 complète.
```

#### Commit 5.2 : BVH Tree
```
Construit une hiérarchie BVH.

Critères :
- Construction top-down avec SAH ou split axe médian
- struct bvh_node { aabb box, *left, *right ou geometry }
- Traversée avec early termination
- Hit plus rapide que O(n)
- Validation : BVH vs brute-force même résultat

Dépend de : AABB.
```

#### Commit 5.3 : Multithreading
```
Ajoute le rendu multithreadé (pthreads).

Critères :
- Découpe par tiles (ex: 16×16 pixels)
- Créer N workers (= nb CPU cores)
- Synchronisation barrière entre tiles
- Validation : 2-4× speedup sur 4 cores

Dépend de : Phase 4 complète.
```

#### Commit 5.4 : Russian Roulette
```
Optimise la récursion avec Russian Roulette.

Critères :
- Probabilité arrêt basée sur luminance couleur
- P_continue = clamp(luminance(color), 0.1, 0.9)
- Diviser par probabilité (unbiased)
- Arrêt plus rapide, peu bruit

Dépend de : reflections + refractions.
```

#### Commit 5.5 : Memory Pooling
```
Allocateur simple pool pour petites structs.

Critères :
- Pool pour hit_records, rays (structures fréquentes)
- Pool_alloc(size) et pool_free()
- Réduire malloc/free en boucle rendu
- Validation : moins d'allocations mesurées

Dépend de : toute archi.
```

---

### Phase 6 : Features Avancées

#### Commit 6.1 : Anti-aliasing (SSAA)
```
Anti-aliasing par supersampling jittered.

Critères :
- N=4 ou 9 samples par pixel
- Jitter aléatoire dans [0, 1]×[0, 1] pixel
- Moyenne des samples
- Validation : edges lisses

Dépend de : Phase 5 complète.
```

#### Commit 6.2 : Depth of Field
```
Depth of Field (focus + blur).

Critères :
- Aperture radius configurable
- Focal distance (distance de focus)
- Rayons aléatoires dans disque aperture
- Validation : objets hors focus flous

Dépend de : camera, random.
```

#### Commit 6.3 : Soft Shadows
```
Soft shadows via area lights.

Critères :
- Area light = disque de rayon R
- N samples aléatoires vers lumière
- Moyenne occlusion
- Validation : ombres douces visibles

Dépend de : shadows.
```

#### Commit 6.4 : Texture Mapping
```
Texture mapping par UV.

Critères :
- UV par sommet (triangle)
- Interpolation bary UV
- sample_texture(u, v) → color
- Validation : checkerboard visible

Dépend de : triangle, materials.
```

#### Commit 6.5 : Procedural Textures
```
Textures procédurales (checkerboard, bruit).

Critères :
- Checkerboard : mod(uv) pour pattern
- Bruit Perlin simple ou value noise
- Applicable comme albedo
- Validation : patterns différents visibles

Dépend de : texture mapping.
```

#### Commit 6.6 : Environment Mapping
```
Skybox/environment mapping.

Critères :
- Rayons non-hitant → sample environment
- Équirectangulaire ou simple gradient
- Illumination ambiante
- Validation : skybox visible en arrière

Dépend de : Phase 5 complète.
```

---

### Phase 7 : SIMD & Hardcore Optimizations

#### Commit 7.1 : SIMD (SSE/AVX)
```
Optimisations SIMD pour vec3.

Critères :
- Traiter 4 vec3 en parallèle (AVX)
- Remplacer operations scalaires par SIMD
- Alignment 32 bytes pour AVX
- Validation : 30-50% gain mesuré

Dépend de : Phase 6 complète.
```

#### Commit 7.2 : Tile-based Rendering
```
Cache-friendly tile-based rendering.

Critères :
- Tuiles 32×32 ou 64×64 pixels
- Travailler sur tile complet (cache L3)
- Validation : cache hit rate meilleur

Dépend de : multithreading.
```

#### Commit 7.3 : Adaptive Sampling
```
Adaptive sampling basé variance.

Critères :
- Variance locale dans tile
- Zones haute variance = plus samples
- Zones basse variance = moins samples
- Validation : moins de samples totaux, même qualité

Dépend de : antialiasing.
```

---

## 📊 Token Budget Strategy

**Contexte permanent** (inclus dans chaque prompt) :
- `SPECIFICATION.md` (entier)
- `PROJECT_STRUCTURE.md` (entier)
- Code des commits précédents (références)

**Prompts optimisés** :
- Court (100-200 tokens) : juste la feature
- Inclure test/validation dans même prompt
- Demander code compilable et testable d'abord
- Demander commits clairs en même temps

**Exemple structure prompt** :
```
[SPEC rapide de 2 lignes]
[Critères de succès]
[Code attendu : .h + .c]
[Tests minimaux]
[Commit message]
```

---

## ✅ Checklist par Commit

Avant chaque `git commit` :

- [ ] Code compile sans warnings (`-Wall -Wextra`)
- [ ] Tests validés manuellement ou automatiquement
- [ ] Pas de TODO laissés
- [ ] Structure bien organisée (déclarations claires)
- [ ] Inline documenté si non-trivial
- [ ] Pas d'allocation inutile
- [ ] Performance validée (si phase 5+)
- [ ] Message commit clair : `feat:` ou `perf:` ou `refactor:`

---

## 🔧 Compilation Test Rapide

```bash
# Après chaque feature
gcc -O3 -march=native -std=c99 -Wall -Wextra -lm -lpthread \
    src/math/*.c src/core/*.c ... -o raytracer
./raytracer output.ppm
# Vérifier output.ppm avec image viewer
```

---

## 📈 Profiling & Optimization

```bash
# Build avec debug symbols
gcc -O2 -g -std=c99 ... -o raytracer

# Profile
gprof ./raytracer gmon.out

# Ou avec perf
perf record ./raytracer
perf report
```

Chercher :
- Hot functions (> 10% temps)
- Memory stalls
- Cache misses

---

## 🎬 Session Finale

À la fin (après Phase 7) :
1. `git log --oneline` → 30+ commits clairs
2. Générer image finale (1920×1080, 256 samples)
3. `time ./raytracer` → benchmark
4. Documenter speedup par phase
5. Créer `SESSION.md` avec résultats

---

## 💡 Tips Vibe Coding

1. **Itérative** : Feature par feature, pas big bang
2. **Validable** : Chaque commit doit être testable
3. **Découpé** : Git propre = contexte manager facile
4. **Documenté** : SPEC = source de vérité pour l'agent
5. **Optimisé** : Profiler avant d'optimiser
6. **Team-friendly** : Code lisible pour human review

---

## 📚 Documents de référence

- `SPECIFICATION.md` : Spécification technique complète
- `PROJECT_STRUCTURE.md` : Architecture des dossiers
- Git history : Source de vérité

Enjoy le vibe coding ! 🚀
