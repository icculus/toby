/*
 * Toby -- A programming language for learning.
 * Copyright (C) 2007  Ryan C. Gordon.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "SDL.h"
#include "toby_app.h"

static SDL_Surface *GScreen = NULL;
static SDL_Surface *GBacking = NULL;
static int GRequestingQuit = 0;
static Uint32 GStopWatch = 0;
static int GDelayAndQuit = -1;

#define TOBY_PROFILE 1


void TOBY_putToScreen(void)
{
    const int xoff = (GScreen->w - GBacking->w) / 2;
    const int yoff = (GScreen->h - GBacking->h) / 2;
    SDL_Rect blitr = { xoff, yoff, GBacking->w, GBacking->h };

    if (SDL_MUSTLOCK(GBacking))
        SDL_UnlockSurface(GBacking);

    SDL_BlitSurface(GBacking, NULL, GScreen, &blitr);

    if (SDL_MUSTLOCK(GBacking))
    {
        while (SDL_LockSurface(GBacking) < 0)
            SDL_Delay(10);
    } /* if */

    TOBY_renderAllTurtles(GScreen);
    SDL_Flip(GScreen);
} /* putToScreen */


void TOBY_startRun(void)
{
    GRequestingQuit = 0;
    GStopWatch = SDL_GetTicks();
} // TOBY_startRun


void TOBY_stopRun()
{
    #if TOBY_PROFILE
    printf("time execute: %ld\n", (unsigned long) (SDL_GetTicks()-GStopWatch));
    #endif
} /* TOBY_stopRun */


void TOBY_pumpEvents(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            GRequestingQuit = 1;

        else if (e.type == SDL_VIDEOEXPOSE)
            TOBY_putToScreen();  /* in case we need to force a repaint... */

        else if (e.type == SDL_KEYDOWN)
        {
            const SDL_keysym *k = &e.key.keysym;
            if (k->sym == SDLK_ESCAPE)
                GRequestingQuit = 1;
            #if PLATFORM_MACOSX
            else if ((k->sym == SDLK_q) && (k->mod & KMOD_META))
                GRequestingQuit = 1;
            #endif
        } /* else if */
    } /* while */

    if (GRequestingQuit)
        TOBY_haltProgram();
} /* TOBY_pumpEvents */


void TOBY_messageBox(const char *msg)
{
    fprintf(stderr, "!!! FIXME: message box: '%s'\n", msg);
} /* TOBY_messageBox */


#if 0
#define _D(x) printf x
#else
#define _D(x)
#endif

static inline void scaleXY(lua_Number *x, lua_Number *y)
{
    /* !!! FIXME: Fixed point? */
    *x = GBacking->w * (*x / N(1000));
    *y = GBacking->h * (*y / N(1000));
} /* scaleXY */


void TOBY_drawLine(lua_Number x1, lua_Number y1, lua_Number x2, lua_Number y2,
                   int r, int g, int b)
{
    /*
     * This is a standard Bresenham line-drawing algorithm, but the specific
     *  implementation was borrowed, optimized, and mangled from
     *  SGE's DoLine code:
     *   http://www.etek.chalmers.se/~e8cal1/sge/
     */

    int dx, dy, sdx, sdy, x, y, px, py;
    const int w = GBacking->w;
    const Uint32 pval = (b << 24) | (g << 16) | (r << 8) | 0xFF;

    /* !!! FIXME: this is always 32-bpp at the moment. */
    Uint32 *p = (Uint32 *) GBacking->pixels;

    scaleXY(&x1, &y1);
    scaleXY(&x2, &y2);

    _D(("LFB: rendering line...(%d, %d)-(%d, %d), (%d, %d, %d)...\n",
        (int) x1, (int) y1, (int) x2, (int) y2, (int) r, (int) g, (int) b));

    dx = x2 - x1;
    dy = y2 - y1;

    sdx = (dx < 0) ? -1 : 1;
    sdy = (dy < 0) ? -1 : 1;

    dx = sdx * dx + 1;
    dy = sdy * dy + 1;

    x = y = 0;

    px = (int) x1;
    py = (int) y1;

    if ((dx == 1) && (dy == 1))
    {
        _D(("LFB: Single pixel line fastpath.\n"));
        *(((Uint32 *) p) + ((py * w) + px)) = pval;
    } /* if */

    else if (dx == 1)
    {
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        const int incr = w * sdy;
        _D(("LFB: Vertical line fastpath.\n"));
        for (y = 0; y < dy; y++)
        {
            *linep = pval;
            linep += incr;
        } /* for */
    } /* else if */

    else if (dy == 1)
    {
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        const int incr = sdx;

        _D(("LFB: Horizontal line fastpath.\n"));

        for (x = 0; x < dx; x++)
        {
            *linep = pval;
            linep += incr;
        } /* for */
    } /* else if */

    else if (dx >= dy)
    {
        int lastX = px;
        int thisY = py;

        _D(("LFB: Line with major axis of X.\n"));

        for (x = 0; x < dx; x++)
        {
            *( ((Uint32 *) p) + ((py * w) + px) ) = pval;
            y += dy;
            if (y >= dx)   // run length completed.
            {
                y -= dx;
                py += sdy;

                lastX = px;
                thisY = py;
            } /* if */
            px += sdx;
        } /* for */
    } /* else if */

    else
    {
        int lastY = py;
        int thisX = px;

        _D(("LFB: Line with major axis of Y.\n"));

        for (y = 0; y < dy; y++)
        {
            *( ((Uint32 *) p) + ((py * w) + px) ) = pval;
            x += dx;
            if (x >= dy)  // run length completed.
            {
                x -= dy;
                px += sdx;

                lastY = py;
                thisX = px;
            } /* if */
            py += sdy;
        } /* for */
    } /* else */
} /* TOBY_drawLine */


