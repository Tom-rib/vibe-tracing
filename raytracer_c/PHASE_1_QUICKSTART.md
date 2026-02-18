# PHASE_1_QUICKSTART.md - Démarrer Phase 1

## 🎯 Objectif Phase 1

Générer une image PPM simple avec une sphère shaded.

```
Input  : Rien (scène dure-codée)
Output : output.ppm (800×600, sphère blanche/grise au centre)
```

---

## 📋 Checklist Phase 1

- [ ] **1.1** - vec3.h/c (vector math)
- [ ] **1.2** - ray.h/c (ray structure)
- [ ] **1.3** - camera.h/c (pinhole camera)
- [ ] **1.4** - image.h/c (PPM output)
- [ ] **1.5** - sphere.h/c (intersection)
- [ ] **1.6** - main.c (flat shading, final image)

---

## 🚀 Étape par étape

### Étape 0 : Initialiser le projet Git

```bash
cd raytracer_c
git init
git config user.name "Votre Nom"
git config user.email "votre@email.com"
git add .
git commit -m "init: project setup with docs and build system"
```

### Étape 1.1 : Vector Math

**Prompt pour l'agent** :
```
Crée la bibliothèque mathématique vectorielle en C (vec3).

Fichiers à créer :
- src/math/vec3.h (header)
- src/math/vec3.c (implémentation)

Critères :
- Structure : typedef struct { float x, y, z, _pad; } vec3;
- Alignement 16 bytes (important pour SIMD Phase 7)
- Fonctions inline :
  * vec3 vec3_add(vec3 a, vec3 b)
  * vec3 vec3_sub(vec3 a, vec3 b)
  * float vec3_dot(vec3 a, vec3 b)
  * vec3 vec3_cross(vec3 a, vec3 b)
  * vec3 vec3_normalize(vec3 v)
  * float vec3_length(vec3 v)
  * vec3 vec3_scale(vec3 v, float s)

Pas de malloc/free, tout statique.
Include guards correct.
Compilation sans warnings : gcc -O3 -std=c99 -Wall -Wextra

Commence par vec3.h avec tous les includes et struct.
Puis vec3.c avec les implémentations inline.
```

**Après codegen** :
```bash
mkdir -p src/math
# ... code généré par l'agent ...
gcc -O3 -std=c99 -Wall -Wextra -c src/math/vec3.c -o vec3.o  # Test compile
git add src/math/
git commit -m "feat: add vector math module"
```

---

### Étape 1.2 : Ray Structure

**Prompt** :
```
Crée la structure ray et opérations de base.

Fichiers :
- src/core/ray.h
- src/core/ray.c

Critères :
- Include vec3.h
- struct ray { vec3 origin, direction; }
- Fonction : vec3 ray_at(ray r, float t)
  → Retourne : origin + direction * t
  
Tests manuels dans ray.c (commentés) :
  ray r = {{0,0,0,0}, {1,0,0,0}};
  vec3 p = ray_at(r, 5.0f); // Doit donner {5,0,0,0}

Compile sans warnings.
```

**Après** :
```bash
mkdir -p src/core
git add src/core/ray.h src/core/ray.c
git commit -m "feat: add ray structure"
```

---

### Étape 1.3 : Camera

**Prompt** :
```
Implémente une caméra perspective (pinhole).

Fichiers :
- src/core/camera.h
- src/core/camera.c

Structures :
- struct camera {
    vec3 position;
    vec3 forward;  // direction avant
    vec3 right;    // direction droite
    vec3 up;       // direction haut
    float fov;     // field of view vertical (en degrés)
    int width, height;
  }

Fonctions :
- camera camera_create(int width, int height, float fov)
  → Position par défaut : (0, 0, 0)
  → Forward : (0, 0, -1)
  → Right : (1, 0, 0)
  → Up : (0, 1, 0)

- ray camera_ray(camera *cam, int x, int y)
  → Génère rayon pour pixel (x, y)
  → x, y en [0, width) et [0, height)
  → Rayon centré au (width/2, height/2)

Pas d'allocation dynamique.
```

**Après** :
```bash
git add src/core/camera.h src/core/camera.c
git commit -m "feat: implement basic camera"
```

---

### Étape 1.4 : Image Output

**Prompt** :
```
Implémente la sauvegarde d'image PPM (format P6).

Fichiers :
- src/core/image.h
- src/core/image.c

Structures :
- struct image {
    uint8_t *pixels;
    int width, height;
  }

Fonctions :
- image image_create(int width, int height)
  → Alloue pixels (width × height × 3 bytes)

- void image_set_pixel(image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b)
  → Remplace pixel à (x, y)

- void image_write_ppm(image *img, const char *filename)
  → Écrit fichier PPM P6 binary
  → Format :
    P6\n
    width height\n
    255\n
    [binary RGB bytes]

- void image_destroy(image *img)
  → Libère pixels

Validation : générer image 10×10 rose (255,0,0) et vérifier fichier
PPM avec un viewer.
```

