#include <stdio.h>
#include <SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isProgramRunning = 1;

// For FPS
#define FPS 30
#define FRAME_LENGTH (1000 / FPS)
int lastFrameTicks;

// Player vars
int playerX, playerY;

int InitalizeWindow() {
	// Intializes everything for SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL: Init Intialization Error.\n");
		return 0;
	}

	window = SDL_CreateWindow(
		"740 Final Project", // Window title
		SDL_WINDOWPOS_CENTERED, // Center the window on width & height
		SDL_WINDOWPOS_CENTERED, 
		800, // Window resolution: width & height
		600,
		SDL_WINDOW_SHOWN // Show window
		);

	// Ensure window intialization
	if (!window) {
		fprintf(stderr, "SDL: Window Intialization Error.\n");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	// Ensure renderer intialization
	if (!renderer) {
		fprintf(stderr, "SDL: Renderer Intialization Error\n");
		return 0;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return 1;
}

void DestroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Setup() {
	playerX = 0;
	playerY = 0;
}

void InputProcessing() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: {
			isProgramRunning = 0;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isProgramRunning = 0;
				break;
			}
		}
	}
}

void Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render all objects on the current cycle
	SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
	SDL_Rect playerRect = { playerX, playerY, 20, 20 };
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderPresent(renderer);
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
	playerX += 50 * deltaTime;
	playerY += 50 * deltaTime;
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

	DestroyWindow();

	return 0;
}