#!/bin/sh

gcc -s -shared -o libSDLRenderer.so -O2 -I. -I/usr/local/jdk/include/ -I/usr/local/jdk/include/linux/ `sdl-config --cflags` `sdl-config --libs` SDLRenderer.c