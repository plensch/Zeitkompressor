#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
 * TODO
 * add proper cli arguments
 * add invert argument
 * remove assumptions
 * lazy load in rgb-mode too
 * fix double-free in sequence mode
 *
 * ASSUMPTIONS:
 * three channel bitmap images (RGB)
 * 256 images in image sequence
 */

/* find unique pixel values to only load needed images from image sequence */
void tm_unique (unsigned char *map, int *unique, int x, int y, int n) {
    unsigned char gray = 0;
    unsigned char *p = map;
    for (int i = 0; i < (x * y * n); i += n) {
        gray = (*(p+i) + *(p+i+1) + *(p+i+2)) / 3;
        if (unique[gray] == 0) {
            unique[gray] = 1;
        }
    }
}

/* load image sequence */
int tm_load (unsigned char **imgs, char *imgs_path, int *unique, int rgb, int cx, int cy, int cn, int all) {
    int x,y,n;
    char imgpath[256]; // TODO make dynamic
    for (int i = 0; i < 256; i++) {
        if (all || (rgb ? 1 : unique[i])) { 
            sprintf(imgpath, "%s%03d.bmp",imgs_path, i);
            imgs[i] = stbi_load(imgpath, &x, &y, &n, 0);
            if (imgs[i] == 0) {
                fprintf(stderr, "Could not open image %s\n", imgpath);
                return 1;
            }
            if ((n != cn) || (y != cy) || (x != cx)) {
                fprintf(stderr, "Image format does not match map (width, height, channels): %s\n", imgpath);
                return 1;
            }
        }
    }
    return 0;
}

/* tmap: map image sequence to canvas */
void tm_map (unsigned char *map, unsigned char *canvas, unsigned char **imgs, int iterations,\
             int rgb, int x, int y, int n) {
    unsigned char gray = 0;
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    unsigned char *m;
    unsigned char *c;
    for (int i = 0; i < iterations; i++) {
        if (i < 1) {
            m = map;
            c = canvas;
        } else {
            m = canvas;
            c = canvas;
        }
        for (int i = 0; i < (x * y * n); i += n) {
            if (rgb) {
                r = *(m+i);
                g = *(m+i+1);
                b = *(m+i+2);
                *(c+i)   = *(imgs[r]+i);
                *(c+i+1) = *(imgs[g]+i+1);
                *(c+i+2) = *(imgs[b]+i+2);
            } else {
                gray = (*(m+i) + *(m+i+1) + *(m+i+2)) / 3;
                *(c+i)   = *(imgs[gray]+i);
                *(c+i+1) = *(imgs[gray]+i+1);
                *(c+i+2) = *(imgs[gray]+i+2);
            }
        }
    }
}

int main (int argc, char *argv[]) {
    if (argc < 6) {
        printf("Usage: %s [map] [image sequence folder] [iterations] [rgb/grayscale] [sequence/static] [out]\n"
                "Images in the sequence should be labeled: 000.bmp, 001.bmp ... 255.bmp\n"
                "(rgb = 1, grayscale = 0)\n"
                "(sequence = 1, static = 0)\n"
                "When sequence = 1 then [out] should be a directory\n", argv[0]); 
        return 1;
    }

    // options
    char *map_path = argv[1];
    char *imgs_path = argv[2];
    int iterations = atoi(argv[3]);
    int rgb = atoi(argv[4]);
    int sequence = atoi(argv[5]);
    char *out_path = argv[6];

    // load map
    int cx,cy,cn;
    unsigned char *map = stbi_load(map_path, &cx, &cy, &cn, 0);
    if (map == 0) {
        fprintf(stderr, "Could not open map %s\n", argv[1]);
        return 1;
    }

    // find which images are needed
    int unique[256] = {0};
    if (!sequence) {
        tm_unique(map, unique, cx, cy, cn);
    }

    // load images
    unsigned char *imgs[256] = {0};
    if (tm_load(imgs, imgs_path, unique, rgb, cx, cy, cn, sequence)) {
        stbi_image_free(map);
        return 1; // passthrough error
    }

    unsigned char *canvas = malloc(cx * cy * cn);
    char outpath[256]; // TODO make dynamic
    if (!sequence) {
        // map image sequence to canvas
        tm_map(map, canvas, imgs, iterations, rgb, cx, cy, cn);
        // save canvas
        stbi_write_bmp(out_path, cx, cy, cn, canvas);
    } else {
        for (int i = 0; i < 256; i++) {
            map = imgs[i];
            tm_map(map, canvas, imgs, iterations, rgb, cx, cy, cn);
            sprintf(outpath, "%s%03d.bmp",out_path, i);
            stbi_write_bmp(outpath, cx, cy, cn, canvas);
        }
    }

    // free images
    stbi_image_free(map);
    for (int i = 0; i < 256; i++) {
        stbi_image_free(imgs[i]);
    }

	return 0;
}
