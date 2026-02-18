/**
 * test_phase1.c - Unit tests for Phase 1 components
 * 
 * Basic validation that core Phase 1 modules work correctly.
 * Compile with: gcc -O2 -std=c99 -Wall -lm src/math/vec3.c src/core/ray.c test_phase1.c -o test_phase1
 */

#include <stdio.h>
#include <math.h>
#include "src/math/vec3.h"
#include "src/core/ray.h"
#include "src/core/color.h"
#include "src/core/material.h"
#include "src/primitives/sphere.h"

/* ============================================================================
   TEST UTILITIES
   ============================================================================ */

int test_count = 0;
int test_passed = 0;
int test_failed = 0;

#define EPSILON 1e-5f

#define TEST(name, expr) \
    do { \
        test_count++; \
        if (expr) { \
            test_passed++; \
            printf("  ✓ %s\n", name); \
        } else { \
            test_failed++; \
            printf("  ✗ %s\n", name); \
        } \
    } while (0)

#define TEST_FLOAT(name, actual, expected) \
    TEST(name, fabsf(actual - expected) < EPSILON)

#define TEST_VEC3(name, a, b) \
    TEST(name, vec3_approx_equal(a, b, EPSILON))

/* ============================================================================
   TESTS: VEC3
   ============================================================================ */

void test_vec3(void) {
    printf("\n=== Testing vec3 ===\n");
    
    vec3 a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3 b = vec3_create(4.0f, 5.0f, 6.0f);
    
    // Basic operations
    TEST("vec3_create", a.x == 1.0f && a.y == 2.0f && a.z == 3.0f);
    
    vec3 sum = vec3_add(a, b);
    TEST_VEC3("vec3_add", sum, vec3_create(5.0f, 7.0f, 9.0f));
    
    vec3 diff = vec3_sub(b, a);
    TEST_VEC3("vec3_sub", diff, vec3_create(3.0f, 3.0f, 3.0f));
    
    vec3 scaled = vec3_scale(a, 2.0f);
    TEST_VEC3("vec3_scale", scaled, vec3_create(2.0f, 4.0f, 6.0f));
    
    // Dot and cross
    float dot = vec3_dot(a, b);
    TEST_FLOAT("vec3_dot", dot, 32.0f);  // 1*4 + 2*5 + 3*6 = 32
    
    vec3 cross = vec3_cross(a, b);
    TEST_VEC3("vec3_cross", cross, vec3_create(-3.0f, 6.0f, -3.0f));
    
    // Length
    float len_sq = vec3_length_squared(vec3_create(3.0f, 4.0f, 0.0f));
    TEST_FLOAT("vec3_length_squared", len_sq, 25.0f);
    
    float len = vec3_length(vec3_create(3.0f, 4.0f, 0.0f));
    TEST_FLOAT("vec3_length", len, 5.0f);
    
    // Normalize
    vec3 normalized = vec3_normalize(vec3_create(3.0f, 4.0f, 0.0f));
    TEST_VEC3("vec3_normalize", normalized, vec3_create(0.6f, 0.8f, 0.0f));
    
    // Reflection
    vec3 dir = vec3_create(1.0f, -1.0f, 0.0f);
    vec3 normal = vec3_create(0.0f, 1.0f, 0.0f);
    vec3 reflected = vec3_reflect(dir, normal);
    TEST_VEC3("vec3_reflect", reflected, vec3_create(1.0f, 1.0f, 0.0f));
    
    printf("  → vec3: %d passed, %d failed\n", 
        test_passed, test_failed);
}

/* ============================================================================
   TESTS: RAY
   ============================================================================ */

