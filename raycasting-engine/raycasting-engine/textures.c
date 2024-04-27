#include "textures.h"


static const char* textureFileNames[NUM_TEXTURES] = {
    "./images/redbrick.png",
    "./images/purplestone.png",
    "./images/mossystone.png",
    "./images/graystone.png",
    "./images/colorstone.png",
    "./images/bluestone.png",
    "./images/wood.png",
    "./images/eagle.png",
    "./images/barrel.png",      // [9]
    "./images/light.png",
    "./images/table.png",
    "./images/guard.png",
    "./images/armor.png"
};

void LoadTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng = upng_new_from_file(textureFileNames[i]);
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                textures[i] = upng;
            }
            else {
                printf("Textures.c: Error decoding tecture %s \n", textureFileNames[i]);
            }
        }
        else {
            printf("Textures.c: Error loading texture %s \n", textureFileNames[i]);
        }
    }
}

void FreeTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (textures[i] != NULL) {
            upng_free(textures[i]);
        }
        else {
            printf("Textures.c: Error freeing texture %s \n", textureFileNames[i]);
        }
    }
}
