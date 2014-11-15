#include "image.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "hand_math.h"

Image * loadTGAImage(const char * filename) {
    // Open file
    FILE * fd = fopen(filename, "rb");
    if( fd == NULL ) {
        return NULL;
    }

    // Get file size
    if( fseek(fd, 0, SEEK_END) ){
        goto errCloseFd;
    }
    size_t size = ftell(fd);

    // Mmap it
    void * addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(fd), 0);
    if( addr == (void*) -1){
        goto errCloseFd;
    }

    Image * result = (Image *) malloc(sizeof(Image));
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
    } header_t ;

    header_t * header = (header_t*) addr;
    result->width = header->Width;
    result->height = header->Height;
    result->pixels = ((uint8_t* ) addr) + sizeof(header_t) + header->CMapLength * header->CMapDepth;
    
    // Premultiply alpha
    uint8_t *p = result->pixels;
    for(int i=0;i<result->width * result->height; i++){
        int alpha = *p;
        for(int j=0;j<4;j++){
            p[j] = (uint8_t)(p[j] * alpha / 255);
        }
        p+=4;
    }

    return result;

errCloseFd:
    fclose(fd);
    return NULL;
}

void deleteImage(Image * img){
    munmap(img->_mmappedData, img->_mmapSize);
    fclose(img->_mmapFd);
    memset(img, 0, sizeof(Image));
}

void paint(PaintMode mode, Image * img, Framebuffer * fb, int xcenter, int ycenter){
    const int startx = xcenter - img->width/2;
    const int starty = ycenter - img->height/2;

    for( int iy = 0 ; (iy < img->height) && (starty + iy < fb->height); iy++){
        const int y = starty + iy;
        if(y < 0){
            continue;
        }
        
        for( int ix = 0 ; (ix < img->width) && (startx + ix < fb->width) ; ix++){
            const int x = startx + ix;
            if(x < 0){
                continue;
            }
            uint8_t * fbPixel = fb->pixels + 4*(x + fb->width * y);
            uint8_t * imgPixel = img->pixels + 4*(ix + img->width*iy);

            const int alpha = (int)*imgPixel;
            if(mode == PAINT_OPAQUE){
                *fbPixel = alpha;
                *(fbPixel+1) = *(imgPixel + 1);
                *(fbPixel+2) = *(imgPixel + 2);
                *(fbPixel+3) = *(imgPixel + 3);
            }else{ // PAINT_OVER
                const int oneMinusAlpha= 255 - alpha;
                for( int i=0; i<4; i++){
                    fbPixel[i] = iclamp(imgPixel[i] + oneMinusAlpha * fbPixel[i] / 255, 0, 255);
                }
            }
        }
    }
}

void clearFramebuffer(Framebuffer* fb, uint8_t a, uint8_t r, uint8_t g, uint8_t b){
    int32_t value = ((int32_t) b)<<24 | ((int32_t)g)<<16 | ((int32_t)r)<<8|a;
    int32_t * v = (int32_t*) fb->pixels;

    for(int i=0;i<fb->width * fb->height; i++){
        v[i] = value;
    }
}

/*
vi: ft=cpp
*/
