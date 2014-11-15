#import <AppKit/AppKit.h>
#include <stdio.h>
#include "os.h"

static NSWindow * g_window;
static NSApplication * g_app;
static CGContextRef g_context;
static NSGraphicsContext * nsgfx_context; 
static unsigned char * g_pixels;

void createWindow(int width, int height){
    g_app = [NSApplication sharedApplication];
    NSApplicationLoad();

    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    NSRect MainWindowRect = NSMakeRect(0, 0, width, height);
    g_window = [[NSWindow alloc] initWithContentRect: MainWindowRect
        styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask 
        backing: NSBackingStoreBuffered defer: NO];

    [g_window setTitle:@"Handmade Game"];
    [g_window center];

    NSView * window_view = [ [ NSView alloc ] initWithFrame:MainWindowRect];
    [ window_view setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable ];
    [ [ g_window contentView ] addSubview:window_view ];
    [ window_view release ];
    [ g_window makeKeyAndOrderFront:nil ];
    [ g_window orderFrontRegardless];
    [ g_window setViewsNeedDisplay:NO ];

    CGColorSpaceRef cgColorspace = CGColorSpaceCreateDeviceRGB();

    g_pixels = malloc(width * height * 4);
    nsgfx_context = [NSGraphicsContext graphicsContextWithWindow:g_window];
    [NSGraphicsContext setCurrentContext:nsgfx_context];
    
    g_context = CGBitmapContextCreate (g_pixels, width, height, 8, 4 * width, cgColorspace, (CGBitmapInfo)kCGImageAlphaNoneSkipFirst);
    CGColorSpaceRelease (cgColorspace);

    [NSApp activateIgnoringOtherApps:YES];
}

bool pollEvent(KeyEvent * ev){
    NSDate * distantPast = [ NSDate distantPast ];
    NSEvent * event = [ NSApp nextEventMatchingMask:NSAnyEventMask untilDate:distantPast inMode: NSDefaultRunLoopMode dequeue:YES ];
    if(event != nil){
        unsigned int type = [ event type ];
        switch(type){
            case NSKeyUp:
                ev->type = KEY_UP;
                ev->key = [event keyCode];
                return true;
            case NSKeyDown:
                ev->type = KEY_DOWN;
                ev->key = [event keyCode];
                return true;
            // We're not handling mouse events right now
        }
    }
    return false;
}

void flushFramebuffer(){
    CGContextFlush (g_context);
    CGContextRef cgc = (CGContextRef) [nsgfx_context graphicsPort];
    CGImageRef image = CGBitmapContextCreateImage (g_context);
    CGRect rectangle = CGRectMake (0,0,640,480);
    CGContextDrawImage (cgc, rectangle, image);
    CGImageRelease(image);
    CGContextFlush (cgc);
}

unsigned char * getFramebuffer(){
    return g_pixels;
}

void closeWindow(){
    [g_window close];
}

/*
# vi: ft=c
*/
