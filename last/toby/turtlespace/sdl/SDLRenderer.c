#include <stdio.h>
#include <string.h>
#include "SDLRenderer.h"
#include "SDL.h"

//static SDL_Surface *screen = NULL;

JNIEXPORT void JNICALL Java_last_toby_turtlespace_sdl_SDLRenderer_native_1Constructor
  (JNIEnv *env, jclass cl)
{
    //printf("Constructor...");
    SDL_Init(SDL_INIT_VIDEO);
    setbuf(stdout, NULL);
    SDL_SetVideoMode(640, 480, 32, 0);
//    memset(screen->pixels, '\0', (640 * 480) * 2);
    //printf("done.\n");
} // native_Constructor

JNIEXPORT void JNICALL Java_last_toby_turtlespace_sdl_SDLRenderer_native_1renderLine
  (JNIEnv *env, jclass cl, jdouble x1, jdouble y1, jdouble x2, jdouble y2, jint r, jint g, jint b)
{
    // borrowed from SGE's DoLine code: http://www.etek.chalmers.se/~e8cal1/sge/
    SDL_Surface *screen = SDL_GetVideoSurface();

    Uint32 pixel = ( ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF) );
	Sint16 dx, dy, sdx, sdy, x, y, px, py;

    //printf("drawing a line...(%lf, %lf)-(%lf, %lf), (%d, %d, %d)...",
    //        x1, y1, x2, y2, r, g, b);

 	dx = (int) x2 - (int) x1;
 	dy = (int) y2 - (int) y1;

 	sdx = (dx < 0) ? -1 : 1;
 	sdy = (dy < 0) ? -1 : 1;

 	dx = sdx * dx + 1;
 	dy = sdy * dy + 1;

 	x = y = 0;

 	px = x1;
 	py = y1;

 	if (dx >= dy){
  	for (x = 0; x < dx; x++){
        *( ((Uint32 *) screen->pixels) + ((py * screen->w) + px) ) = pixel;

	  	y += dy;
	  	if (y >= dx){
	    	y -= dx;
	    	py += sdy;
	   	}
	  	px += sdx;
		}
	}
	else{
  	for (y = 0; y < dy; y++){

        *( ((Uint32 *) screen->pixels) + ((py * screen->w) + px) ) = pixel;

	  	x += dx;
	  	if (x >= dy){
	    	x -= dy;
	    	px += sdx;
	   	}
	  	py += sdy;
		}
	}

    SDL_UpdateRect(screen, (int) ((x1 < x2) ? x1 : x2), (int) ((y1 < y2) ? y1 : y2), dx, dy);

    //printf("done.\n");
} // native_renderLine

JNIEXPORT void JNICALL Java_last_toby_turtlespace_sdl_SDLRenderer_native_1cleanup
  (JNIEnv *env, jclass cl)
{
    //printf("cleanup...");
    //memset(screen->pixels, '\0', (640 * 480) * 2);
    //printf("done.\n");
} // native_cleanup
