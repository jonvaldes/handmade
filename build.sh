#!/bin/bash
function fail(){
    echo 
    echo ERROR!
    exit 1
}
clang -g -Wall -std=c99 -framework Cocoa -framework Foundation -lobjc -ObjC osx.m main.c image.c hand_math.c -o handmade|| fail
