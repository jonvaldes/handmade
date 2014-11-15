#pragma once 
#include <stdint.h>
#include <stdbool.h>

void createWindow();
void closeWindow();

typedef struct {
    int width;
    int height;
    uint8_t * pixels;
} Framebuffer;

Framebuffer * getFramebuffer();
void flushFramebuffer();
void clearFramebuffer(Framebuffer* fb, uint8_t a, uint8_t r, uint8_t g, uint8_t b);

enum {
    KEY_DOWN = 1,
    KEY_UP = 2
};

typedef struct {
    int type;
    int key;
} KeyEvent;

bool pollEvent(KeyEvent * ev);




