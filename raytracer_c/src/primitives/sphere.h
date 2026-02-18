/**
 * sphere.h - Sphere primitive and ray-sphere intersection
 * 
 * Implements analytical ray-sphere intersection using the quadratic formula.
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "../math/vec3.h"
#include "../core/ray.h"
#include "../core/material.h"

/* ============================================================================
   HIT RECORD
   ============================================================================ */

/**
 * Information about a ray-surface intersection.
 * Returned by intersection tests.
 */
typedef struct {
    vec3 position;      // Intersection point in world space
    vec3 normal;        // Surface normal (normalized, pointing outward)
    float t;            // Parameter t where ray hits (ray_at(t) = position)
    material mat;       // Surface material
} hit_record;

/* ============================================================================
   SPHERE STRUCTURE
   ============================================================================ */

/**
 * Sphere primitive.
 * Center point and radius define the sphere surface.
 */
typedef struct {
    vec3 center;        // Center position
    float radius;       // Radius
    material mat;       // Material properties
} sphere;

/* ============================================================================
   CONSTRUCTORS
   ============================================================================ */

/** Create sphere at position with radius and material */
static inline sphere sphere_create(vec3 center, float radius, material mat) {
    return (sphere){center, radius, mat};
}

/* ============================================================================
   INTERSECTION
   ============================================================================ */

/**
 * Test ray-sphere intersection.
 * @param r         Ray
 * @param s         Sphere
 * @param out       Pointer to output hit_record (filled if hit)
 * @return          1 if ray hits sphere, 0 otherwise
 * 
 * If ray hits, returns minimum t > epsilon.
 * Fills out with hit position, normal, t, and material.
 * Normal always points outward from sphere.
 * 
 * Algorithm: Solve |ray.origin + t * ray.direction - sphere.center|² = radius²
 * Results in quadratic: a*t² + b*t + c = 0
 * where a = dot(direction, direction)
 *       b = 2 * dot(direction, oc)  [oc = origin - center]
 *       c = dot(oc, oc) - radius²
 */
int sphere_intersect(ray r, sphere s, hit_record *out);

/**
 * Test ray-sphere intersection with custom epsilon.
 * @param r         Ray
 * @param s         Sphere
 * @param epsilon   Minimum t value (avoids shadow acne)
 * @param out       Pointer to output hit_record
 * @return          1 if ray hits, 0 otherwise
 */
int sphere_intersect_epsilon(ray r, sphere s, float epsilon, hit_record *out);

#endif // SPHERE_H
