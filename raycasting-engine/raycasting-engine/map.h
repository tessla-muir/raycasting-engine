#ifndef MAP_H
#define MAP_H

#include "graphics.h"
#include "constants.h"

int GetMapAt(int i, int j);
int HasMapWallAt(float x, float y);
void RenderMap();

#endif 