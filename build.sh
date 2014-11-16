#!/bin/bash
function fail(){
    echo 
    echo ERROR!
    exit 1
}
clang -O2 -Wall -std=c99 -framework Cocoa -framework Foundation -lobjc -ObjC osx.m main.c image.c profile.c -o handmade|| fail
