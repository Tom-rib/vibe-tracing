/**
 * main.c - Main entry point for Raytracer
 * 
 * Phase 1: Basic raytracing with flat shading
 * Renders a single sphere with simple diffuse lighting based on normal.
 */

#include "config.h"
#include "math/vec3.h"
#include "core/ray.h"
#include "core/camera.h"
#include "core/image.h"
#include "core/color.h"
#include "core/material.h"
#include "primitives/sphere.h"

#include <stdio.h>
#include <time.h>

/* ============================================================================
   MAIN RAYTRACING FUNCTION
   ============================================================================ */

/**
 * Simple flat shading: color based on surface normal.
 * Simulates basic diffuse lighting without explicit light source.
 */
color shade_flat(hit_record hit, ray r) {
    // Simple diffuse based on surface normal
    // Formula: 0.5 + 0.5 * (normal · -ray_direction)
    // This gives a smooth gradient based on surface angle
    
    vec3 view_dir = vec3_normalize(vec3_negate(r.direction));
    float diffuse = 0.5f + 0.5f * vec3_dot(hit.normal, view_dir);
    
    // Apply to material color
    color result = vec3_scale(hit.mat.albedo, diffuse);
    
    return result;
}

/**
 * Main rendering loop: cast ray for each pixel.
 */
void render(image *img, camera *cam, sphere *spheres, int num_spheres) {
    printf("Rendering %d × %d pixels...\n", img->width, img->height);
    
    int total_pixels = img->width * img->height;
    int pixels_done = 0;
    
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Cast ray for this pixel
            ray r = camera_ray(cam, x, y);
            
            // Find closest intersection
            hit_record closest_hit = {0};
            int hit_something = 0;
            float closest_t = 1e6f;
            
            for (int i = 0; i < num_spheres; i++) {
                hit_record hit;
                if (sphere_intersect(r, spheres[i], &hit)) {
                    if (hit.t < closest_t) {
                        closest_t = hit.t;
                        closest_hit = hit;
                        hit_something = 1;
                    }
                }
            }
            
            // Shade pixel
            color pixel_color;
            if (hit_something) {
                pixel_color = shade_flat(closest_hit, r);
            } else {
                // Background: black
                pixel_color = color_black();
            }
            
            // Store pixel
            image_set_pixel(img, x, y, pixel_color);
            
            // Progress indicator every 100 pixels
            pixels_done++;
            if (pixels_done % 1000 == 0) {
                printf("  %d / %d pixels done (%.1f%%)\n", 
                    pixels_done, total_pixels, 
                    100.0f * pixels_done / total_pixels);
            }
        }
    }
    
    printf("Rendering complete!\n");
}

/* ============================================================================
   MAIN
   ============================================================================ */

int main(void) {
    printf("=== Raytracer - Phase 1 ===\n");
    printf("Basic raytracing with flat shading\n\n");
    
    // Timing
    clock_t start = clock();
    
    // Create image
    printf("Creating image (%d × %d)...\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    image img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
    
    // Create camera
    printf("Setting up camera (FOV %.1f°)...\n", DEFAULT_FOV);
    camera cam = camera_create(IMAGE_WIDTH, IMAGE_HEIGHT, DEFAULT_FOV);
    
    // Create scene: simple sphere in the middle
    printf("Setting up scene...\n");
    sphere spheres[10];
    int num_spheres = 0;
    
    // Main white sphere in the center
    spheres[num_spheres++] = sphere_create(
        vec3_create(0.0f, 0.0f, -5.0f),  // Position
        1.0f,                              // Radius
        material_matte_white()             // Material
    );
    
    // Optional: Add more spheres for testing
    /*
    spheres[num_spheres++] = sphere_create(
        vec3_create(-2.0f, 0.0f, -6.0f),
        0.8f,
        material_matte_red()
    );
    
    spheres[num_spheres++] = sphere_create(
        vec3_create(2.0f, 0.0f, -6.0f),
        0.8f,
        material_matte_blue()
    );
    */
    
    printf("Scene has %d sphere(s)\n", num_spheres);
    
    // Render
    printf("\n");
    render(&img, &cam, spheres, num_spheres);
    
    // Save image
    printf("\nSaving image to output.ppm...\n");
    if (image_write_ppm(&img, "output.ppm") == 0) {
        printf("✓ Image saved successfully\n");
    } else {
        printf("✗ Error saving image\n");
    }
    
    // Cleanup
    image_destroy(&img);
    
    // Timing
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nRender time: %.2f seconds\n", elapsed);
    printf("Pixels per second: %.0f\n", (IMAGE_WIDTH * IMAGE_HEIGHT) / elapsed);
    
    return 0;
}
