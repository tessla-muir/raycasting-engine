#include "player.h"

player_t player = {
	.x = 350,
	.y = 450,
	.width = 12,
	.height = 12,
	.walkDir = 0,
	.turnDir = 0,
	.rotation = 3 * PI / 2,
	.walkSpeed = 80,
	.turnSpeed = 45 * (PI / 180)
};

void PlayerMovement(float time) {
	// Rotation
	player.rotation += player.turnDir * (player.turnSpeed * time);
	NormalizeAngle(&player.rotation);

	// Movement
	float step = player.walkDir * (player.walkSpeed * time);
	float newX = player.x + cos(player.rotation) * step;
	float newY = player.y + sin(player.rotation) * step;

	// Wall Collisions
	if (!HasMapWallAt(newX, newY)) {
		player.x = newX;
		player.y = newY;
	}
}

void RenderPlayer() {
	DrawRect(player.x * MAP_SCALER, player.y * MAP_SCALER, player.width * MAP_SCALER, player.height * MAP_SCALER, 0xFFFFFFF);
}