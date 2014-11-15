#!/bin/bash
function fail(){
    echo 
    echo ERROR!
    exit 1
}
echo "osx.o"
clang -ObjC -c osx.m -o osx.o || fail
echo "main.c"
clang -std=c99 -framework Cocoa -framework Foundation -lobjc main.c osx.o || fail
