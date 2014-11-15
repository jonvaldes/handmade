#include "os.h"
#include <stdio.h>

int main(){
    createWindow(640,480);
    int framecnt = 0;

    while(1){
        KeyEvent ev;
        if(pollEvent(&ev)){
            printf("Ev: %d\n", ev.key);
            if(ev.key == 53){
                break;
            }
        }
        unsigned char * fb = getFramebuffer();
        for(int y=0;y<480;y++){
            for(int x=0;x<640;x++){
                fb[ (x + y * 640) * 4] = (framecnt+ x+y)%255;
                fb[ (x + y * 640) * 4 + 1] = (framecnt + x+y)%255;
                fb[ (x + y * 640) * 4 + 2] = (framecnt + x+y)%255;
                fb[ (x + y * 640) * 4 + 3] = 255;
            }
        }
        flushFramebuffer();
        framecnt++;
    }
    closeWindow();
}

/*
# vi: ft=cpp
*/
