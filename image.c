#include "image.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

Image* loadTGAImage(const char* filename) {
    // Open file
    FILE* fd = fopen(filename, "rb");
    if(fd == NULL) {
        return NULL;
    }

    // Get file size
    if(fseek(fd, 0, SEEK_END)) {
        goto errCloseFd;
    }
    size_t size = ftell(fd);

    // Mmap it
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(fd), 0);
    if(addr == (void*)-1) {
        goto errCloseFd;
    }

    Image* result = (Image*)malloc(sizeof(Image));
    result->_mmapFd = fd;
    result->_mmapSize = size;
    result->_mmappedData = addr;

    typedef struct {
        uint8_t IDLength;
        uint8_t ColorMapType;
        uint8_t ImageType;
        uint16_t CMapLength;
        uint8_t CMapDepth;
        uint16_t XOffset;
        uint16_t YOffset;
        uint16_t Width;
        uint16_t Height;
        uint8_t PixelDepth;
        uint8_t ImageDescriptor;
    } header_t;

    header_t* header = (header_t*)addr;
    result->width = header->Width;
    result->height = header->Height;
    result->pixels = ((uint8_t*)addr) + sizeof(header_t) + header->CMapLength * header->CMapDepth;

    // Premultiply alpha
    for(int i = 0; i < result->width * result->height; i++) {
        uint8_t* p = result->pixels + i * 4;
        int alpha = p[3];
        for(int j = 0; j < 3; j++) {
            p[j] = (uint8_t)(p[j] * alpha / 255);
        }
        // BGRA -> ARGB
        uint8_t temp;
        temp = p[0];
        p[0] = p[3];
        p[3] = temp;
        temp = p[1];
        p[1] = p[2];
        p[2] = temp;
    }

    return result;

errCloseFd:
    fclose(fd);
    return NULL;
}

void deleteImage(Image* img) {
    munmap(img->_mmappedData, img->_mmapSize);
    fclose(img->_mmapFd);
    memset(img, 0, sizeof(Image));
}

void paint(PaintMode mode, Image* src, Image* dst, int xcenter, int ycenter) {
    const int startx = xcenter - src->width / 2;
    const int starty = ycenter - src->height / 2;

    for(int iy = 0; (iy < src->height) && (starty + iy < dst->height); iy++) {
        const int y = starty + iy;
        if(y < 0) {
            continue;
        }
        for(int ix = 0; (ix < src->width) && (startx + ix < dst->width); ix++) {
            const int x = startx + ix;
            if(x < 0) {
                continue;
            }
            uint8_t* dstPixel = dst->pixels + 4 * (x + dst->width * y);
            uint8_t* srcPixel = src->pixels + 4 * (ix + src->width * iy);

            if(mode == PAINT_OPAQUE || srcPixel[0] == 0xFF) {
                *((uint32_t*)dstPixel) = *((uint32_t*)srcPixel);
            } else { // PAINT_OVER
                const int alpha = (int)srcPixel[0];
                if(alpha == 0) {
                    continue;
                }
                const int oneMinusAlpha = 255 - alpha;
                for(int i = 0; i < 4; i++) {
                    dstPixel[i] = 0xFF & (srcPixel[i] + oneMinusAlpha * dstPixel[i] / 255);
                }
            }
        }
    }
}

void clear(Image* img, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t value = ((uint32_t)b) << 24 | ((uint32_t)g) << 16 | ((uint32_t)r) << 8 | a;
    uint32_t* v = (uint32_t*)img->pixels;

    for(int i = 0; i < img->width * img->height; i++) {
        v[i] = value;
    }
}

Color col(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    Color res;
    res.a = a;
    res.r = r;
    res.g = g;
    res.b = b;
    return res;
}

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

/*
vi: ft=cpp
*/
