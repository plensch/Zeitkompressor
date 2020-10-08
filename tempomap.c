#include <stdio.h>
#include <pthread.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
 * TODO
 * add option to hold images in memory between iterations
 */

/*
 * ASSUMPTIONS:
 * three channel bitmap images (RGB)
 * 256 images in image sequence
 */

struct tmap_args {
    unsigned char *map;
    unsigned char **imgs;
    int iterations;
    int x;
    int y;
    int n;
    int start;
    int end;
};

void tmap(void *args) {
    int iterations  = ((struct tmap_args*)args)->iterations;
    int x = ((struct tmap_args*)args)->x;
    int y = ((struct tmap_args*)args)->y;
    int n = ((struct tmap_args*)args)->n;
    int start = ((struct tmap_args*)args)->start;
    int end = ((struct tmap_args*)args)->end;
    unsigned char **imgs = ((struct tmap_args*)args)->imgs;
    for (int i = 0; i < iterations; i++) {
        unsigned char gray = 0;
        unsigned char *p = ((struct tmap_args*)args)->map;
        for (unsigned int i = 0; i < end*n; i += n) {
            gray = (*(p+i) * *(p+i+1) * *(p+i+2)) / 3;
            *(p+i)   = *(imgs[gray]+i);
            *(p+i+1) = *(imgs[gray]+i+1);
            *(p+i+2) = *(imgs[gray]+i+2);
        }
    }
}

int main (int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s [map] [image sequence folder] [iterations] [out]\n"
                        "Images in the sequence should be labeled: 000.bmp, 001.bmp ... 255.bmp\n", argv[0]);
        return 1;
    }


    // images
    unsigned char *imgs[256] = {0};

    // load map
    int cx,cy,cn;
    unsigned char *map = stbi_load(argv[1], &cx, &cy, &cn, 0);
    if (map == 0) {
        fprintf(stderr, "Could not open map %s\n", argv[1]);
        return 1;
    }

    // load images
    int x,y,n;
    char imgpath[13];
    for (unsigned int i = 0; i < 256; i++) {
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

    // map image sequence to canvas
    struct tmap_args targs;
    targs.map = map;
    targs.imgs = imgs;
    targs.iterations = atoi(argv[3]);
    targs.x = cx;
    targs.y = cy;
    targs.n = cn;

    int threadcount = 2;
    pthread_t tid[threadcount];
    /// create all threads
    for (int i = 0; i < threadcount; i++) {
        targs.start = i * ((cx*cy)/threadcount);
        targs.end = (i+1) * ((cx*cy)/threadcount);
        pthread_create(&tid[i], NULL, tmap, &targs);
    }

    /// wait all threads by joining them
    for (int i = 0; i < threadcount; i++) {
        pthread_join(tid[i], NULL);
    }

    // free images
    stbi_write_bmp("out.bmp", cx, cy, cn, map);
    stbi_image_free(map);
    for (unsigned int i = 0; i < 256; i++) {
        stbi_image_free(imgs[i]);
    }

	return 0;
}
