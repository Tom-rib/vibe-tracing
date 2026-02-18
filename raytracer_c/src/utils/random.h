/**
 * random.h - Random number generation utilities
 * 
 * Simple PRNG for sampling, jitter, and Monte Carlo integration.
 * Uses Linear Congruential Generator (LCG) for speed.
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <math.h>
#include "../math/vec3.h"

/* ============================================================================
   PRNG STATE
   ============================================================================ */

/**
 * Random number generator state.
 * Thread-local for multithreading support.
 */
typedef struct {
    uint32_t seed;
} rng_state;

/* ============================================================================
   INITIALIZATION
   ============================================================================ */

/**
 * Initialize RNG with seed.
 * @param state     RNG state
 * @param seed      Seed value (0 = use system time)
 */
rng_state rng_create(uint32_t seed);

/**
 * Initialize RNG from current time.
 */
rng_state rng_create_time(void);

/* ============================================================================
   RANDOM NUMBERS
   ============================================================================ */

/**
 * Generate random float in [0, 1).
 * @param state     RNG state (modified)
 * @return          Random float
 */
float rng_float(rng_state *state);

/**
 * Generate random float in [min, max).
 */
float rng_range(rng_state *state, float min, float max);

/**
 * Generate random integer in [0, max).
 */
int rng_int(rng_state *state, int max);

/* ============================================================================
   VECTOR SAMPLING
   ============================================================================ */

/**
 * Generate random unit vector (uniform on sphere).
 * Uses rejection sampling.
 */
vec3 rng_unit_vector(rng_state *state);

/**
 * Generate random vector in unit disk (for DoF sampling).
 * Uniform distribution in disk of radius 1.
 */
vec3 rng_unit_disk(rng_state *state);

/**
 * Generate random vector in unit sphere (for volume sampling).
 */
vec3 rng_unit_sphere(rng_state *state);

/**
 * Generate random vector in hemisphere around normal.
 * Cosine-weighted for importance sampling.
 */
vec3 rng_hemisphere_cosine(rng_state *state, vec3 normal);

/* ============================================================================
   UTILITY
   ============================================================================ */

/**
 * Quick random permutation of indices [0, n).
 * Modifies array in-place.
 */
void rng_shuffle(rng_state *state, int *arr, int n);

#endif // RANDOM_H