int TOBY_drawString(lua_Number x, lua_Number y, const char *utf8str,
                    lua_Number angle, int r, int g, int b)
{
    return 0;  /* !!! FIXME: write me. */
} /* TOBY_drawString */


void TOBY_drawTurtle(const Turtle *turtle, void *data)
{
    //SDL_Surface *surf = ((data == NULL)) ? GBacking : (SDL_Surface *) data);
} /* TOBY_drawTurtle */


void TOBY_cleanup(int r, int g, int b)
{
    SDL_FillRect(GBacking, NULL, SDL_MapRGBA(GBacking->format, r, g, b, 0xFF));
    SDL_FillRect(GScreen, NULL, SDL_MapRGB(GScreen->format, r, g, b));
} /* TOBY_cleanup */


long TOBY_getTicks(void)
{
    return (long) SDL_GetTicks();
} // TOBY_getTicks


void TOBY_yieldCPU(int ms)
{
    SDL_Delay(ms);
} // TOBY_yieldCPU


static char *loadProgram(const char *fname)
{
    char *retval = NULL;
    FILE *io = fopen(fname, "rb");
    if (io == NULL)
        fprintf(stderr, "Failed to open '%s': %s\n", fname, strerror(errno));
    else
    {
        long len = 0;
        if ( (fseek(io, 0, SEEK_END) == -1) ||
             ((len = ftell(io)) == -1) ||
             (fseek(io, 0, SEEK_SET) == -1) )
        {
            fprintf(stderr, "i/o error on '%s': %s\n", fname, strerror(errno));
        } /* if */

        else
        {
            retval = (char *) malloc(len + 1);
            if (retval == NULL)
                fprintf(stderr, "Out of memory.\n");
            else
            {
                if (fread(retval, len, 1, io) == 1)
                    retval[len] = '\0';
                else
                {
                    fprintf(stderr, "Failed to read '%s': %s\n", fname,
                            strerror(errno));
                    free(retval);
                    retval = NULL;
                } /* else */
            } /* else */
        } /* else */
        fclose(io);
    } /* else */

    return retval;
} /* loadProgram */


static int doProgram(const char *program, int w, int h, Uint32 flags)
{
    const int size = (h < w) ? h : w;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n",
                SDL_GetError());
        return 4;
    } /* if */

    SDL_WM_SetCaption("Toby", "Toby");

    if ((GScreen = SDL_SetVideoMode(w, h, 0, flags)) == NULL)
    {
        fprintf(stderr, "SDL_SetVideoMode() failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 5;
    } /* if */

    /* Blank to black to start, just in case. */
    SDL_FillRect(GScreen, NULL, SDL_MapRGB(GScreen->format, 0, 0, 0));
    SDL_Flip(GScreen);

    GBacking = SDL_CreateRGBSurface(0, size, size, 32,
                                   0x0000FF00,  /* red */
                                   0x00FF0000,  /* green */
                                   0xFF000000,  /* blue */
                                   0x000000FF); /* alpha */
    if (GBacking == NULL)
    {
        fprintf(stderr, "SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 6;
    } /* if */

    SDL_SetAlpha(GBacking, 0, 0);
    SDL_FillRect(GBacking, NULL, SDL_MapRGBA(GBacking->format, 0, 0, 0, 0xFF));

    if (SDL_MUSTLOCK(GBacking))
    {
        while (SDL_LockSurface(GBacking) < 0)
            SDL_Delay(10);
    } /* if */

    TOBY_runProgram(program, 0);

    if (GDelayAndQuit >= 0)
        TOBY_delay(GDelayAndQuit);
    else
    {
        while (!GRequestingQuit)
            TOBY_delay(100);
    } /* else */

    SDL_FreeSurface(GBacking);
    GScreen = GBacking = NULL;

    SDL_Quit();
    return 0;
} /* doProgram */


int main(int argc, char **argv)
{
    /* !!! FIXME: allow user to change dimensions. */
    const int w = 800;
    const int h = 600;
    char *program = NULL;
    Uint32 sdlflags = 0;
    int retval = 0;
    int i = 0;

    /* Parse command lines. */
    for (i = 1; i < argc; i++)
    {
        const char *arg = argv[i];
        if (*arg == '-')
        {
            while (*(++arg) == '-') { /* no-op. */ }
            if (strcmp(arg, "fullscreen") == 0)
                sdlflags |= SDL_FULLSCREEN;
            else if (strcmp(arg, "windowed") == 0)
                sdlflags &= ~SDL_FULLSCREEN;
            else if (strcmp(arg, "delayandquit") == 0)
            {
                if ((arg = argv[++i]) != NULL)
                    GDelayAndQuit = atoi(arg);
            } /* else if */
            else if (strcmp(arg, "buildver") == 0)
            {
                printf("%s\n", GBuildVer);
                return 0;
            } /* else if */
            else if (strcmp(arg, "license") == 0)
            {
                printf("%s\n", GLicense);
                return 0;
            } /* else if */
        } /* if */
        else
        {
            if (program != NULL)
            {
                fprintf(stderr, "More than one program specified.\n");
                return 1;
            } /* if */

            program = loadProgram(argv[i]);
            if (program == NULL)
                return 2;  /* error is output in loadProgram()... */
        } /* else */
    } /* for */

    if (program == NULL)
    {
        fprintf(stderr, "No program specified.\n");
        return 3;
    } /* if */

    retval = doProgram(program, w, h, sdlflags);
    free(program);
    return retval;
} /* main */

/* end of toby_sdl.c ... */

