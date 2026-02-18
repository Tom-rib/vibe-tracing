/**
 * camera.c - Camera implementation
 */

#include "camera.h"
#include <math.h>

/**
 * Create camera with default orientation (looking down -Z).
 */
camera camera_create(int width, int height, float fov) {
    float fov_radians = fov * 3.14159265f / 180.0f;
    float plane_dist = 1.0f / tanf(fov_radians * 0.5f);
    
    camera cam = {
        .position = {0.0f, 0.0f, 0.0f, 0.0f},
        .forward = {0.0f, 0.0f, -1.0f, 0.0f},
        .right = {1.0f, 0.0f, 0.0f, 0.0f},
        .up = {0.0f, 1.0f, 0.0f, 0.0f},
        .fov = fov,
        .width = width,
        .height = height,
        .plane_dist = plane_dist
    };
    
    return cam;
}

/**
 * Create camera looking at target with custom up direction.
 */
camera camera_create_look_at(
    vec3 position, vec3 target, vec3 up_hint,
    float fov, int width, int height
) {
    // Compute forward (from position to target)
    vec3 forward = vec3_normalize(vec3_sub(target, position));
    
    // Compute right = forward × up
    vec3 right = vec3_normalize(vec3_cross(forward, up_hint));
    
    // Recompute up = right × forward
    vec3 up = vec3_cross(right, forward);
    
    float fov_radians = fov * 3.14159265f / 180.0f;
    float plane_dist = 1.0f / tanf(fov_radians * 0.5f);
    
    camera cam = {
        .position = position,
        .forward = forward,
        .right = right,
        .up = up,
        .fov = fov,
        .width = width,
        .height = height,
        .plane_dist = plane_dist
    };
    
    return cam;
}

/**
 * Generate ray for pixel (x, y).
 * Converts pixel coordinates to normalized device coordinates,
 * then transforms to world space.
 */
ray camera_ray(const camera *cam, int x, int y) {
    // Normalized device coordinates [-1, 1]
    float ndc_x = 2.0f * x / (float)cam->width - 1.0f;
    float ndc_y = 1.0f - 2.0f * y / (float)cam->height;  // Flip Y
    
    // Account for aspect ratio
    float aspect = (float)cam->width / (float)cam->height;
    ndc_x *= aspect;
    
    // Compute ray direction in camera space
    vec3 dir = vec3_normalize(vec3_add(
        vec3_add(
            vec3_scale(cam->forward, cam->plane_dist),
            vec3_scale(cam->right, ndc_x)
        ),
        vec3_scale(cam->up, ndc_y)
    ));
    
    return ray_create(cam->position, dir);
}

/**
 * Generate ray with sub-pixel offset for antialiasing.
 */
ray camera_ray_offset(const camera *cam, int x, int y, float dx, float dy) {
    // Add sub-pixel offset to coordinates
    float fx = (float)x + dx;
    float fy = (float)y + dy;
    
    // Normalized device coordinates [-1, 1]
    float ndc_x = 2.0f * fx / (float)cam->width - 1.0f;
    float ndc_y = 1.0f - 2.0f * fy / (float)cam->height;
    
    // Account for aspect ratio
    float aspect = (float)cam->width / (float)cam->height;
    ndc_x *= aspect;
    
    // Compute ray direction in camera space
    vec3 dir = vec3_normalize(vec3_add(
        vec3_add(
            vec3_scale(cam->forward, cam->plane_dist),
            vec3_scale(cam->right, ndc_x)
        ),
        vec3_scale(cam->up, ndc_y)
    ));
    
    return ray_create(cam->position, dir);
}
