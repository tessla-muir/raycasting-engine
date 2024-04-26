#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <math.h>

#define TILE_SIZE 64
#define MAP_ROWS 13
#define MAP_COLS 20
#define MAP_SCALER 0.25

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define PI 3.14159265
#define FOVA (60 * PI / 180)
#define DISTANCE_PROJ_PLANE ((WIN_WIDTH / 2) / tan(FOVA / 2))
#define WALL_STRIP_WIDTH 1
#define NUM_RAYS WIN_WIDTH / WALL_STRIP_WIDTH

#define FPS 60
#define FRAME_LENGTH (1000 / FPS)

#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define NUM_TEXTURES 8

typedef uint32_t color_t;

#endif