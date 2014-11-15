#pragma once 
#include <stdbool.h>

void createWindow();
void closeWindow();
unsigned char * getFramebuffer();
void flushFramebuffer();


enum {
    KEY_DOWN = 1,
    KEY_UP = 2
};

typedef struct {
    int type;
    int key;
} KeyEvent;

bool pollEvent(KeyEvent * ev);




