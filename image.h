#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct{
    int width;
    int height;
    uint8_t * pixels;

    void * _mmappedData;
    uint32_t _mmapSize;
    FILE * _mmapFd;
} Image;

Image * loadTGAImage(const char * filename);
void deleteImage(Image * img);

typedef enum{ 
    PAINT_OPAQUE,
    PAINT_OVER,
} PaintMode;

void clear(Image * img, uint8_t a, uint8_t r, uint8_t g, uint8_t b);

void paint(PaintMode mode, Image * src, Image * dst, int x, int y);

