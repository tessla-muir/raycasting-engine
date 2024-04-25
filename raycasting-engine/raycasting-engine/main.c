#include <stdint.h>
#include <SDL.h>
#include <limits.h>
#include "graphics.h"
#include "constants.h"
#include "textures.h"
#include "rays.h"
#include "map.h"

int isProgramRunning = 1;
int lastFrameTicks;

uint32_t* textures[NUM_TEXTURES];

void Setup() {
	// Asks uPNG to decode PNG files and loads them to wallTex array
	LoadWallTextures();
}

void InputProcessing() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		// User can press X on window to quit
	case SDL_QUIT: {
		isProgramRunning = 0;
		break;
	}
	case SDL_KEYDOWN: {
		// User can press ESC key to quit
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			isProgramRunning = 0;
			break;
		}
		// Start player movement/turning on key down
		if (event.key.keysym.sym == SDLK_UP) {
			player.walkDir = 1;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
			player.walkDir = -1;
		}
		if (event.key.keysym.sym == SDLK_LEFT) {
			player.turnDir = -1;
		}
		if (event.key.keysym.sym == SDLK_RIGHT) {
			player.turnDir = 1;
		}
		break;
	}
	case SDL_KEYUP: {
		// Stop player movement/turning on key up
		if (event.key.keysym.sym == SDLK_UP) {
			player.walkDir = 0;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
			player.walkDir = 0;
		}
		if (event.key.keysym.sym == SDLK_LEFT) {
			player.turnDir = 0;
		}
		if (event.key.keysym.sym == SDLK_RIGHT) {
			player.turnDir = 0;
		}
		break;
	}
	}
}

void RenderWallProj() {
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
			int textureOffsetY = distanceFromTop *  ((float)TEX_HEIGHT / wallStripHeight);

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

void Render() {
	// Clear color buffer
	ClearColorBuffer(0xFF000000);

	RenderWallProj();

	// Render all objects on the current cycle, Displays minimap
	RenderMap();
	RenderRays();
	RenderPlayer();

	RenderColorBuffer();
}

void Update() {
	// Wait to update the target frame -- even framerate independent of processor power
	int waitTime = FRAME_LENGTH - (SDL_GetTicks() - lastFrameTicks);
	if (waitTime > 0 && waitTime <= FRAME_LENGTH) {
		SDL_Delay(waitTime);
	}

	float deltaTime = (SDL_GetTicks() - lastFrameTicks) / 1000.0f;
	lastFrameTicks = SDL_GetTicks();

	// Update objects based on deltaTime!
	PlayerMovement(deltaTime);
	CastAllRays();
}

void ReleaseResources() {
	FreeWallTextures();
	DestroyWindow();
}

int main(int argc, char* args[]) {
	isProgramRunning = InitalizeWindow();

	Setup();

	while (isProgramRunning)
	{
		InputProcessing();
		Update();
		Render();
	}

	ReleaseResources();

	return 0;
}