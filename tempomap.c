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
int tm_load (unsigned char **imgs, char *imgs_path, int *unique, int rgb, int cx, int cy, int cn) {
    int x,y,n;
    char imgpath[256]; // TODO make dynamic
    for (int i = 0; i < 256; i++) {
        if (rgb ? 1 : unique[i]) { 
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
void tm_map (unsigned char *map, unsigned char **imgs, int iterations, int rgb, int x, int y, int n) {
    unsigned char gray = 0;
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    for (int i = 0; i < iterations; i++) {
        unsigned char *p = map;
        for (int i = 0; i < (x * y * n); i += n) {
            if (rgb) {
                r = *(p+i);
                g = *(p+i+1);
                b = *(p+i+2);
                *(p+i)   = *(imgs[r]+i);
                *(p+i+1) = *(imgs[g]+i+1);
                *(p+i+2) = *(imgs[b]+i+2);
            } else {
                gray = (*(p+i) + *(p+i+1) + *(p+i+2)) / 3;
                *(p+i)   = *(imgs[gray]+i);
                *(p+i+1) = *(imgs[gray]+i+1);
                *(p+i+2) = *(imgs[gray]+i+2);
            }
        }
    }
}

int main (int argc, char *argv[]) {
    if (argc < 6) {
        printf("Usage: %s [map] [image sequence folder] [iterations] [rgb/grayscale] [out]\n"
                "Images in the sequence should be labeled: 000.bmp, 001.bmp ... 255.bmp\n"
                "rgb = 1, grayscale = 0\n", argv[0]);
        return 1;
    }

    // options
    char *map_path = argv[1];
    char *imgs_path = argv[2];
    int iterations = atoi(argv[3]);
    int rgb = atoi(argv[4]);
    char *out_path = argv[5];

    // load map
    int cx,cy,cn;
    unsigned char *map = stbi_load(map_path, &cx, &cy, &cn, 0);
    if (map == 0) {
        fprintf(stderr, "Could not open map %s\n", argv[1]);
        return 1;
    }

    // find which images are needed
    int unique[256] = {0};
    tm_unique(map, unique, cx, cy, cn);

    // load images
    unsigned char *imgs[256] = {0};
    if (tm_load(imgs, imgs_path, unique, rgb, cx, cy, cn)) {
        stbi_image_free(map);
        return 1; // passthrough error
    }

    // map image sequence to canvas
    tm_map(map, imgs, iterations, rgb, cx, cy, cn);

    // save canvas
    stbi_write_bmp(out_path, cx, cy, cn, map);

    // free images
    stbi_image_free(map);
    for (int i = 0; i < 256; i++) {
        stbi_image_free(imgs[i]);
    }

	return 0;
}
