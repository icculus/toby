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
static int GRequestingQuit = 0;
static Uint32 GLastPumpEvents = 0;
static Uint32 GStopWatch = 0;
static lua_Number GTurtleSpaceSize = 0;

#define TOBY_PROFILE 1


void TOBY_startRun(void)
{
    GTurtleSpaceSize = (GScreen->h < GScreen->w) ? GScreen->h : GScreen->w;
    GRequestingQuit = 0;
    GLastPumpEvents = 0;
    GStopWatch = SDL_GetTicks();
} // TOBY_startRun


void TOBY_stopRun()
{
    #if TOBY_PROFILE
    printf("time execute: %ld\n", (unsigned long) (SDL_GetTicks()-GStopWatch));
    #endif
} /* TOBY_stopRun */


int TOBY_pumpEvents(void)
{
    SDL_Event e;
    const Uint32 now = SDL_GetTicks();
    if ((now - GLastPumpEvents) > 50)
    {
        TOBY_renderAllTurtles(0, 0);
        SDL_Flip(GScreen);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                GRequestingQuit = 1;
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
        GLastPumpEvents = SDL_GetTicks();   /* reset this for next call. */
    } /* if */

    return !GRequestingQuit;
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
    *x = GTurtleSpaceSize * (*x / N(1000));
    *y = GTurtleSpaceSize * (*y / N(1000));
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
    const int xoff = (GScreen->w - GTurtleSpaceSize) / 2;
    const int yoff = (GScreen->h - GTurtleSpaceSize) / 2;
    const int w = GScreen->w;
    const Uint32 pval = (b << 24) | (g << 16) | (r << 8);

    /* !!! FIXME: this is always 32-bpp at the moment. */
    Uint32 *p = (Uint32 *) GScreen->pixels;

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

    px = ((int)x1) + xoff;
    py = ((int)y1) + yoff;

    if ((dx == 1) && (dy == 1))
    {
        _D(("LFB: Single pixel line fastpath.\n"));
        *(((Uint32 *) p) + ((py * w) + px)) = pval;
    } /* if */

    else if (dx == 1)
    {
        _D(("LFB: Vertical line fastpath.\n"));
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        int incr = w * sdy;
        for (y = 0; y < dy; y++)
        {
            *linep = pval;
            linep += incr;
        } /* for */
    } /* else if */

    else if (dy == 1)
    {
        _D(("LFB: Horizontal line fastpath.\n"));
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        int incr = sdx;
        for (x = 0; x < dx; x++)
        {
            *linep = pval;
            linep += incr;
        } /* for */
    } /* else if */

    else if (dx >= dy)
    {
        _D(("LFB: Line with major axis of X.\n"));

        int lastX = px;
        int thisY = py;

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
        _D(("LFB: Line with major axis of Y.\n"));

        int lastY = py;
        int thisX = px;

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


void TOBY_drawTurtle(const Turtle *turtle, int intoBackingStore)
{
} /* TOBY_drawTurtle */


void TOBY_blankTurtle(const Turtle *turtle)
{
} /* TOBY_blankTurtle */


void TOBY_cleanup(int r, int g, int b)
{
    SDL_FillRect(GScreen, NULL, SDL_MapRGB(GScreen->format, r, g, b));
    SDL_Flip(GScreen);
} /* TOBY_cleanup */


int TOBY_delay(int ms)
{
    Uint32 now = SDL_GetTicks();
    const Uint32 end = now + ms;
    while (now < end)
    {
        if (!TOBY_pumpEvents())
            return 0;
        now = SDL_GetTicks();
        if (now < end)
        {
            const Uint32 ticks = end - now;
            SDL_Delay((ticks > 50) ? 50 : ticks);
            now = SDL_GetTicks();
        } /* if */
    } /* while */

    return TOBY_pumpEvents();
} /* TOBY_delay */


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
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n",
                SDL_GetError());
        return 4;
    } /* if */

    SDL_WM_SetCaption("Toby", "Toby");

    if ((GScreen = SDL_SetVideoMode(w, h, 32, flags)) == NULL)
    {
        fprintf(stderr, "SDL_SetVideoMode(%d, %d, 32, 0) failed: %s\n",
                w, h, SDL_GetError());
        SDL_Quit();
        return 5;
    } /* if */

    TOBY_runProgram(program, 0);

    while (TOBY_delay(100)) { /* no-op. */ }

    SDL_Quit();
    return 0;
} /* doProgram */


int main(int argc, char **argv)
{
    /* !!! FIXME: allow user to change dimensions. */
    const int w = 1024;
    const int h = 768;
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
            else if (strcmp(arg, "buildver") == 0)
            {
                printf("%s\n", GBuildVer);
                return 0;
            } /* if */
            else if (strcmp(arg, "license") == 0)
            {
                printf("%s\n", GLicense);
                return 0;
            } /* if */
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

