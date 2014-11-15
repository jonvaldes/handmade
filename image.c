#include "image.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

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
    void * addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fileno(fd), 0);
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

/*
vi: ft=cpp
*/
