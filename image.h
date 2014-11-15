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

