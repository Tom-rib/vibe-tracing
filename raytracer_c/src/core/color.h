/**
 * color.h - Color utilities and conversions
 * 
 * Colors are represented as vec3 with components in [0, 1] range.
 * Conversion to uint8 [0, 255] for PPM output.
 */

#ifndef COLOR_H
#define COLOR_H

#include "../math/vec3.h"
#include <stdint.h>

/* ============================================================================
   COLOR TYPE
   ============================================================================ */

/** Color is just a vec3 with normalized components [0, 1] */
typedef vec3 color;

/* ============================================================================
   CONSTRUCTORS
   ============================================================================ */

/** Create color from normalized components [0, 1] */
static inline color color_create(float r, float g, float b) {
    return vec3_create(r, g, b);
}

/** Black (0, 0, 0) */
static inline color color_black(void) {
    return vec3_zero();
}

/** White (1, 1, 1) */
static inline color color_white(void) {
    return vec3_one();
}

/** Red (1, 0, 0) */
static inline color color_red(void) {
    return vec3_create(1.0f, 0.0f, 0.0f);
}

/** Green (0, 1, 0) */
static inline color color_green(void) {
    return vec3_create(0.0f, 1.0f, 0.0f);
}

/** Blue (0, 0, 1) */
static inline color color_blue(void) {
    return vec3_create(0.0f, 0.0f, 1.0f);
}

/* ============================================================================
   CONVERSION & UTILITIES
   ============================================================================ */

/**
 * Convert color [0, 1] to 24-bit RGB packed integer (0xRRGGBB).
 * Components are clamped to [0, 1] then scaled to [0, 255].
 */
static inline uint32_t color_to_rgb24(color c) {
    // Clamp to [0, 1]
    vec3 clamped = vec3_clamp(c, 0.0f, 1.0f);
    
    // Convert to [0, 255]
    uint8_t r = (uint8_t)(clamped.x * 255.99f);
    uint8_t g = (uint8_t)(clamped.y * 255.99f);
    uint8_t b = (uint8_t)(clamped.z * 255.99f);
    
    // Pack as 0xRRGGBB
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

/**
 * Convert color [0, 1] to byte triplet [0, 255].
 * Stores result in r, g, b pointers.
 */
static inline void color_to_bytes(color c, uint8_t *r, uint8_t *g, uint8_t *b) {
    vec3 clamped = vec3_clamp(c, 0.0f, 1.0f);
    *r = (uint8_t)(clamped.x * 255.99f);
    *g = (uint8_t)(clamped.y * 255.99f);
    *b = (uint8_t)(clamped.z * 255.99f);
}

/**
 * Blend two colors with parameter t [0, 1].
 * Result: a * (1 - t) + b * t
 */
static inline color color_blend(color a, color b, float t) {
    return vec3_add(
        vec3_scale(a, 1.0f - t),
        vec3_scale(b, t)
    );
}

/**
 * Clamp color components to [0, 1].
 * Useful after computations that might exceed bounds.
 */
static inline color color_clamp(color c) {
    return vec3_clamp(c, 0.0f, 1.0f);
}

/**
 * Get luminance of color (standard formula).
 * Formula: 0.299 * R + 0.587 * G + 0.114 * B
 */
static inline float color_luminance(color c) {
    return 0.299f * c.x + 0.587f * c.y + 0.114f * c.z;
}

#endif // COLOR_H
