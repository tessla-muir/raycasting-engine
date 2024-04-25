#include "graphics.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

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
		WIN_WIDTH, // Window resolution: width & height
		WIN_HEIGHT,
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

	// Allocate total amount of bytes to hold in buffer 
	colorBuffer = (color_t*)malloc(sizeof(color_t) * (color_t)WIN_WIDTH * (color_t)WIN_HEIGHT);
	colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH, WIN_HEIGHT);

	return 1;
}

void DestroyWindow() {
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);

	// Destroy in opposite order of creation
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void ClearColorBuffer(color_t color) {
	for (int x = 0; x < WIN_WIDTH; x++) {
		for (int y = 0; y < WIN_HEIGHT; y++) {
			colorBuffer[(WIN_WIDTH * y) + x] = color;
		}
	}
}

void RenderColorBuffer() {
	SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)(color_t)WIN_WIDTH * sizeof(color_t));
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void DrawPixel(int x, int y, color_t color) {
	colorBuffer[(WIN_WIDTH * y) + x] = color;
}

void DrawRect(int x, int y, int width, int height, color_t color) {
	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			DrawPixel(i, j, color);
		}
	}
}

// Follows Digital Differential Analyzer for line drawing
// Thanks to GeeksForGeeks for DDA Line Explanation
void DrawLine(int x0, int y0, int x1, int y1, color_t color) {
	// Delta values
	int dX = x1 - x0;
	int dY = y1 - y0;

	int longestSide = (abs(dX) >= abs(dY)) ? abs(dX) : abs(dY);

	// Find increments
	float xIncr = dX / (float)longestSide;
	float yIncr = dY / (float)longestSide;

	float x = x0;
	float y = y0;

	for (int i = 0; i < longestSide; i++) {
		DrawPixel(round(x), round(y), color);
		x += xIncr;
		y += yIncr;
	}
}