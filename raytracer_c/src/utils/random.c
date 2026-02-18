/**
 * random.c - Random number generation implementation
 */

#include "random.h"
#include <time.h>

/* Linear Congruential Generator (LCG) parameters */
#define LCG_A  1103515245u
#define LCG_C  12345u
#define LCG_M  2147483648u

/**
 * Initialize RNG with seed.
 */
rng_state rng_create(uint32_t seed) {
    if (seed == 0) {
        seed = (uint32_t)time(NULL);
    }
    return (rng_state){seed};
}

/**
 * Initialize RNG from current time.
 */
rng_state rng_create_time(void) {
    return rng_create((uint32_t)time(NULL));
}

/**
 * Generate next random number.
 * Simple LCG implementation.
 */
static uint32_t rng_next(rng_state *state) {
    state->seed = (LCG_A * state->seed + LCG_C) & 0x7fffffffu;
    return state->seed;
}

/**
 * Generate random float in [0, 1).
 */
float rng_float(rng_state *state) {
    return (float)rng_next(state) / (float)LCG_M;
}

/**
 * Generate random float in [min, max).
 */
float rng_range(rng_state *state, float min, float max) {
    return min + rng_float(state) * (max - min);
}

/**
 * Generate random integer in [0, max).
 */
int rng_int(rng_state *state, int max) {
    if (max <= 0) return 0;
    return (int)(rng_float(state) * max);
}

/**
 * Generate random unit vector (uniform on sphere).
 * Uses rejection sampling in unit cube.
 */
vec3 rng_unit_vector(rng_state *state) {
    vec3 p;
    do {
        p = vec3_create(
            rng_range(state, -1.0f, 1.0f),
            rng_range(state, -1.0f, 1.0f),
            rng_range(state, -1.0f, 1.0f)
        );
    } while (vec3_length_squared(p) >= 1.0f);
    
    return vec3_normalize(p);
}

/**
 * Generate random vector in unit disk.
 * Uniform distribution (not polar coordinates).
 */
vec3 rng_unit_disk(rng_state *state) {
    vec3 p;
    do {
        p = vec3_create(
            rng_range(state, -1.0f, 1.0f),
            rng_range(state, -1.0f, 1.0f),
            0.0f
        );
    } while (vec3_length_squared(p) >= 1.0f);
    
    return p;
}

/**
 * Generate random vector in unit sphere.
 * Uniform distribution using rejection sampling.
 */
vec3 rng_unit_sphere(rng_state *state) {
    return rng_unit_vector(state);  // Same as unit_vector
}

/**
 * Generate random vector in hemisphere around normal.
 * Cosine-weighted for importance sampling (better convergence).
 */
vec3 rng_hemisphere_cosine(rng_state *state, vec3 normal) {
    // Generate random point in unit disk
    float r1 = rng_float(state);
    float r2 = rng_float(state);
    
    float r = sqrtf(r1);
    float theta = 2.0f * 3.14159265f * r2;
    
    float x = r * cosf(theta);
    float y = r * sinf(theta);
    float z = sqrtf(1.0f - r1);  // Cosine-weighted
    
    // Create local coordinate frame around normal
    vec3 u, v;
    
    if (fabsf(normal.x) < 0.9f) {
        u = vec3_normalize(vec3_cross(vec3_create(1.0f, 0.0f, 0.0f), normal));
    } else {
        u = vec3_normalize(vec3_cross(vec3_create(0.0f, 1.0f, 0.0f), normal));
    }
    v = vec3_cross(normal, u);
    
    // Transform to hemisphere
    return vec3_add(
        vec3_add(
            vec3_scale(u, x),
            vec3_scale(v, y)
        ),
        vec3_scale(normal, z)
    );
}

/**
 * Shuffle array using Fisher-Yates algorithm.
 */
void rng_shuffle(rng_state *state, int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rng_int(state, i + 1);
        
        // Swap
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}
