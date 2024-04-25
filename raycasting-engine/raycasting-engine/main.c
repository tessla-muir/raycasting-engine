#include <stdint.h>
#include <SDL.h>
#include <limits.h>
#include "graphics.h"
#include "constants.h"
#include "textures.h"

int isProgramRunning = 1;
int lastFrameTicks;
float mapScaleFactor = 0.25;

const int map[MAP_ROWS][MAP_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 7, 0, 6, 0, 5, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

uint32_t* textures[NUM_TEXTURES];

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
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int isRayFacingUp;
	int isRayFacingDown;
	int isRayFacingLeft;
	int isRayFacingRight;
	int wallHitContent;
} rays[NUM_RAYS];

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

	// Asks uPNG to decode PNG files and loads them to wallTex array
	loadWallTextures();
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

void RenderPlayer() {
	//// Player Rectangle
	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	//SDL_Rect playerRect = {
	//	player.x * mapScaleFactor,
	//	player.y * mapScaleFactor,
	//	player.width * mapScaleFactor,
	//	player.height * mapScaleFactor
	//};
	//SDL_RenderFillRect(renderer, &playerRect);
}

float DistanceBetweenPoints(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float NormalizeAngle(float angle) {
	angle = remainderf(angle, (2 * PI));
	if (angle < 0) {
		angle = (2 * PI) + angle;
	}
	return angle;
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
	int foundHorizontalWallHit = 0;
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
			horizontalWallContent = map[(int)floorf(yToCheck / TILE_SIZE)][(int)floorf(xToCheck / TILE_SIZE)];
			foundHorizontalWallHit = 1;
			break;
		}
		else {
			nextHorizontalTouchX += xstep;
			nextHorizontalTouchY += ystep;
		}
	}

	// Vertical Ray-Grid Intercention Code
	int foundVerticalWallHit = 0;
	float VerticalWallHitX = 0;
	float VerticalWallHitY = 0;
	int VerticalWallContent = 0;

	// Find the x-cord of the closest horz grid intercention
	xintercept = floorf(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the y-cord of the closest horz grid intercention
	yintercept = player.y + (xintercept - player.x) * tanf(rayAngle);

	// Calc the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tanf(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

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
			foundVerticalWallHit = 1;
			break;
		}
		else {
			nextVerticalTouchX += xstep;
			nextVerticalTouchY += ystep;
		}
	}

	// Calc both horz and vert hit distances, chooose the smallest one
	float horizontalHitDist = foundHorizontalWallHit ? DistanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY) : INT_MAX;
	float verticalHitDist = foundVerticalWallHit ? DistanceBetweenPoints(player.x, player.y, VerticalWallHitX, VerticalWallHitY) : INT_MAX;

	if (verticalHitDist < horizontalHitDist) {
		rays[stripId].distance = verticalHitDist;
		rays[stripId].wallHitX = VerticalWallHitX;
		rays[stripId].wallHitY = VerticalWallHitY;
		rays[stripId].wallHitContent = VerticalWallContent;
		rays[stripId].wasHitVertical = 1;
	}
	else {
		rays[stripId].distance = horizontalHitDist;
		rays[stripId].wallHitX = horizontalWallHitX;
		rays[stripId].wallHitY = horizontalWallHitY;
		rays[stripId].wallHitContent = horizontalWallContent;
		rays[stripId].wasHitVertical = 0;
	}
	rays[stripId].rayAngle = rayAngle;
	rays[stripId].isRayFacingDown = isRayFacingDown;
	rays[stripId].isRayFacingUp = isRayFacingUp;
	rays[stripId].isRayFacingLeft = isRayFacingLeft;
	rays[stripId].isRayFacingRight = isRayFacingRight;
}

void CastAllRays() {
	for (int column = 0; column < NUM_RAYS; column++) {
		float rayAngle = player.rotation + atan((column - NUM_RAYS/2) / DISTANCE_PROJ_PLANE);
		CastRay(rayAngle, column);

		rayAngle += FOVA / NUM_RAYS;
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

			DrawRect(tileX * mapScaleFactor, tileY * mapScaleFactor, TILE_SIZE * mapScaleFactor, TILE_SIZE * mapScaleFactor, tileColor);
		}
	}
}

void RenderRays() {
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//for (int i = 0; i < NUM_RAYS; i++) {
	//	SDL_RenderDrawLine(renderer, mapScaleFactor * player.x, mapScaleFactor * player.y, mapScaleFactor * rays[i].wallHitX, mapScaleFactor * rays[i].wallHitY);
	//}
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
	freeWallTextures();
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