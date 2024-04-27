#include "wall.h"

void ChangeColorIntensity(color_t* color, float factor) {
	color_t a = (*color & 0xFF000000);
	color_t r = (*color & 0x00FF0000) * factor;
	color_t g = (*color & 0x0000FF00) * factor;
	color_t b = (*color & 0x000000FF) * factor;

	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

void RenderWallProj() {
	for (int x = 0; x < NUM_RAYS; x++) {
		float perDist = rays[x].distance * cos(rays[x].rayAngle - player.rotation);
		float WallHeight = (TILE_SIZE / perDist) * DISTANCE_PROJ_PLANE;


		int wallTopY = (WIN_HEIGHT / 2) - (WallHeight / 2);
		wallTopY = wallTopY < 0 ? 0 : wallTopY;

		int wallBotY = (WIN_HEIGHT / 2) + (WallHeight / 2);
		wallBotY = wallBotY > WIN_HEIGHT ? WIN_HEIGHT : wallBotY;

		// Render color of ceiling
		for (int y = 0; y < wallTopY; y++) {
			DrawPixel(x, y, 0xCCCCCC);
		}

		int textureOffsetX;
		// Calc Offset X
		if (rays[x].wasHitVertical) {
			// perform Offset for vert hit
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		}
		else {
			// perform Offset for horz hit
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		}

		// Texture ID from map
		int texNum = rays[x].wallHitContent - 1;
		int texWidth = upng_get_width(textures[texNum]);
		int texHeight = upng_get_height(textures[texNum]);

		// Render wall top to bottom pixel
		for (int y = wallTopY; y < wallBotY; y++) {
			// Calc Offset Y
			int distanceFromTop = y + (WallHeight / 2) - (WIN_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)texHeight / WallHeight);

			// Set color of wall to the color of texture
			color_t* wallTexBuffer = (color_t*)upng_get_buffer(textures[texNum]);
			color_t texColor = wallTexBuffer[(texWidth * textureOffsetY) + textureOffsetX];

			// Makes pixel color more intense if ray hit was vetical
			if (rays[x].wasHitVertical) {
				ChangeColorIntensity(&texColor, 0.7);
			}

			DrawPixel(x, y, texColor);
		}

		// Render color of floor
		for (int y = wallBotY; y < WIN_HEIGHT; y++) {
			DrawPixel(x, y, 0x444444);
		}
	}
}