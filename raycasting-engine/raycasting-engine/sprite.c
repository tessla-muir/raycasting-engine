#include "sprite.h"

#define NUM_SPRITES 1

static sprite_t sprites[NUM_SPRITES] = {
	{ .x = 400, .y = 530, .texture = 9 }
};

void RenderSpriteProj() {
	// Find sprites currently in the player's FOV
	for (int i = 0; i < NUM_SPRITES; i++) {
		// Angle between the center of the player's FOV and the sprite
		float playerSpriteAngle = player.rotation - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

		// Normalize the angle to be between 0 and 2 * PI
		if (playerSpriteAngle < 0) {
			playerSpriteAngle += 2 * PI;
		}

		// Check if the sprite angle is within the player's FOV
		if (playerSpriteAngle < FOVA / 2 || playerSpriteAngle > 2 * PI - FOVA / 2) {
			sprites[i].isVisible = 1;
		}
		else {
			sprites[i].isVisible = 0;
		}
	}


	//// Draw the visibile sprites
	//for (int i = 0; i < NUM_SPRITES; i++) {
	//	if (sprites[i].isVisible) {

	//	}
	//}
}

void RenderMapSprites() {
	for (int i = 0; i < NUM_SPRITES; i++) {
		DrawRect(
			sprites[i].x * MAP_SCALER,
			sprites[i].y * MAP_SCALER,
			6,
			6,
			(sprites[i].isVisible) ? 0xFF00FFFF : 0xFF444444
		);
	}
}