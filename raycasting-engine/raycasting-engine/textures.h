#ifndef TEXTURES_H
#define TEXTURES_H

#define NUM_TEXTURES 8
#include <stdint.h>
#include "upng.h"

typedef struct {
    upng_t* upngTexture;
    int width;
    int height;
    uint32_t* texture_buffer;
} texture_t;

texture_t wallTextures[NUM_TEXTURES];

void loadWallTextures();
void freeWallTextures();

#endif
