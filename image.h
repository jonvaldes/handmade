#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int width;
    int height;
    uint8_t* pixels;

    void* _mmappedData;
    uint32_t _mmapSize;
    FILE* _mmapFd;
    bool mustFreePixels;
} Image;

Image* createImage(int width, int height);
Image* loadTGAImage(const char* filename);
void deleteImage(Image* img);

typedef enum {
    PAINT_OPAQUE,
    PAINT_OVER,
} PaintMode;

typedef struct {
    union {
        struct {
            uint8_t a;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };
        uint8_t c[4];
    };
} Color;

Color color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

void clear(Image* img, const Color* c);

void paint(PaintMode mode, Image* src, Image* dst, int x, int y);
void drawText(Image* dest, const Image* font, const Color* c, const char* text, int destX, int destY);
void textSize(Image* font, const char* text, int* width, int* height);
void drawButton(Image* dest, Image* font, const char* text, int x, int y);
