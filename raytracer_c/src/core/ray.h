/**
 * ray.h - Ray structure and operations
 * 
 * A ray is defined by an origin point and a direction vector.
 * Formula: P(t) = origin + t * direction (where t >= 0)
 */

#ifndef RAY_H
#define RAY_H

#include "../math/vec3.h"

/* ============================================================================
   RAY STRUCTURE
   ============================================================================ */

/**
 * A ray with origin and direction.
 * direction should be normalized for best results.
 */
typedef struct {
    vec3 origin;
    vec3 direction;
} ray;

/* ============================================================================
   CONSTRUCTORS
   ============================================================================ */

/** Create ray from origin and direction */
static inline ray ray_create(vec3 origin, vec3 direction) {
    return (ray){origin, direction};
}

/* ============================================================================
   OPERATIONS
   ============================================================================ */

/**
 * Get point on ray at parameter t.
 * Formula: P(t) = origin + t * direction
 * 
 * @param r     Ray
 * @param t     Parameter (typically t > 0)
 * @return      Point on ray
 */
static inline vec3 ray_at(ray r, float t) {
    return vec3_add(r.origin, vec3_scale(r.direction, t));
}

/**
 * Get point on ray at parameter t (non-inline version for reference).
 * Used mainly for testing.
 */
vec3 ray_at_fn(ray r, float t);

#endif // RAY_H
