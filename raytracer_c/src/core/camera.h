/**
 * camera.h - Pinhole camera model
 * 
 * Generates rays for perspective projection. Camera is positioned
 * and oriented in 3D space, and generates a ray for each pixel.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vec3.h"
#include "ray.h"

/* ============================================================================
   CAMERA STRUCTURE
   ============================================================================ */

/**
 * Pinhole camera for perspective projection.
 * Generates rays from camera position through image plane.
 */
typedef struct {
    vec3 position;      // Camera position in world space
    vec3 forward;       // Forward direction (view direction)
    vec3 right;         // Right direction (image x-axis)
    vec3 up;            // Up direction (image y-axis)
    float fov;          // Field of view (vertical, in degrees)
    int width, height;  // Image resolution
    float plane_dist;   // Distance to image plane (computed from FOV)
} camera;

/* ============================================================================
   CONSTRUCTORS & SETUP
   ============================================================================ */

/**
 * Create camera with default orientation.
 * @param width     Image width (pixels)
 * @param height    Image height (pixels)
 * @param fov       Vertical field of view (degrees)
 * @return          Camera looking down -Z axis
 * 
 * Default camera:
 * - Position: (0, 0, 0)
 * - Forward: (0, 0, -1) (looking down -Z)
 * - Right: (1, 0, 0)
 * - Up: (0, 1, 0)
 */
camera camera_create(int width, int height, float fov);

/**
 * Create camera with custom position and target.
 * @param position  Camera position in world space
 * @param target    Point to look at
 * @param up        Up direction (for camera roll)
 * @param fov       Vertical field of view (degrees)
 * @param width     Image width
 * @param height    Image height
 * @return          Oriented camera
 */
camera camera_create_look_at(
    vec3 position, vec3 target, vec3 up,
    float fov, int width, int height
);

/* ============================================================================
   RAY GENERATION
   ============================================================================ */

/**
 * Generate ray for pixel (x, y).
 * @param cam       Camera
 * @param x         Pixel X coordinate [0, width)
 * @param y         Pixel Y coordinate [0, height)
 * @return          Ray from camera through pixel
 * 
 * Ray origin is camera position.
 * Ray direction is normalized.
 * (0, 0) is top-left, (width-1, height-1) is bottom-right.
 */
ray camera_ray(const camera *cam, int x, int y);

/**
 * Generate ray with sub-pixel offset (for antialiasing).
 * @param cam       Camera
 * @param x         Pixel X coordinate
 * @param y         Pixel Y coordinate
 * @param dx        Sub-pixel offset X [0, 1]
 * @param dy        Sub-pixel offset Y [0, 1]
 * @return          Ray from camera through subpixel
 */
ray camera_ray_offset(const camera *cam, int x, int y, float dx, float dy);

#endif // CAMERA_H
