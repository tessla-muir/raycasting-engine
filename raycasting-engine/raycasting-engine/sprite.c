#include "sprite.h"

#define NUM_SPRITES 1

static sprite_t sprites[NUM_SPRITES] = {
	{ .x = 500, .y = 530, .texture = 9 }
};

void RenderSpriteProj() {

}

void RenderMapSprites() {
	for (int i = 0; i < NUM_SPRITES; i++) {
		DrawRect(sprites[i].x * MAP_SCALER, sprites[i].y * MAP_SCALER, 6, 6, 0xFF00FFFF);
	}
}