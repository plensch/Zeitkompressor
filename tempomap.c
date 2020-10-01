#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
 * TODO
 * check if canvas and images are openen (p != 0)
 * check if all images have the same size
 */

int main (int argc, char *argv[]) {
    // images
    unsigned char *imgs[256] = {0};

    // load canvas
    int cx,cy,cn;
    unsigned char *map = stbi_load("in.bmp", &cx, &cy, &cn, 0);
    /* printf("%d %d %d\n", cx, cy, cn); */

    // load images
    int x,y,n;
    char imgpath[13];
    for (unsigned int i = 0; i < 256; i++) {
        sprintf(imgpath, "imgs/%03d.bmp",i);
        imgs[i] = stbi_load(imgpath, &x, &y, &n, 0);
        /* printf("%d %d %d %d %s\n", x, y, n, i, imgpath); */
    }

    // create map
    /* unsigned char *canvas = malloc(gray_img_size); */
    /* size_t canvassize = cx * cy * cn * 3; */

    // magic
    unsigned char gray = 0;
    unsigned char *p = map;
    for (unsigned int i = 0; i < (cx*cy*cn); i += cn) {
        gray = (*(p+i) * *(p+i+1) * *(p+i+2)) / 3;
        *(p+i) = *(imgs[gray]+i);
        *(p+i+1) = *(imgs[gray]+i+1);
        *(p+i+2) = *(imgs[gray]+i+2);
    }

    stbi_write_bmp("out.bmp", cx, cy, cn, map);
    stbi_image_free(map);

    // free images
    for (unsigned int i = 0; i < 256; i++) {
        stbi_image_free(imgs[i]);
    }

	return 0;
}
