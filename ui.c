#include "ui.h"

void drawText(Image* dest, const Image* font, const Color* c, const char* text, int destX, int destY) {
    int fontWidth = font->width;
    int fontHeight = font->height / 256;

    int lineDestX = destX;
    while(*text) {
        unsigned char t = *(unsigned char*)text;
        if(t == '\n') {
            lineDestX = destX;
            destY += fontHeight;
            text++;
            continue;
        }

        int origy = t * fontHeight;
        for(int y = 0; (y < fontHeight) && (destY + y < dest->height); y++) {
            if(destY + y < 0) {
                continue;
            }
            uint8_t* destRow = dest->pixels + 4 * (destY + y) * dest->width;
            for(int x = 0; x < fontWidth; x++) {
                if((lineDestX + x >= dest->width) || (lineDestX + x < 0)) {
                    continue;
                }
                uint8_t* origP = &font->pixels[4 * (x + (y + origy) * fontWidth)];
                uint8_t* destP = destRow + 4 * (lineDestX + x);
                int alpha = (int)origP[0];
                if(alpha == 0) {
                    continue;
                }
                const int oneMinusAlpha = 255 - (alpha * (int)c->a / 255);
                for(int i = 0; i < 4; i++) {
                    destP[i] = 0xFF & (origP[i] * (int)c->c[i] / 255 * (int)c->a / 255 +
                                       oneMinusAlpha * destP[i] / 255);
                }
            }
        }
        lineDestX += fontWidth;
        text++;
    }
}

void textSize(Image* font, const char* text, int* width, int* height) {
    int curWidth = 0;
    int curHeight = 1;
    int maxWidth = 0;
    for(; *text; text++) {
        if(*text == '\n') {
            curHeight++;
            maxWidth = curWidth > maxWidth ? curWidth : maxWidth;
            curWidth = 0;
        } else {
            curWidth++;
        }
    }
    maxWidth = curWidth > maxWidth ? curWidth : maxWidth;

    int fontWidth = font->width;
    int fontHeight = font->height / 256;
    *width = maxWidth * fontWidth;
    *height = curHeight * fontHeight;
}

void fillRect(Image* dest, const Color* c, int minx, int miny, int maxx, int maxy) {
    for(int y = miny; y < maxy; y++) {
        if(y < 0 || y >= dest->height) {
            continue;
        }
        for(int x = minx; x < maxx; x++) {
            if(x < 0 || x >= dest->width) {
                continue;
            }
            uint8_t* p = &dest->pixels[4 * (x + y * dest->width)];
            for(int i = 0; i < 4; i++) {
                p[i] = c->c[i];
            }
        }
    }
}

void fillBevel(Image* dest, const Color* c, int bevel, int minx, int miny, int maxx, int maxy) {
    const Color* currentColor;
    Color light = *c;
    Color dark = *c;
    for(int i = 1; i < 4; i++) {
        light.c[i] = ((int)light.c[i]) * 3 / 2;
        dark.c[i] = ((int)dark.c[i]) * 2 / 3;
    }
    for(int y = miny; y < maxy; y++) {
        // TODO - thess loops should compute start and end instead of continuing
        if(y < 0 || y >= dest->height) {
            continue;
        }
        for(int x = minx; x < maxx; x++) {
            if(x < 0 || x >= dest->width) {
                continue;
            }
            if(((y < miny + bevel) && ((y - miny) < (maxx - x))) ||
               ((x < minx + bevel) && ((x - minx) < (maxy - y - 1)))) {
                currentColor = &light;
            } else if(y > maxy - bevel - 1 || x > maxx - bevel - 1) {
                currentColor = &dark;
            } else {
                currentColor = c;
            }
            uint8_t* p = &dest->pixels[4 * (x + y * dest->width)];
            for(int i = 0; i < 4; i++) {
                p[i] = currentColor->c[i];
            }
        }
    }
}

void drawButton(Image* dest, Image* font, const char* text, int x, int y) {
    const int padding = 7;
    const int bevel = 2;
    int textWidth, textHeight;
    textSize(font, text, &textWidth, &textHeight);

    int hw = textWidth / 2;
    int hh = textHeight / 2;
    const Color back = color(255, 60, 60, 70);
    const Color front = color(255, 250, 250, 250);
    fillBevel(dest, &back, bevel, x - hw - padding, y - hh - padding, x + hw + padding, y + hh + padding);
    drawText(dest, font, &front, text, x - hw, y - hh);
}

/*
vi: ft=cpp
*/
