#include <stdio.h>
#include <SDL.h>
#include <limits.h>

// Constants
#define PI 3.14159265
#define FOVA (60 * PI / 180)

#define WALL_STRIP_WIDTH 1
#define NUM_RAYS WIN_WIDTH / WALL_STRIP_WIDTH
#define TILE_SIZE 64
#define MAP_ROWS 13
#define MAP_COLS 20
float mapScaleFactor = 1.0;

const int map[MAP_ROWS][MAP_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// For FPS
#define FPS 30
#define FRAME_LENGTH (1000 / FPS)
int lastFrameTicks;

#define WIN_WIDTH (MAP_COLS * TILE_SIZE)
#define WIN_HEIGHT (MAP_ROWS * TILE_SIZE)
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isProgramRunning = 1;

struct Player {
	float x;
	float y;
	float width;
	float height;
	int turnDir;
	int walkDir;
	float rotation;
	float walkSpeed;
	float turnSpeed;
} player;

struct Ray
{
	float rayAngle;
	float wallHitx;
	float wallHity;
	float distance;
	int wasHitVertical;
	int isRayFacingUp;
	int isRayFacingDown;
	int isRayFacingLeft;
	int isRayFacingRight;
	int wallHitContent;
} rays[NUM_RAYS];

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

	return 1;
}

void DestroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Setup() {
	// Set player stats
	player.x = WIN_WIDTH / 2;
	player.y = WIN_HEIGHT / 2;
	player.width = 5;
	player.height = 5;
	player.walkDir = 0;
	player.turnDir = 0;
	player.rotation = PI / 2;
	player.walkSpeed = 80;
	player.turnSpeed = 45 * (PI / 180);
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
				player.turnDir = 1;
			}
			if(event.key.keysym.sym == SDLK_RIGHT) {
				player.turnDir = -1;
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

void RenderMap() {
	// Go through tiles by row and column
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			// Determine tile parameters
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 255 : 0;

			// Render tile
			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTile = {
				tileX * mapScaleFactor, tileY* mapScaleFactor, TILE_SIZE * mapScaleFactor, TILE_SIZE* mapScaleFactor
			};
			SDL_RenderFillRect(renderer, &mapTile);
		}
	}
}

float NormalizeAngle(float angle) {
	angle = remainderf(angle, (2 * PI));
	if (angle < 0) {
		angle = (2 * PI) + angle;
	}
	return angle;
}

float DistanceBetweenPoints(x1, y1, x2, y2) {
	return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void CastRay(float rayAngle, int stripId) {
	rayAngle = NormalizeAngle(rayAngle);

	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	// Horizontal Ray-Grid Intercention Code
	int foundHorizontalWallHit = SDL_FALSE;
	float horizontalWallHitX = 0;
	float horizontalWallHitY = 0;
	int horizontalWallContent = 0;

	// Find the y-cord of the closest horz grid intercention
	yintercept = floorf(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-cord of the closest horz grid intercention
	xintercept = player.x + (yintercept - player.y) / tanf(rayAngle);

	// Calc the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;
	
	xstep = TILE_SIZE / tanf(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorizontalTouchX = xintercept;
	float nextHorizontalTouchY = yintercept;

	while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= WIN_WIDTH && nextHorizontalTouchY >= 0 && nextHorizontalTouchY <= WIN_HEIGHT) {
		float xToCheck = nextHorizontalTouchX;
		float yToCheck = nextHorizontalTouchY + (isRayFacingUp ? -1 : 0);

		if (hasMapWallAt(xToCheck, yToCheck)) {
			//Found Wall Hit
			horizontalWallHitX = nextHorizontalTouchX;
			horizontalWallHitY = nextHorizontalTouchY;
			horizontalWallContent = map[(int) floorf(yToCheck / TILE_SIZE)][(int) floorf(xToCheck / TILE_SIZE)];
			foundHorizontalWallHit = SDL_TRUE;
			break;
		}
		else {
			nextHorizontalTouchX += xstep;
			nextHorizontalTouchY += ystep;
		}
	}

	// Vertical Ray-Grid Intercention Code
	int foundVerticalWallHit = SDL_FALSE;
	float VerticalWallHitX = 0;
	float VerticalWallHitY = 0;
	int VerticalWallContent = 0;

	// Find the y-cord of the closest horz grid intercention
	xintercept = floorf(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the x-cord of the closest horz grid intercention
	yintercept = player.y + (xintercept - player.x) / tanf(rayAngle);

	// Calc the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE / tanf(rayAngle);
	ystep *= (isRayFacingUp && xstep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && xstep < 0) ? -1 : 1;

	float nextVerticalTouchX = xintercept;
	float nextVerticalTouchY = yintercept;

	while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= WIN_WIDTH && nextVerticalTouchY >= 0 && nextVerticalTouchY <= WIN_HEIGHT) {
		float xToCheck = nextVerticalTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVerticalTouchY;

		if (hasMapWallAt(xToCheck, yToCheck)) {
			//Found Wall Hit
			VerticalWallHitX = nextVerticalTouchX;
			VerticalWallHitY = nextVerticalTouchY;
			VerticalWallContent = map[(int)floorf(yToCheck / TILE_SIZE)][(int)floorf(xToCheck / TILE_SIZE)];
			foundVerticalWallHit = SDL_TRUE;
			break;
		}
		else {
			nextVerticalTouchX += xstep;
			nextVerticalTouchY += ystep;
		}
	}

	// Calc both horz and vert hit distances, chooose the smallest one
	float horizontalHitDist = foundHorizontalWallHit ? DistanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY) : INT_MAX;
}

void CastAllRays() {
	// start first ray subtracting half of our FOV
	float rayAngle = player.rotation - (FOVA / 2);
	
	for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
		CastRay(rayAngle, stripId);

		rayAngle += FOVA / NUM_RAYS;
	}
}

void RenderPlayer() {
	// Player Rectangle
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect playerRect = {
		player.x * mapScaleFactor,
		player.y * mapScaleFactor,
		player.width * mapScaleFactor,
		player.height * mapScaleFactor
	};
	SDL_RenderFillRect(renderer, &playerRect);

	// Line of player direction
	SDL_RenderDrawLine(
		renderer,
		player.x * mapScaleFactor,
		player.y * mapScaleFactor,
		player.x + cos(player.rotation) * 35 * mapScaleFactor,
		player.y + sin(player.rotation) * 35 * mapScaleFactor
	);
}

int hasMapWallAt(float x, float y) {
	// Check window border
	if (x < 0 || x > WIN_WIDTH || y < 0 || y > WIN_HEIGHT) return 0;

	// Check made walls as well
	int mapTileX = floor(x / TILE_SIZE);
	int mapTileY = floor(y / TILE_SIZE);
	return map[mapTileY][mapTileX] != 0;
}

void PlayerMovement(float time) {
	// Rotation
	player.rotation += player.turnDir * (player.turnSpeed * time);

	// Movement
	float step = player.walkDir * (player.walkSpeed * time);
	float newX = player.x + cos(player.rotation) * step;
	float newY = player.y + sin(player.rotation) * step;

	// Wall Collisions
	if (!hasMapWallAt(newX, newY)) {
		player.x = newX;
		player.y = newY;
	}
}

void Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render all objects on the current cycle
	RenderMap();
	//RenderRays();
	RenderPlayer();

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
	PlayerMovement(deltaTime);
	CastAllRays();
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