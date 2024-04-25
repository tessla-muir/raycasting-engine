#include "wall.h"

void RenderWallProj(void) {
	for (int i = 0; i < NUM_RAYS; i++) {
		float perDist = rays[i].distance * cos(rays[i].rayAngle - player.rotation);
		float projWallHeight = (TILE_SIZE / perDist) * DISTANCE_PROJ_PLANE;

		int wallStripHeight = (int)projWallHeight;

		int wallTopPix = (WIN_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPix = wallTopPix < 0 ? 0 : wallTopPix;

		int wallBotPix = (WIN_HEIGHT / 2) + (wallStripHeight / 2);
		wallBotPix = wallBotPix > WIN_HEIGHT ? WIN_HEIGHT : wallBotPix;

		// Render color of ceiling
		for (int y = 0; y < wallTopPix; y++) {
			DrawPixel(i, y, 0xCCCCCC);
		}

		int textureOffsetX;
		// Calc Offset X
		if (rays[i].wasHitVertical) {
			// perform Offset for vert hit
			textureOffsetX = (int)rays[i].wallHitY % TILE_SIZE;
		}
		else {
			// perform Offset for horz hit
			textureOffsetX = (int)rays[i].wallHitX % TILE_SIZE;
		}

		// Texture ID from map
		int texNum = rays[i].wallHitContent - 1;

		// Render wall top to bottom pixel
		for (int y = wallTopPix; y < wallBotPix; y++) {
			// Calc Offset Y
			int distanceFromTop = y + (wallStripHeight / 2) - (WIN_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)TEX_HEIGHT / wallStripHeight);

			// set color of wall to the color of texture
			uint32_t texColor = wallTextures[texNum].texture_buffer[(TEX_WIDTH * textureOffsetY) + textureOffsetX];
			DrawPixel(i, y, texColor);
		}

		// Render color of floor
		for (int y = wallBotPix; y < WIN_HEIGHT; y++) {
			DrawPixel(i, y, 0x444444);
		}
	}
}