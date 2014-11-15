#include "os.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    createWindow(640,480);
    int framecnt = 0;

    Image * img = loadTGAImage("cat.tga");
    if(img == NULL){
        printf("ERROR: Could not open image file\n");
        exit(1);
    }

    while(1){
        KeyEvent ev;
        while(pollEvent(&ev)){
            printf("Keycode: %d\n", ev.key);
            if(ev.key == 53){
                goto endGame;
            }
        }
        unsigned char * fb = getFramebuffer();
        for(int y=0;y<480;y++){
            for(int x=0;x<640;x++){
                int px = (x + framecnt) % img->width;
                int py = (y + framecnt) % img->height;
                // ARGB format!
                uint8_t * pixel = img->pixels + 4*(px + py * img->width);
                uint8_t alpha = *pixel;

                fb[ (x + y * 640) * 4] = *(pixel+3);
                fb[ (x + y * 640) * 4 + 1] = ((int)*(pixel+2)) * alpha / 255;
                fb[ (x + y * 640) * 4 + 2] = ((int)*(pixel+1)) * alpha / 255;
                fb[ (x + y * 640) * 4 + 3] = ((int)*(pixel+0)) * alpha / 255;
            }
        }
        flushFramebuffer();
        framecnt++;
    }

endGame:
    deleteImage(img);
    closeWindow();
}

/*
# vi: ft=cpp
*/
