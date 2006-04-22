#!/bin/sh

gcc -Wall -O0 -g -o toby -lm toby.c tobygrfx.c tobydata.c unixbits.c `sdl-config --cflags --libs`

