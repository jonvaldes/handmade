#include "os.h"

void clearFramebuffer(Framebuffer* fb, uint8_t a, uint8_t r, uint8_t g, uint8_t b){
    int32_t value = ((int32_t) b)<<24 | ((int32_t)g)<<16 | ((int32_t)r)<<8|a;
    int32_t * v = (int32_t*) fb->pixels;

    for(int i=0;i<fb->width * fb->height; i++){
        v[i] = value;
    }
}

