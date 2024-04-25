#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"
#include "graphics.h"

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int turnDir;
	int walkDir;
	float rotation;
	float walkSpeed;
	float turnSpeed;
} player_t;

extern player_t player;

void PlayerMovement(float deltaTime);
void RenderPlayer();

#endif