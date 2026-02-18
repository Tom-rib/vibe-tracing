/**
 * sphere.c - Sphere intersection implementation
 */

#include "sphere.h"
#include <math.h>

/**
 * Test ray-sphere intersection using default epsilon.
 */
int sphere_intersect(ray r, sphere s, hit_record *out) {
    return sphere_intersect_epsilon(r, s, 1e-6f, out);
}

/**
 * Test ray-sphere intersection with custom epsilon.
 * Solves |ray.origin + t * ray.direction - sphere.center|² = radius²
 */
int sphere_intersect_epsilon(ray r, sphere s, float epsilon, hit_record *out) {
    // Vector from ray origin to sphere center
    vec3 oc = vec3_sub(r.origin, s.center);
    
    // Quadratic equation: a*t² + b*t + c = 0
    // a = dot(direction, direction)
    // b = 2 * dot(direction, oc)
    // c = dot(oc, oc) - radius²
    
    float a = vec3_dot(r.direction, r.direction);
    float b = 2.0f * vec3_dot(r.direction, oc);
    float c = vec3_dot(oc, oc) - s.radius * s.radius;
    
    // Discriminant
    float discriminant = b * b - 4.0f * a * c;
    
    if (discriminant < 0.0f) {
        return 0;  // No intersection
    }
    
    // Two solutions
    float sqrt_disc = sqrtf(discriminant);
    float t1 = (-b - sqrt_disc) / (2.0f * a);
    float t2 = (-b + sqrt_disc) / (2.0f * a);
    
    // Find minimum positive t
    float t = -1.0f;
    if (t1 > epsilon) {
        t = t1;
    } else if (t2 > epsilon) {
        t = t2;
    } else {
        return 0;  // No positive intersection
    }
    
    // Fill hit record
    out->t = t;
    out->position = ray_at(r, t);
    out->normal = vec3_normalize(vec3_sub(out->position, s.center));
    out->mat = s.mat;
    
    return 1;
}
