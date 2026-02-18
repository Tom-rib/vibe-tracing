/**
 * material.h - Material properties
 * 
 * Defines material properties for surfaces.
 * Used in shading calculations (Lambert, Phong, Fresnel, etc).
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math/vec3.h"
#include "color.h"

/* ============================================================================
   MATERIAL STRUCTURE
   ============================================================================ */

/**
 * Material properties for surface shading.
 * 
 * Fields:
 * - albedo:     Base color of surface [0, 1]
 * - roughness:  Surface roughness [0, 1] (0 = mirror, 1 = diffuse)
 * - metallic:   Metallicness [0, 1] (0 = dielectric, 1 = metal)
 * - ior:        Index of refraction (1.0 = air, 1.5 = glass, 2.4 = diamond)
 * - emission:   Emitted light color [0, 1] (for light-emitting surfaces)
 */
typedef struct {
    color albedo;       // Base color/albedo
    float roughness;    // Roughness [0, 1]
    float metallic;     // Metallicness [0, 1]
    float ior;          // Index of refraction
    color emission;     // Emitted color (phase 6+)
} material;

/* ============================================================================
   MATERIAL PRESETS
   ============================================================================ */

/** Matte white material */
static inline material material_matte_white(void) {
    return (material){
        .albedo = color_white(),
        .roughness = 0.8f,
        .metallic = 0.0f,
        .ior = 1.0f,
        .emission = color_black()
    };
}

/** Matte red material */
static inline material material_matte_red(void) {
    return (material){
        .albedo = color_red(),
        .roughness = 0.8f,
        .metallic = 0.0f,
        .ior = 1.0f,
        .emission = color_black()
    };
}

/** Matte blue material */
static inline material material_matte_blue(void) {
    return (material){
        .albedo = color_blue(),
        .roughness = 0.8f,
        .metallic = 0.0f,
        .ior = 1.0f,
        .emission = color_black()
    };
}

/** Mirror/reflective material */
static inline material material_mirror(void) {
    return (material){
        .albedo = color_white(),
        .roughness = 0.0f,
        .metallic = 1.0f,
        .ior = 1.0f,
        .emission = color_black()
    };
}

/** Glass material (IOR = 1.5) */
static inline material material_glass(void) {
    return (material){
        .albedo = color_white(),
        .roughness = 0.0f,
        .metallic = 0.0f,
        .ior = 1.5f,
        .emission = color_black()
    };
}

/** Diamond material (IOR = 2.4) */
static inline material material_diamond(void) {
    return (material){
        .albedo = color_white(),
        .roughness = 0.0f,
        .metallic = 0.0f,
        .ior = 2.4f,
        .emission = color_black()
    };
}

#endif // MATERIAL_H
