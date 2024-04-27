#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include <stdio.h>
#include "upng.h"
#include "constants.h"

upng_t* textures[NUM_TEXTURES];

void LoadTextures();
void FreeTextures();

#endif