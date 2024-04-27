#ifndef RAYS_H
#define RAYS_H

#include <stdbool.h>
#include <limits.h>
#include "constants.h"
#include "player.h"
#include "graphics.h"

typedef struct
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

void NormalizeAngle(float* angle);

float DistanceBetweenPoints(float x1, float y1, float x2, float y2);

void CastRay(float rayAngle, int stripId);

void RenderRays();

void CastAllRays();

bool isRayFacingUp(float angle);
bool isRayFacingDown(float angle);
bool isRayFacingLeft(float angle);
bool isRayFacingRight(float angle);

#endif