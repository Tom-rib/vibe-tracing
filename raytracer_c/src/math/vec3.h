/**
 * vec3.h - 3D Vector Mathematics
 * 
 * Core vector operations for the raytracer. All functions are inline
 * for maximum performance. Memory aligned to 16 bytes for SIMD compatibility.
 */

#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <string.h>

/* ============================================================================
   VECTOR STRUCTURE
   ============================================================================ */

/**
 * 3D Vector with padding for 16-byte alignment (SIMD).
 * Members: x, y, z, _pad (unused, for alignment)
 */
typedef struct {
    float x, y, z;
    float _pad;
} vec3;

/* ============================================================================
   CONSTRUCTORS & INITIALIZATION
   ============================================================================ */

/** Create vector from components */
static inline vec3 vec3_create(float x, float y, float z) {
    return (vec3){x, y, z, 0.0f};
}

/** Zero vector */
static inline vec3 vec3_zero(void) {
    return (vec3){0.0f, 0.0f, 0.0f, 0.0f};
}

/** Vector (1, 1, 1) */
static inline vec3 vec3_one(void) {
    return (vec3){1.0f, 1.0f, 1.0f, 0.0f};
}

/* ============================================================================
   BASIC OPERATIONS
   ============================================================================ */

/** Add two vectors: a + b */
static inline vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z, 0.0f};
}

/** Subtract two vectors: a - b */
static inline vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z, 0.0f};
}

/** Negate vector: -v */
static inline vec3 vec3_negate(vec3 v) {
    return (vec3){-v.x, -v.y, -v.z, 0.0f};
}

/** Scale vector by scalar: v * s */
static inline vec3 vec3_scale(vec3 v, float s) {
    return (vec3){v.x * s, v.y * s, v.z * s, 0.0f};
}

/** Multiply component-wise: a * b */
static inline vec3 vec3_mul(vec3 a, vec3 b) {
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z, 0.0f};
}

/* ============================================================================
   DOT & CROSS PRODUCTS
   ============================================================================ */

/** Dot product: a · b */
static inline float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/** Cross product: a × b */
static inline vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0.0f
    };
}

/* ============================================================================
   LENGTH & NORMALIZATION
   ============================================================================ */

/** Length/magnitude of vector: ||v|| */
static inline float vec3_length(vec3 v) {
    return sqrtf(vec3_dot(v, v));
}

/** Squared length: ||v||² (faster, no sqrt) */
static inline float vec3_length_squared(vec3 v) {
    return vec3_dot(v, v);
}

/** Normalize vector to unit length */
static inline vec3 vec3_normalize(vec3 v) {
    float len = vec3_length(v);
    if (len < 1e-8f) {
        return vec3_zero();  // Avoid division by zero
    }
    return vec3_scale(v, 1.0f / len);
}

/** Fast normalize (assumes vector is not zero) */
static inline vec3 vec3_normalize_fast(vec3 v) {
    return vec3_scale(v, 1.0f / sqrtf(vec3_dot(v, v)));
}

/* ============================================================================
   REFLECTION & REFRACTION
   ============================================================================ */

/**
 * Reflect vector: reflects incoming direction off surface normal.
 * Assumes normal is unit vector.
 * Formula: v - 2 * (v · n) * n
 */
static inline vec3 vec3_reflect(vec3 v, vec3 n) {
    float dot = vec3_dot(v, n);
    return vec3_sub(v, vec3_scale(n, 2.0f * dot));
}

/**
 * Refract vector using Snell's law.
 * @param v     Incoming direction (should be normalized)
 * @param n     Surface normal (should be normalized)
 * @param eta   Ratio of refractive indices (eta1 / eta2)
 * @return      Refracted direction, or zero vector if total internal reflection
 */
static inline vec3 vec3_refract(vec3 v, vec3 n, float eta) {
    float cos_i = -vec3_dot(v, n);
    if (cos_i < 0.0f) {
        cos_i = -cos_i;
        n = vec3_negate(n);
    }
    
    float cos_t_sq = 1.0f - eta * eta * (1.0f - cos_i * cos_i);
    if (cos_t_sq < 0.0f) {
        return vec3_zero();  // Total internal reflection
    }
    
    float cos_t = sqrtf(cos_t_sq);
    return vec3_add(
        vec3_scale(v, eta),
        vec3_scale(n, eta * cos_i - cos_t)
    );
}

/* ============================================================================
   CLAMPING & UTILITIES
   ============================================================================ */

/** Clamp vector components to [min, max] */
static inline vec3 vec3_clamp(vec3 v, float min_val, float max_val) {
    float clamp_component(float x) {
        if (x < min_val) return min_val;
        if (x > max_val) return max_val;
        return x;
    }
    
    return (vec3){
        clamp_component(v.x),
        clamp_component(v.y),
        clamp_component(v.z),
        0.0f
    };
}

/** Min component of vector */
static inline float vec3_min_component(vec3 v) {
    float min = v.x;
    if (v.y < min) min = v.y;
    if (v.z < min) min = v.z;
    return min;
}

/** Max component of vector */
static inline float vec3_max_component(vec3 v) {
    float max = v.x;
    if (v.y > max) max = v.y;
    if (v.z > max) max = v.z;
    return max;
}

/** Absolute value of vector components */
static inline vec3 vec3_abs(vec3 v) {
    return (vec3){
        v.x < 0.0f ? -v.x : v.x,
        v.y < 0.0f ? -v.y : v.y,
        v.z < 0.0f ? -v.z : v.z,
        0.0f
    };
}

/* ============================================================================
   COMPARISONS
   ============================================================================ */

/** Check if vectors are approximately equal */
static inline int vec3_approx_equal(vec3 a, vec3 b, float epsilon) {
    vec3 diff = vec3_sub(a, b);
    return vec3_length_squared(diff) < epsilon * epsilon;
}

/** Check if vector is approximately zero */
static inline int vec3_approx_zero(vec3 v, float epsilon) {
    return vec3_length_squared(v) < epsilon * epsilon;
}

#endif // VEC3_H
