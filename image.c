#include "image.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

Image* createImage(int width, int height) {
    Image* result = (Image*)calloc(1, sizeof(Image));
    result->width = width;
    result->height = height;
    result->mustFreePixels = true;
    result->pixels = (uint8_t*)calloc(width * height * 4, 1);
    return result;
}

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
    result->mustFreePixels = false;

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
    if(img->mustFreePixels) {
        free(img->pixels);
    }
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

static uint32_t colorToInt(const Color* c) {
    return ((uint32_t)c->b) << 24 | ((uint32_t)c->g) << 16 | ((uint32_t)c->r) << 8 | c->a;
}

void clear(Image* img, const Color* c) {
    uint32_t value = colorToInt(c);
    uint32_t* v = (uint32_t*)img->pixels;

    for(int i = 0; i < img->width * img->height; i++) {
        v[i] = value;
    }
}

Color color(uint8_t a, uint8_t r, uint8_t g, uint8_t b) { return (Color){.a = a, .r = r, .g = g, .b = b}; }

/*
vi: ft=cpp
*/
