#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include "upng.h"
#include "constants.h"

typedef struct {
    upng_t* upngTexture;
    int width;
    int height;
    color_t* texture_buffer;
} texture_t;

texture_t wallTextures[NUM_TEXTURES];

void LoadWallTextures();
void FreeWallTextures();

#endif
