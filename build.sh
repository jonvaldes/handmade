#!/bin/bash
function fail(){
    echo 
    echo ERROR!
    exit 1
}
echo "osx.o"
clang -ObjC -c osx.m -o osx.o || fail
echo "main.c"
clang -g -Wall -std=c99 -framework Cocoa -framework Foundation -lobjc main.c image.c hand_math.c os.c osx.o || fail
