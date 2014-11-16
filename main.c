#include "os.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "profile.h"

int main() {
    createWindow(640, 480);
    int framecnt = 0;

    Image* img = loadTGAImage("cat.tga");
    if(img == NULL) {
        printf("ERROR: Could not open image file\n");
        exit(1);
    }
    Image* font = loadTGAImage("font.tga");
    if(font == NULL) {
        printf("ERROR: Could not load font file\n");
        exit(1);
    }

    int mousePos[2] = {0, 0};

    while(1) {
        KeyEvent ev;
        profileBlockStart("eventHandling");
        while(pollEvent(&ev)) {
            switch(ev.type) {
            case KEY_DOWN:
                printf("Keycode: %d\n", ev.key);
                if(ev.key == 53) {
                    goto endGame;
                }
                break;
            case MOUSE_MOVE:
                mousePos[0] = ev.posx;
                mousePos[1] = ev.posy;
                break;
            }
        }
        profileBlockEnd("eventHandling");
        Image* fb = getFramebuffer();
        profileBlockStart("clearFramebuffer");
        Color backColor = color(0, 30, 140, 40);
        clear(fb, &backColor);
        profileBlockEnd("clearFramebuffer");

        profileBlockStart("paintCats");
        for(int i = 0; i < 300; i++) {
            paint(PAINT_OVER, img, fb, (int)(fb->width / 2 + (i * 0.7) * sin(i * 1.221 + framecnt * 0.004)),
                  (int)(fb->height / 2 + (i * 0.7) * cos(i * 1.221 + framecnt * 0.004)));
        }
        profileBlockEnd("paintCats");

        profileBlockStart("paintText");
        Color textColor = color(255, 200, 250, 255);
        textColor.a = (framecnt * 32) % 256;
        drawText(fb, font, &textColor, "Hello world!", mousePos[0], mousePos[1]);
        drawButton(fb, font, "Click here!", 120, 200);

        profileBlockEnd("paintText");

        profileBlockStart("flush");
        flushFramebuffer();
        profileBlockEnd("flush");
        framecnt++;
    }

endGame:
    saveProfile("_profile.json");
    deleteImage(img);
    closeWindow();
}

/*
# vi: ft=cpp
*/
