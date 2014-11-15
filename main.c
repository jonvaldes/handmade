#include "os.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
        Image * fb = getFramebuffer();
        clear(fb, 0, 30, 140, 40);

        for(int i=0; i<20; i++){
            paint(PAINT_OVER, img, fb, 
                    (int)(fb->width / 2 + (i*20) * sin(i * 1.221 + framecnt * 0.01)),
                    (int)(fb->height/ 2 + (i*20) * cos(i * 1.221 + framecnt * 0.01 )));
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
