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
 * split up main function
 * lazy load in rgb-mode too
 */

/*
 * ASSUMPTIONS:
 * three channel bitmap images (RGB)
 * 256 images in image sequence
 */

int main (int argc, char *argv[]) {
    if (argc < 6) {
        printf("Usage: %s [map] [image sequence folder] [iterations] [rgb/grayscale] [out]\n"
                "Images in the sequence should be labeled: 000.bmp, 001.bmp ... 255.bmp\n"
                "rgb = 1, grayscale = 0\n", argv[0]);
        return 1;
    }

    // rgb or grayscale map
    int rgb = atoi(argv[4]);


    // images
    unsigned char *imgs[256] = {0};

    // load map
    int cx,cy,cn;
    unsigned char *map = stbi_load(argv[1], &cx, &cy, &cn, 0);
    if (map == 0) {
        fprintf(stderr, "Could not open map %s\n", argv[1]);
        return 1;
    }

    // find unique pixel values to only load needed images from image sequence
    unsigned char gray = 0;
    unsigned char *p = map;
    int unique[256] = {0};
    for (int i = 0; i < (cx * cy * cn); i += cn) {
        gray = (*(p+i) + *(p+i+1) + *(p+i+2)) / 3;
        if (unique[gray] == 0) {
            unique[gray] = 1;
        }
    }

    // load images
    int x,y,n;
    char imgpath[13];
    for (int i = 0; i < 256; i++) {
        if (rgb ? 1 : unique[i]) { 
            sprintf(imgpath, "%s/%03d.bmp",argv[2], i);
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

    // map image sequence to canvas
    for (int i = 0; i < atoi(argv[3]); i++) {
        unsigned char gray = 0;
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char *p = map;
        for (int i = 0; i < (cx * cy * cn); i += cn) {
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

    stbi_write_bmp(argv[5], cx, cy, cn, map);
    stbi_image_free(map);
    for (int i = 0; i < 256; i++) {
        stbi_image_free(imgs[i]);
    }

	return 0;
}