void test_ray(void) {
    printf("\n=== Testing ray ===\n");
    
    ray r = ray_create(
        vec3_create(0.0f, 0.0f, 0.0f),
        vec3_create(1.0f, 0.0f, 0.0f)
    );
    
    // Ray at t=0 should be origin
    TEST_VEC3("ray_at(0)", ray_at(r, 0.0f), r.origin);
    
    // Ray at t=1 should be origin + direction
    TEST_VEC3("ray_at(1)", ray_at(r, 1.0f), 
              vec3_create(1.0f, 0.0f, 0.0f));
    
    // Ray at t=5
    TEST_VEC3("ray_at(5)", ray_at(r, 5.0f),
              vec3_create(5.0f, 0.0f, 0.0f));
    
    printf("  → ray: %d passed, %d failed\n",
        test_passed, test_failed);
}

/* ============================================================================
   TESTS: COLOR
   ============================================================================ */

void test_color(void) {
    printf("\n=== Testing color ===\n");
    
    color c = color_create(0.5f, 0.5f, 0.5f);
    TEST("color_create", c.x == 0.5f && c.y == 0.5f && c.z == 0.5f);
    
    uint32_t rgb = color_to_rgb24(color_create(1.0f, 0.0f, 0.0f));
    TEST("color_to_rgb24(red)", rgb == 0xFF0000);
    
    uint8_t r, g, b;
    color_to_bytes(color_create(1.0f, 0.5f, 0.0f), &r, &g, &b);
    TEST("color_to_bytes", r == 255 && g >= 127 && g <= 128 && b == 0);
    
    float lum = color_luminance(color_white());
    TEST_FLOAT("color_luminance(white)", lum, 1.0f);
    
    printf("  → color: %d passed, %d failed\n",
        test_passed, test_failed);
}

/* ============================================================================
   TESTS: SPHERE INTERSECTION
   ============================================================================ */

void test_sphere_intersection(void) {
    printf("\n=== Testing sphere intersection ===\n");
    
    sphere s = sphere_create(
        vec3_create(0.0f, 0.0f, -5.0f),
        1.0f,
        material_matte_white()
    );
    
    // Ray hitting sphere head-on
    ray r = ray_create(
        vec3_create(0.0f, 0.0f, 0.0f),
        vec3_create(0.0f, 0.0f, -1.0f)
    );
    
    hit_record hit;
    int does_hit = sphere_intersect(r, s, &hit);
    TEST("sphere_intersect (should hit)", does_hit);
    TEST_FLOAT("sphere_intersect (t value)", hit.t, 4.0f);  // Hits at z=-4
    
    // Ray missing sphere
    ray r_miss = ray_create(
        vec3_create(10.0f, 10.0f, 0.0f),
        vec3_create(0.0f, 0.0f, -1.0f)
    );
    
    int doesnt_hit = !sphere_intersect(r_miss, s, &hit);
    TEST("sphere_intersect (should miss)", doesnt_hit);
    
    // Ray from inside sphere
    ray r_inside = ray_create(
        vec3_create(0.0f, 0.0f, -5.0f),
        vec3_create(0.0f, 0.0f, -1.0f)
    );
    
    hit_record hit2;
    int hit_inside = sphere_intersect(r_inside, s, &hit2);
    TEST("sphere_intersect (from inside)", hit_inside);
    TEST_FLOAT("sphere_intersect (from inside, t)", hit2.t, 1.0f);  // Hits at z=-6
    
    printf("  → sphere intersection: %d passed, %d failed\n",
        test_passed, test_failed);
}

/* ============================================================================
   MAIN
   ============================================================================ */

int main(void) {
    printf("===============================================\n");
    printf("  Phase 1 Unit Tests\n");
    printf("===============================================\n");
    
    test_vec3();
    test_ray();
    test_color();
    test_sphere_intersection();
    
    printf("\n===============================================\n");
    printf("  Summary\n");
    printf("===============================================\n");
    printf("Total:  %d tests\n", test_count);
    printf("Passed: %d tests\n", test_passed);
    printf("Failed: %d tests\n", test_failed);
    printf("===============================================\n");
    
    return test_failed == 0 ? 0 : 1;
}
