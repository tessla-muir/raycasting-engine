#ifndef RAYS_H
#define RAYS_H

#include <limits.h>
#include "constants.h"
#include "player.h"
#include "map.h"

typedef struct
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
} ray_t;

extern ray_t rays[NUM_RAYS];

float NormalizeAngle(float angle);

float DistanceBetweenPoints(float x1, float y1, float x2, float y2);

void CastRay(float rayAngle, int stripId);

void RenderRays();

#endif