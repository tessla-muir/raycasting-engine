#include "rays.h"

ray_t rays[NUM_RAYS];

void NormalizeAngle(float* angle) {
	*angle = remainderf(*angle, (2 * PI));
	if (*angle < 0) {
		*angle = (2 * PI) + *angle;
	}
}

float DistanceBetweenPoints(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool isRayFacingDown(float angle) {
	return angle > 0 && angle < PI;
}

bool isRayFacingUp(float angle) {
	return !isRayFacingDown(angle);
}

bool isRayFacingRight(float angle) {
	return angle < 0.5 * PI || angle > 1.5 * PI;
}

bool isRayFacingLeft(float angle) {
	return !isRayFacingRight(angle);
}

void CastRay(float rayAngle, int stripId) {
	NormalizeAngle(&rayAngle);

	float xintercept, yintercept;
	float xstep, ystep;

	// Horizontal Ray-Grid Intercention Code
	int foundHorizontalWallHit = 0;
	float horizontalWallHitX = 0;
	float horizontalWallHitY = 0;
	int horizontalWallContent = 0;

	// Find the y-cord of the closest horz grid intercention
	yintercept = floorf(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown(rayAngle) ? TILE_SIZE : 0;

	// Find the x-cord of the closest horz grid intercention
	xintercept = player.x + (yintercept - player.y) / tanf(rayAngle);

	// Calc the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp(rayAngle) ? -1 : 1;

	xstep = TILE_SIZE / tanf(rayAngle);
	xstep *= (isRayFacingLeft(rayAngle) && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight(rayAngle) && xstep < 0) ? -1 : 1;

	float nextHorizontalTouchX = xintercept;
	float nextHorizontalTouchY = yintercept;

	while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= WIN_WIDTH && nextHorizontalTouchY >= 0 && nextHorizontalTouchY <= WIN_HEIGHT) {
		float xToCheck = nextHorizontalTouchX;
		float yToCheck = nextHorizontalTouchY + (isRayFacingUp(rayAngle) ? -1 : 0);

		if (HasMapWallAt(xToCheck, yToCheck)) {
			//Found Wall Hit
			horizontalWallHitX = nextHorizontalTouchX;
			horizontalWallHitY = nextHorizontalTouchY;
			horizontalWallContent = GetMapAt((int)floorf(yToCheck / TILE_SIZE), (int)floorf(xToCheck / TILE_SIZE));
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
	xintercept += isRayFacingRight(rayAngle) ? TILE_SIZE : 0;

	// Find the y-cord of the closest horz grid intercention
	yintercept = player.y + (xintercept - player.x) * tanf(rayAngle);

	// Calc the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft(rayAngle) ? -1 : 1;

	ystep = TILE_SIZE * tanf(rayAngle);
	ystep *= (isRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;

	float nextVerticalTouchX = xintercept;
	float nextVerticalTouchY = yintercept;

	while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= WIN_WIDTH && nextVerticalTouchY >= 0 && nextVerticalTouchY <= WIN_HEIGHT) {
		float xToCheck = nextVerticalTouchX + (isRayFacingLeft(rayAngle) ? -1 : 0);
		float yToCheck = nextVerticalTouchY;

		if (HasMapWallAt(xToCheck, yToCheck)) {
			//Found Wall Hit
			VerticalWallHitX = nextVerticalTouchX;
			VerticalWallHitY = nextVerticalTouchY;
			VerticalWallContent = GetMapAt((int)floorf(yToCheck / TILE_SIZE), (int)floorf(xToCheck / TILE_SIZE));
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
		rays[stripId].rayAngle = rayAngle;
	}
	else {
		rays[stripId].distance = horizontalHitDist;
		rays[stripId].wallHitX = horizontalWallHitX;
		rays[stripId].wallHitY = horizontalWallHitY;
		rays[stripId].wallHitContent = horizontalWallContent;
		rays[stripId].wasHitVertical = 0;
		rays[stripId].rayAngle = rayAngle;
	}
}

void CastAllRays() {
	for (int column = 0; column < NUM_RAYS; column++) {
		float rayAngle = player.rotation + atan((column - NUM_RAYS / 2) / DISTANCE_PROJ_PLANE);
		CastRay(rayAngle, column);

		rayAngle += FOVA / NUM_RAYS;
	}
}

void RenderRays() {
	for (int i = 0; i < NUM_RAYS; i++) {
		DrawLine(MAP_SCALER * player.x, MAP_SCALER * player.y, MAP_SCALER * rays[i].wallHitX, MAP_SCALER * rays[i].wallHitY, 0xFF0000FF);
	}
}