#include "sprite.h"

#define NUM_SPRITES 10

static sprite_t sprites[NUM_SPRITES] = {
	{.x = 200, .y = 710, .texture = 9}, // Barrel
	{.x = 250, .y = 730, .texture = 9}, // Barrel
	{.x = 450, .y = 580, .texture = 11}, // Table
	{.x = 450, .y = 580, .texture = 10}, // Light
	{.x = 300, .y = 120, .texture = 13}, // Armor
	{.x = 400, .y = 120, .texture = 13}, // Armor
	{.x = 700, .y = 680, .texture = 14}, // Dog
	{.x = 880, .y = 365, .texture = 12}, // Guard
	{.x = 980, .y = 580, .texture = 11}, // Table
	{.x = 980, .y = 580, .texture = 10}, // Light
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

		// Check if the sprite angle is within the player's FOV + small margin 
		const float Smargin = 0.15;
		if (playerSpriteAngle < (FOVA / 2) + Smargin|| playerSpriteAngle > 2 * PI - (FOVA / 2) - Smargin) {
			sprites[i].isVisible = 1;
			sprites[i].angle = playerSpriteAngle;
			sprites[i].dist = DistanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);

		}
		else {
			sprites[i].isVisible = 0;
		}
	}
	
	// Sorts aprites by distance 
	for (int i = 0; i < NUM_SPRITES - 1; i++) {
		for (int j = i + 1; j < NUM_SPRITES; j++) {
			if (sprites[i].dist < sprites[j].dist) {
				sprite_t temp = sprites[i];
				sprites[i] = sprites[j];
				sprites[j] = temp;
			}
		}
	}

	// Draw the visibile sprites
	// Similar to wall projection
	for (int i = 0; i < NUM_SPRITES; i++) {
		if (sprites[i].isVisible) {
			// Calc perp dist of sprite to stop fish bowl effect
			float perDist = sprites[i].dist * cos(sprites[i].angle);

			// Find the sprite height -- Similar triangles
			float spriteHeight = (TILE_SIZE / perDist) * DISTANCE_PROJ_PLANE;
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
			float spriteLeftX = (WIN_WIDTH / 2) + spritePosX - (spriteWidth / 2);
			float spriteRightX = spriteLeftX + spriteWidth;

			// Texture dimensions
			int texWidth = upng_get_width(textures[sprites[i].texture]);
			int texHeight = upng_get_height(textures[sprites[i].texture]);

			// Now we can finally draw the sprite
			// Loop through x
			for (int x = spriteLeftX; x < spriteRightX; x++) {
				float texelWidth = (texWidth / spriteWidth); // Need to consider that one texture pixel could take up multiple on the screen
				int texOffsetX = (x - spriteLeftX) * texelWidth;

				// Loop through y
				for (int y = spriteTopY; y < spriteBottomY; y++) {
					// Ensure valid values
					if (x > 0 && x < WIN_WIDTH && y > 0 && y < WIN_HEIGHT) {
						int texelDist = y + (spriteHeight / 2) - (WIN_HEIGHT / 2); // Need to consider that one texture pixel could take up multiple on the screen
						int texOffsetY = texelDist * (texHeight / spriteHeight);

						color_t* spriteTexBuffer = (color_t*) upng_get_buffer(textures[sprites[i].texture]);
						color_t texColor = spriteTexBuffer[(texWidth * texOffsetY) + texOffsetX];
						
						//if (texColor == 0xFFFF00FF) continue; // Skip the bright pink background
						if (sprites->dist < rays[x].distance && texColor != 0xFFFF00FF) {
							DrawPixel(x, y, texColor);
						}
						
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