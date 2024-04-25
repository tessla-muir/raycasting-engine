#include <stdint.h>
#include <SDL.h>
#include "graphics.h"
#include "constants.h"
#include "textures.h"
#include "rays.h"
#include "map.h"
#include "wall.h"

int isProgramRunning = 1;
int lastFrameTicks;

color_t* textures[NUM_TEXTURES];

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