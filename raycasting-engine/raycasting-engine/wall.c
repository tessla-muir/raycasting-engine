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
		float projWallHeight = (TILE_SIZE / perDist) * DISTANCE_PROJ_PLANE;

		int wallStripHeight = (int)projWallHeight;

		int wallTopPix = (WIN_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPix = wallTopPix < 0 ? 0 : wallTopPix;

		int wallBotPix = (WIN_HEIGHT / 2) + (wallStripHeight / 2);
		wallBotPix = wallBotPix > WIN_HEIGHT ? WIN_HEIGHT : wallBotPix;

		// Render color of ceiling
		for (int y = 0; y < wallTopPix; y++) {
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

		// Render wall top to bottom pixel
		for (int y = wallTopPix; y < wallBotPix; y++) {
			// Calc Offset Y
			int distanceFromTop = y + (wallStripHeight / 2) - (WIN_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)TEX_HEIGHT / wallStripHeight);

			// set color of wall to the color of texture
			color_t texColor = wallTextures[texNum].texture_buffer[(TEX_WIDTH * textureOffsetY) + textureOffsetX];

			// Makes pixel color more intense if ray hit was vetical
			if (rays[i].wasHitVertical) {
				ChangeColorIntensity(&texColor, 0.7);
			}

			DrawPixel(x, y, texColor);
		}

		// Render color of floor
		for (int y = wallBotPix; y < WIN_HEIGHT; y++) {
			DrawPixel(x, y, 0x444444);
		}
	}
}