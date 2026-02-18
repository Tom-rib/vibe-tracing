/**
 * image.c - Image implementation
 */

#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Create image with given dimensions.
 */
image image_create(int width, int height) {
    image img = {
        .width = width,
        .height = height,
        .pixels = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t))
    };
    return img;
}

/**
 * Destroy image and free memory.
 */
void image_destroy(image *img) {
    if (img && img->pixels) {
        free(img->pixels);
        img->pixels = NULL;
    }
}

/**
 * Set pixel color (from float [0, 1]).
 */
void image_set_pixel(image *img, int x, int y, color c) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return;  // Out of bounds
    }
    
    uint8_t r, g, b;
    color_to_bytes(c, &r, &g, &b);
    
    int idx = (y * img->width + x) * 3;
    img->pixels[idx + 0] = r;
    img->pixels[idx + 1] = g;
    img->pixels[idx + 2] = b;
}

/**
 * Set pixel with byte values.
 */
void image_set_pixel_bytes(image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return;
    }
    
    int idx = (y * img->width + x) * 3;
    img->pixels[idx + 0] = r;
    img->pixels[idx + 1] = g;
    img->pixels[idx + 2] = b;
}

/**
 * Get pixel color.
 */
color image_get_pixel(const image *img, int x, int y) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return color_black();
    }
    
    int idx = (y * img->width + x) * 3;
    float r = img->pixels[idx + 0] / 255.0f;
    float g = img->pixels[idx + 1] / 255.0f;
    float b = img->pixels[idx + 2] / 255.0f;
    
    return color_create(r, g, b);
}

/**
 * Write image to PPM file (P6 binary format).
 */
int image_write_ppm(const image *img, const char *filename) {
    if (!img || !img->pixels || !filename) {
        return -1;
    }
    
    FILE *f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s' for writing\n", filename);
        return -1;
    }
    
    // Write PPM header
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", img->width, img->height);
    fprintf(f, "255\n");
    
    // Write pixel data
    size_t written = fwrite(img->pixels, 1, img->width * img->height * 3, f);
    if (written != (size_t)(img->width * img->height * 3)) {
        fprintf(stderr, "Error: failed to write all pixel data\n");
        fclose(f);
        return -1;
    }
    
    fclose(f);
    return 0;
}

/**
 * Read image from PPM file (P6 format only).
 */
image image_read_ppm(const char *filename) {
    image img = {0};
    
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s' for reading\n", filename);
        return img;
    }
    
    // Read header
    char magic[3];
    if (fscanf(f, "%2s\n", magic) != 1 || magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Error: not a valid PPM P6 file\n");
        fclose(f);
        return img;
    }
    
    // Skip comments
    int c;
    while ((c = fgetc(f)) == '#') {
        while ((c = fgetc(f)) != '\n' && c != EOF);
    }
    ungetc(c, f);
    
    // Read dimensions
    int max_val;
    if (fscanf(f, "%d %d %d\n", &img.width, &img.height, &max_val) != 3) {
        fprintf(stderr, "Error: invalid PPM header\n");
        fclose(f);
        return (image){0};
    }
    
    if (max_val != 255) {
        fprintf(stderr, "Error: PPM max value must be 255\n");
        fclose(f);
        return (image){0};
    }
    
    // Allocate and read pixels
    img.pixels = (uint8_t *)malloc(img.width * img.height * 3);
    if (!img.pixels) {
        fprintf(stderr, "Error: cannot allocate image memory\n");
        fclose(f);
        return (image){0};
    }
    
    size_t read = fread(img.pixels, 1, img.width * img.height * 3, f);
    if (read != (size_t)(img.width * img.height * 3)) {
        fprintf(stderr, "Error: failed to read all pixel data\n");
        free(img.pixels);
        img.pixels = NULL;
        fclose(f);
        return (image){0};
    }
    
    fclose(f);
    return img;
}

/**
 * Clear image to color.
 */
void image_clear(image *img, color c) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            image_set_pixel(img, x, y, c);
        }
    }
}

/**
 * Fill rectangle with color.
 */
void image_fill_rect(image *img, int x0, int y0, int x1, int y1, color c) {
    // Clamp bounds
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > img->width) x1 = img->width;
    if (y1 > img->height) y1 = img->height;
    
    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            image_set_pixel(img, x, y, c);
        }
    }
}
