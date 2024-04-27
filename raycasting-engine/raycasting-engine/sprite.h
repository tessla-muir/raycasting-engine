#ifndef SPRITE_H
#define SPRITE_H

#include "constants.h"
#include "graphics.h"
#include "textures.h"s
#include "player.h"

typedef struct {
	float x;
	float y;
	float dist;
	float angle;
	int texture;
} sprite_t;

void RenderSpriteProj(void);
void RenderMapSprites(void);

#endif