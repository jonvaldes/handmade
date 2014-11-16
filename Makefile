all: 
	clang -O2 -Wall -std=c99 -framework Cocoa -framework Foundation -lobjc \
    -ObjC osx.m main.c image.c profile.c ui.c -o handmade