**Après** :
```bash
git add src/core/image.h src/core/image.c
git commit -m "feat: add PPM image output"
```

---

### Étape 1.5 : Sphere Intersection

**Prompt** :
```
Implémente l'intersection ray-sphère analytique.

Fichiers :
- src/primitives/sphere.h
- src/primitives/sphere.c

Structures :
- struct material {
    vec3 color;
    float roughness;
    float metallic;
    float ior;
  }

- struct hit_record {
    vec3 position;
    vec3 normal;
    float t;
    material mat;
  }

- struct sphere {
    vec3 center;
    float radius;
    material mat;
  }

Fonctions :
- bool sphere_intersect(ray r, sphere s, hit_record *out)
  → Résout : |r.origin + t * r.direction - s.center|² = s.radius²
  → Retourne le t minimum POSITIF
  → Remplit *out avec position, normal, t, material
  → La normale doit être normalisée

Validation :
  - Rayon au centre : doit hit à t = ±radius
  - Rayon parallèle tangent : pas de hit
  - Normale correcte (pointant dehors)
```

**Après** :
```bash
mkdir -p src/primitives
git add src/primitives/sphere.h src/primitives/sphere.c
git commit -m "feat: implement sphere intersection"
```

---

### Étape 1.6 : Flat Shading & Main

**Prompt** :
```
Implémente le shading plat et la boucle de rendu principale.

Fichiers :
- src/main.c (créer)

Contenu :
1. Créer image 800×600
2. Créer caméra (FOV 90°)
3. Créer une sphère au centre (position z=-5, radius=1.0, couleur blanc)
4. Pour chaque pixel :
   a. Lancer rayon depuis caméra
   b. Tester intersection sphère
   c. Si hit :
      - Normal du hitpoint
      - Shading simple : couleur * (0.5 + 0.5 * dot(normal, -ray.direction))
      - Clampé [0,1] et converti en uint8
   d. Si miss : noir (0, 0, 0)
5. Sauvegarder "output.ppm"
6. Afficher temps d'exécution

Compilation :
gcc -O3 -march=native -std=c99 -Wall -Wextra -lm \
    src/math/vec3.c src/core/ray.c src/core/camera.c \
    src/core/image.c src/primitives/sphere.c src/main.c \
    -o raytracer

Validation : output.ppm doit montrer sphère blanche/grise illuminée.
```

**Après** :
```bash
# Tester la compilation et le rendu
make clean && make
./build/bin/raytracer output.ppm
# Ouvrir output.ppm avec un viewer PPM

git add src/main.c
git commit -m "feat: add flat shading and main loop"
```

---

## 📊 Résultat attendu Phase 1

### Image visuelle
```
output.ppm (800×600) :
- Fond noir
- Sphère blanche/grise au centre
- Shading lisse (normal-based)
- Pas de ombres ni lumières (juste diffuse de la normal)
```

### Performance
```
CPU : ~1-2 sec pour 800×600 (dépend CPU)
Memory : ~2 MB (800×600×3 bytes image + code)
```

### Git Log
```
feat: add vector math module
feat: add ray structure
feat: implement basic camera
feat: add PPM image output
feat: implement sphere intersection
feat: add flat shading and main loop
```

---

## ✅ Validation Phase 1

Avant de passer à Phase 2 :

- [ ] `make clean && make` compile sans warnings
- [ ] `./build/bin/raytracer` s'exécute sans crash
- [ ] `output.ppm` existe et peut être ouvert
- [ ] Image montre sphère shaded (pas noire, pas bruitée)
- [ ] 6 commits Git clairs
- [ ] Tous les fichiers ont headers clairs
- [ ] Pas de TODOs

---

## 🐛 Troubleshooting Phase 1

### Compilation échoue : "undefined reference to `sqrt`"
```bash
# Ajouter -lm à la compilation
gcc ... -lm src/...c
```

### PPM ne s'ouvre pas
- Vérifier que format est "P6\n" (pas "P5")
- Vérifier que les bytes RGB sont bien présents (width × height × 3)
- Essayer avec un viewer externe

### Image entièrement noire
- Rayon bien lancé depuis caméra ?
- Sphère bien positionnée (z=-5, caméra z=0) ?
- Shading calcule dot(normal, view_dir) ?

### Image entièrement blanche
- Clamp les couleurs à [0, 1] ?
- uint8_t conversion correct : `(int)(color * 255.99f)` ?

---

## 🎬 Prochain : Phase 2

Une fois Phase 1 validée, passer à **Phase 2 : Lumières & Shading**

- Ajouter point lights
- Implémenter Lambert diffuse
- Ajouter Phong specular
- Implémenter shadow rays

Voir `COPILOT.md` pour les prompts Phase 2.

---

Happy coding! 🎨
