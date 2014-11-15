#pragma once
#include <stdint.h>
#include <stdio.h>
#include "os.h"

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

void paint(PaintMode mode, Image * img, Framebuffer * fb, int x, int y);

