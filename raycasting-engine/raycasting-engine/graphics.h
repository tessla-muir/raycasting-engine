#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <SDL.h>
#include "constants.h"

int InitalizeWindow();
void DestroyWindow();
void ClearColorBuffer(uint32_t color);
void RenderColorBuffer();
void DrawPixel(int x, int y, uint32_t color);

#endif