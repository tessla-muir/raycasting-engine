#include "map.h"

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

int GetMapAt(int i, int j) {
	return map[i][j];
}

int HasMapWallAt(float x, float y) {
	// Check window border
	if (x < 0 || x > WIN_WIDTH || y < 0 || y > WIN_HEIGHT) return 0;

	// Check made walls as well
	int mapTileX = floor(x / TILE_SIZE);
	int mapTileY = floor(y / TILE_SIZE);
	return map[mapTileY][mapTileX] != 0;
}

void RenderMap() {
	// Go through tiles by row and column
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			// Determine tile parameters
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 0xFFFFFFF : 0;

			DrawRect(tileX * MAP_SCALER, tileY * MAP_SCALER, TILE_SIZE * MAP_SCALER, TILE_SIZE * MAP_SCALER, tileColor);
		}
	}
}