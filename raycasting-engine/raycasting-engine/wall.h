#ifndef WALL_H
#define WALL_H

#include <math.h>
#include "constants.h"
#include "player.h"
#include "rays.h"
#include "graphics.h"
#include "textures.h"

void RenderWallProj();
void ChangeColorIntensity(color_t* color, float factor);

#endif // !WALL_H
