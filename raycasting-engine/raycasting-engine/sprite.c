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
			sprites[i].angle = playerSpriteAngle;
			sprites[i].dist = DistanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
		}
		else {
			sprites[i].isVisible = 0;
		}
	}

	// Draw the visibile sprites
	// Similar to wall projection
	for (int i = 0; i < NUM_SPRITES; i++) {
		if (sprites[i].isVisible) {
			// Find the sprite height -- Similar triangles
			float spriteHeight = (TILE_SIZE / sprites[i].dist) * DISTANCE_PROJ_PLANE;
			float spriteWidth = spriteHeight; // Same in this case - square assets

			// Find y values
			float spriteTopY = (WIN_HEIGHT / 2) - (spriteHeight / 2);
			spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY; // Ensure it's above zero

			float spriteBottomY = (WIN_HEIGHT / 2) + (spriteHeight / 2);
			spriteBottomY = (spriteBottomY > WIN_HEIGHT) ? WIN_HEIGHT : spriteBottomY; // Ensure it's below window height

			// Find sprite position relative to proj plane
			float spriteAngle = atan2(sprites[i].y - player.y, sprites[i].x - player.x) - player.rotation;
			float spritePosX = tan(spriteAngle) * DISTANCE_PROJ_PLANE;

			// Find x values
			float spriteLeftX = (WIN_WIDTH / 2) + spritePosX;
			float spriteRightX = spriteLeftX + spriteWidth;

			// Now we can finally draw the sprite
			// Loop through x
			for (int x = spriteLeftX; x < spriteRightX; x++) {
				// Loop through y
				for (int y = spriteTopY; y < spriteBottomY; y++) {
					// Ensure valid values
					if (x > 0 && x < WIN_WIDTH && y > 0 && y < WIN_HEIGHT) {
						DrawPixel(x, y, 0xFFFF0000);
					}
				}
			}
		}
	}
}

void RenderMapSprites() {
	for (int i = 0; i < NUM_SPRITES; i++) {
		DrawRect(
			sprites[i].x * MAP_SCALER,
			sprites[i].y * MAP_SCALER,
			3,
			3,
			(sprites[i].isVisible) ? 0xFF00FFFF : 0xFF444444
		);
	}
}