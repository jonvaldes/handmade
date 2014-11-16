#pragma once
#include "image.h"
void fillRect(Image* dest, const Color* c, int minx, int miny, int maxx, int maxy);
void fillBevel(Image* dest, const Color* c, int bevel, int minx, int miny, int maxx, int maxy);

void drawText(Image* dest, const Image* font, const Color* c, const char* text, int destX, int destY);
void textSize(Image* font, const char* text, int* width, int* height);
void drawButton(Image* dest, Image* font, const char* text, int x, int y);
