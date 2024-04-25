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
void DrawRect(int x, int y, int width, int height, uint32_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

#endif