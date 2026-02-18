/**
 * image.h - Image buffer and PPM output
 * 
 * Stores pixel data and provides PPM (Portable Pixmap) output.
 * Format: P6 binary (24-bit RGB, big-endian)
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include "../core/color.h"

/* ============================================================================
   IMAGE STRUCTURE
   ============================================================================ */

/**
 * Image buffer with width × height pixels.
 * Pixels are stored as contiguous RGB bytes (24-bit color).
 */
typedef struct {
    uint8_t *pixels;    // RGB pixel data (width × height × 3 bytes)
    int width;
    int height;
} image;

/* ============================================================================
   CREATION & DESTRUCTION
   ============================================================================ */

/**
 * Create image with given dimensions.
 * @param width     Image width (pixels)
 * @param height    Image height (pixels)
 * @return          Allocated image (pixels initialized to 0)
 * 
 * Allocates width × height × 3 bytes for RGB data.
 */
image image_create(int width, int height);

/**
 * Destroy image and free memory.
 * @param img       Image to destroy
 */
void image_destroy(image *img);

/* ============================================================================
   PIXEL ACCESS
   ============================================================================ */

/**
 * Set pixel color.
 * @param img       Image
 * @param x         X coordinate [0, width)
 * @param y         Y coordinate [0, height)
 * @param c         Color [0, 1]
 * 
 * Color is clamped to [0, 1] and converted to [0, 255].
 */
void image_set_pixel(image *img, int x, int y, color c);

/**
 * Set pixel with byte values.
 * @param img       Image
 * @param x         X coordinate
 * @param y         Y coordinate
 * @param r, g, b   Byte values [0, 255]
 */
void image_set_pixel_bytes(image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b);

/**
 * Get pixel color.
 * @param img       Image
 * @param x         X coordinate
 * @param y         Y coordinate
 * @return          Color [0, 1] (approximated from bytes)
 */
color image_get_pixel(const image *img, int x, int y);

/* ============================================================================
   I/O
   ============================================================================ */

/**
 * Write image to PPM file (P6 binary format).
 * @param img           Image
 * @param filename      Output filename
 * @return              0 on success, -1 on error
 * 
 * PPM P6 format:
 * P6
 * width height
 * 255
 * [binary RGB bytes]
 */
int image_write_ppm(const image *img, const char *filename);

/**
 * Read image from PPM file (P6 format only).
 * @param filename      Input filename
 * @return              Loaded image, or empty image on error
 */
image image_read_ppm(const char *filename);

/* ============================================================================
   UTILITIES
   ============================================================================ */

/**
 * Clear image to color.
 * @param img       Image
 * @param c         Color to fill
 */
void image_clear(image *img, color c);

/**
 * Fill rectangle with color.
 * @param img       Image
 * @param x0, y0    Top-left corner
 * @param x1, y1    Bottom-right corner
 * @param c         Color
 */
void image_fill_rect(image *img, int x0, int y0, int x1, int y1, color c);

#endif // IMAGE_H
