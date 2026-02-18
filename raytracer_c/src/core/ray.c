/**
 * ray.c - Ray implementation
 */

#include "ray.h"

/**
 * Get point on ray at parameter t.
 * Non-inline version for use in callbacks or testing.
 */
vec3 ray_at_fn(ray r, float t) {
    return ray_at(r, t);
}
