#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include "image.h"

void createWindow();
void closeWindow();

Image * getFramebuffer();
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




