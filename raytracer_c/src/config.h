/**
 * config.h - Configuration globale du raytracer
 * 
 * Ce fichier contient toutes les constantes configurables du projet.
 * Modifiez ces valeurs pour ajuster le comportement global du raytracer.
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================================
   IMAGE RESOLUTION
   ============================================================================ */

#define IMAGE_WIDTH         800
#define IMAGE_HEIGHT        600

/* ============================================================================
   RENDERING QUALITY & SPEED
   ============================================================================ */

/** Maximum recursion depth for reflections/refractions */
#define MAX_DEPTH           5

/** Number of threads for multithreading (Phase 5.3) */
#define NUM_THREADS         4

/** Number of AA samples per pixel (Phase 6.1) */
#define NUM_AA_SAMPLES      4

/** Number of light samples for soft shadows (Phase 6.3) */
#define NUM_LIGHT_SAMPLES   1

/** Adaptive sampling variance threshold (Phase 7.3) */
#define ADAPTIVE_THRESHOLD  0.01f

/* ============================================================================
   SCENE LIMITS
   ============================================================================ */

/** Maximum number of objects in scene */
#define MAX_OBJECTS         1000

/** Maximum number of lights */
#define MAX_LIGHTS          16

/** Maximum triangles (for meshes) */
#define MAX_TRIANGLES       1000000

/* ============================================================================
   ACCELERATION STRUCTURES
   ============================================================================ */

/** Enable BVH acceleration (Phase 5.2) */
#define USE_BVH             1

/** BVH maximum depth */
#define BVH_MAX_DEPTH       32

/** Enable SIMD optimizations (Phase 7.1) */
#define USE_SIMD            0

/* ============================================================================
   CAMERA
   ============================================================================ */

/** Default camera FOV (vertical, in degrees) */
#define DEFAULT_FOV         90.0f

/** Default camera position */
#define DEFAULT_CAM_X       0.0f
#define DEFAULT_CAM_Y       0.0f
#define DEFAULT_CAM_Z       0.0f

/* ============================================================================
   MEMORY ALLOCATION
   ============================================================================ */

/** Enable memory pooling (Phase 5.5) */
#define USE_MEMORY_POOL     1

/** Size of memory pool (bytes) */
#define MEMORY_POOL_SIZE    (1024 * 1024 * 10)  // 10 MB

/* ============================================================================
   NUMERICAL CONSTANTS
   ============================================================================ */

/** Small epsilon for ray-surface intersection tests */
#define EPSILON             1e-6f

/** Maximum ray distance (for far clip) */
#define MAX_RAY_DISTANCE    1e6f

/** Russian roulette minimum probability */
#define RR_MIN_PROB         0.1f

/** Russian roulette maximum probability */
#define RR_MAX_PROB         0.9f

/* ============================================================================
   OUTPUT & DEBUGGING
   ============================================================================ */

/** Verbose output during rendering */
#define VERBOSE             0

/** Save intermediate frames during rendering */
#define SAVE_INTERMEDIATES  0

/** Enable performance profiling */
#define ENABLE_PROFILING    0

/* ============================================================================
   FEATURE FLAGS (Control what to compile)
   ============================================================================ */

/** Phase 2: Lighting & Shading */
#define FEATURE_LIGHTING    1

/** Phase 3: Reflections & Refractions */
#define FEATURE_REFLECTIONS 1
#define FEATURE_REFRACTIONS 1
#define FEATURE_FRESNEL     1

/** Phase 4: Advanced Geometry */
#define FEATURE_PLANES      1
#define FEATURE_TRIANGLES   1
#define FEATURE_MESHES      1

/** Phase 5: Performance */
#define FEATURE_AABB        1
#define FEATURE_THREADING   1

/** Phase 6: Advanced Features */
#define FEATURE_ANTIALIASING    1
#define FEATURE_DEPTH_OF_FIELD  1
#define FEATURE_SOFT_SHADOWS    1
#define FEATURE_TEXTURES        1
#define FEATURE_PROCEDURAL      1
#define FEATURE_ENVIRONMENT     1

/** Phase 7: SIMD & Hardcore */
#define FEATURE_SIMD        0
#define FEATURE_TILE_BASED  0
#define FEATURE_ADAPTIVE    0

/* ============================================================================
   OPTIMIZATION FLAGS
   ============================================================================ */

/** Compiler pragmas for aggressive optimization */
#ifdef __GNUC__
    #define INLINE          inline __attribute__((always_inline))
    #define RESTRICT        __restrict__
    #define ALIGNED(n)      __attribute__((aligned(n)))
#else
    #define INLINE          inline
    #define RESTRICT
    #define ALIGNED(n)
#endif

/* ============================================================================
   COLOR SPACE
   ============================================================================ */

/** Gamma correction exponent (1.0 = no correction, 2.2 = typical gamma) */
#define GAMMA_CORRECTION    1.0f

/** Tone mapping method: 0 = none, 1 = reinhard, 2 = aces */
#define TONE_MAPPING        0

/* ============================================================================
   RANDOM NUMBER GENERATION
   ============================================================================ */

/** Random seed (0 = use system time) */
#define RANDOM_SEED         0

/* ============================================================================
   VALIDATION / TESTING
   ============================================================================ */

/** Enable assertion checks */
#define ENABLE_ASSERTS      1

/** Validate ray at each step (slower but safer) */
#define VALIDATE_RAYS       0

/** Check for NaN in computation results */
#define CHECK_NAN           1

#endif // CONFIG_H
