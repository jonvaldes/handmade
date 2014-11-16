#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "image.h"

void createWindow();
void closeWindow();

Image* getFramebuffer();
void flushFramebuffer();

enum { KEY_DOWN = 1, KEY_UP = 2, MOUSE_DOWN = 3, MOUSE_UP = 4, MOUSE_MOVE = 5 };

typedef struct {
    int type;
    int key;
    int posx;
    int posy;
} KeyEvent;

bool pollEvent(KeyEvent* ev);

// Current timestamp in nanosecs
uint64_t nanoTime();
