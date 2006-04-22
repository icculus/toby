#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "SDL.h"

#include "unixbits.h"

#define STUBBED(x) printf("STUBBED: %s\n", x);
#define _D(x)

struct rgb
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

static struct rgb rgbs[] =
{
    { 0, 0, 0 },       /* black */
    { 0, 26, 196 },    /* dark blue */
    { 0, 153, 45 },    /* dark green */
    { 0, 144, 138 },   /* dark cyan */
    { 160, 0, 0 },     /* dark red */
    { 197, 0, 202 },   /* dark magenta */
    { 129, 90, 16 },   /* brown */
    { 200, 200, 200 }, /* gray */
    { 90, 90, 90 },    /* dark gray */
    { 0, 200, 255 },   /* bright blue */
    { 0, 90, 38 },     /* bright green */
    { 91, 110, 255 },  /* bright cyan */
    { 255, 0, 0 },     /* bright red */
    { 255, 0, 255 },   /* bright pink */
    { 255, 255, 0 },   /* bright yellow */
    { 255, 255, 255 }, /* bright white */
};

static short GColor = 15;
static Uint32 GColor32 = 0;
static SDL_Surface *GScreen = NULL;
static SDL_Rect GViewport = { 0, 0, 0, 0 };
struct xycoord GPos = { 0, 0 };
struct rccoord GTextPos = { 0, 0 };

char *itoa(int val, char *buf, int radix)
{
    assert(radix == 10);
    sprintf(buf, "%d", val);
    return buf;
}

void _outtext(char *text)
{
    STUBBED("_outtext");
}

static Uint32 calcColor32(short c)
{
    return SDL_MapRGB(GScreen->format, rgbs[c].r, rgbs[c].g, rgbs[c].b);
}

short _setcolor(short color)
{
    if ((color < 0) || (color > 15))
        return -1;

    short prev = GColor;
    GColor = color;
    GColor32 = calcColor32(color);
    return prev;
}

short _getcolor(void)
{
    return GColor;
}

void _setpixel(short x, short y)
{
    char *ptr = (char *) GScreen->pixels;
    ptr += (y * GScreen->pitch) + (x * GScreen->format->BytesPerPixel);
    *((Uint32 *) ptr) = GColor32;
    SDL_UpdateRect(GScreen, x, y, 1, 1);  /* this is NOT efficient. */
}

short _setvideomode(short mode)
{
    SDL_Quit();
    GScreen = NULL;

    if (mode == _DEFAULTMODE)
        return 1;

    else if (mode == _MAXRESMODE)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
            return 0;

        GScreen = SDL_SetVideoMode(640, 480, 32, 0);
        if (GScreen != NULL)
        {
            GViewport.x = 0;
            GViewport.y = 0;
            GViewport.w = GScreen->w;
            GViewport.h = GScreen->h;
            GColor32 = calcColor32(GColor);
            SDL_WM_SetCaption("Toby", "toby");
            SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                                SDL_DEFAULT_REPEAT_INTERVAL);
            return 1;
        }
    }

    return 0;
}

char getch(void)
{
    SDL_Event e;

    if (!GScreen)
        return ((char) getchar());

    while (1)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_KEYDOWN:
                    if (e.key.keysym.mod & KMOD_SHIFT)
                        e.key.keysym.sym = toupper(e.key.keysym.sym);
                    if (e.key.keysym.sym < 127)
                        return((char) e.key.keysym.sym);
                    break;

                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                    break;
            }
        }
    }

    return 0;
}

void _clearscreen(short target)
{
    if (target != _GVIEWPORT)
        return;

    SDL_FillRect(GScreen, &GViewport, 0);
    SDL_Flip(GScreen);
}


void _getimage(short x1, short y1, short x2, short y2, char *image)
{
    int w, h, x, y;
    Uint32 *wptr;

    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 >= GScreen->w)
        x2 = GScreen->w - 1;
    if (y2 >= GScreen->h)
        y2 = GScreen->h - 1;

    w = x2 - x1;
    h = y2 - y1;

    ((Uint32 *) image)[0] = w;
    ((Uint32 *) image)[1] = h;
    wptr = (Uint32 *) (image + sizeof (Uint32) * 2);

    for (y = y1; y <= y2; y++)
    {
        for (x = x1; x <= x2; x++)
        {
            Uint32 *rptr = (Uint32 *) (GScreen->pixels +
                ((y * GScreen->pitch) + (x * GScreen->format->BytesPerPixel)));
            *(wptr++) = *rptr;
        }
    }
}

void _putimage(short _x, short _y, char *image, short action)
{
    assert(action == _GPSET);
    Uint32 w = ((Uint32 *) image)[0];
    Uint32 h = ((Uint32 *) image)[1];
    Uint32 *rptr = (Uint32 *) (image + sizeof (Uint32) * 2);
    int x1 = _x;
    int x2 = _x + w;
    int y1 = _y;
    int y2 = _y + h;
    int x, y;

    for (y = y1; y <= y2; y++)
    {
        if ((y < 0) || (y >= GScreen->h))
            rptr += w;
        else
        {
            for (x = x1; x <= x2; x++)
            {
                if ((x < 0) || (x >= GScreen->w))
                    rptr++;
                else
                {
                    Uint32 *wptr = (Uint32 *) (GScreen->pixels +
                        ((y * GScreen->pitch) + (x * GScreen->format->BytesPerPixel)));
                    *wptr = *(rptr++);
                }
            }
        }
    }

    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 >= GScreen->w)
        x2 = GScreen->w - 1;
    if (y2 >= GScreen->h)
        y2 = GScreen->h - 1;

    SDL_UpdateRect(GScreen, x1, y1, x2-x1, y2-y1);
}

void _settextwindow(short r1, short c1, short r2, short c2)
{
    STUBBED("_settextwindow");
}

int _imagesize(int x1, int y1, int x2, int y2)
{
    int w, h;

    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 >= GScreen->w)
        x2 = GScreen->w - 1;
    if (y2 >= GScreen->h)
        y2 = GScreen->h - 1;

    w = x2 - x1;
    h = y2 - y1;

    return (sizeof (Uint32) + sizeof (Uint32) + (w * h * 4));
}

struct xycoord _moveto(short x, short y)
{
    static struct xycoord prev;
    prev.xcoord = GPos.xcoord;
    prev.ycoord = GPos.ycoord;
    GPos.xcoord = x;
    GPos.ycoord = y;
    return prev;
}



short _lineto(short _x, short _y)
{
    int x1 = GPos.xcoord;
    int y1 = GPos.ycoord;
    int x2 = _x;
    int y2 = _y;
    int w = GScreen->w;

    GPos.xcoord = _x;
    GPos.ycoord = _y;

    // This is a standard Bresenham line-drawing algorithm, but the specific
    //  implementation was borrowed, optimized, and mangled from
    //  SGE's DoLine code:
    //   http://www.etek.chalmers.se/~e8cal1/sge/

    #define LINEBLITCHUNKS 10
    SDL_Rect rects[LINEBLITCHUNKS];
    Uint32 *p = GScreen->pixels;

    _D(("LFB: rendering line...(%d, %d)-(%d, %d), (%f, %f, %f, %f)...\n",
        x1, y1, x2, y2, r, g, b, a));

	int dx, dy, sdx, sdy, x, y, px, py, incr;

    dx = x2 - x1;
    dy = y2 - y1;

    sdx = (dx < 0) ? -1 : 1;
    sdy = (dy < 0) ? -1 : 1;

    dx = sdx * dx + 1;
    dy = sdy * dy + 1;

    x = y = 0;

    px = x1;
    py = y1;

    if ((dx == 1) && (dy == 1))
    {
        _D(("LFB: Single pixel line fastpath.\n"));
        *(((Uint32 *) p) + ((py * w) + px)) = GColor32;
        SDL_UpdateRect(GScreen, px, py, 1, 1);
        return 1;
    } // if

    else if (dx == 1)
    {
        _D(("LFB: Vertical line fastpath.\n"));
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        incr = w * sdy;
        for (y = 0; y < dy; y++)
        {
            *linep = GColor32;
            linep += incr;
        } // for
        SDL_UpdateRect(GScreen, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, 1, dy);
        return 1;
    } // else if

    else if (dy == 1)
    {
        _D(("LFB: Horizontal line fastpath.\n"));
        Uint32 *linep = ((Uint32 *) p) + ((py * w) + px);
        incr = sdx;
        for (x = 0; x < dx; x++)
        {
            *linep = GColor32;
            linep += incr;
        } // for
        SDL_UpdateRect(GScreen, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, dx, 1);
        return 1;
    } // else if

    else if (dx >= dy)
    {
        _D(("LFB: Line with major axis of X.\n"));

        int lastX = px;
        int thisY = py;
        int rectidx = 0;

        for (x = 0; x < dx; x++)
        {
            *( ((Uint32 *) p) + ((py * w) + px) ) = GColor32;

            y += dy;
            if (y >= dx)   // run length completed.
            {
                if (rectidx != -1)
                {
                    rects[rectidx].x = (lastX < px) ? lastX : px;
                    rects[rectidx].y = thisY;
                    rects[rectidx].w = (px - lastX) * sdx;
                    rects[rectidx].h = 1;

                    rectidx++;
                    if (rectidx >= LINEBLITCHUNKS)
                        rectidx = -1;
                } // if

                y -= dx;
                py += sdy;

                lastX = px;
                thisY = py;
            } // if
            px += sdx;
        } // for

        if (rectidx != -1)
            SDL_UpdateRects(GScreen, rectidx, rects);
        else
            goto tobylfb_blit_slanted_line;

        return 1;
    } // else if

    else
    {
        _D(("LFB: Line with major axis of Y.\n"));

        int lastY = py;
        int thisX = px;
        int rectidx = 0;

        for (y = 0; y < dy; y++)
        {
            *( ((Uint32 *) p) + ((py * w) + px) ) = GColor32;

            x += dx;
            if (x >= dy)  // run length completed.
            {
                if (rectidx != -1)
                {
                    rects[rectidx].x = thisX;
                    rects[rectidx].y = (lastY < py) ? lastY : py;
                    rects[rectidx].w = 1;
                    rects[rectidx].h = (py - lastY) * sdy;

                    rectidx++;
                    if (rectidx >= LINEBLITCHUNKS)
                        rectidx = -1;
                } // if

                x -= dy;
                px += sdx;

                lastY = py;
                thisX = px;
            } // if
            py += sdy;
        } // for

        if (rectidx != -1)
            SDL_UpdateRects(GScreen, rectidx, rects);
        else
            goto tobylfb_blit_slanted_line;

        return 1;
    } // else

    assert(0);  // should have return'd or goto'd by here.

tobylfb_blit_slanted_line:
        // !!! This isn't rendering right in all cases.
  {
    int i;
    float chunkX = ((float) dx) / ((float) LINEBLITCHUNKS);
    float chunkY = ((float) dy) / ((float) LINEBLITCHUNKS);
    float yincr;
    float fx, fy;

    fx = (float) ((x1 < x2) ? x1 : x2);

    if ( ((sdx > 0) && (sdy > 0)) ||   // line from q1 to q4.
         ((sdx < 0) && (sdy < 0)) )
    {
        fy = (float) ((y1 < y2) ? y1 : y2);
        yincr = chunkY;
    } // if

    else   // line from q3 to q2.
    {
        fy = (float) (((y1 > y2) ? y1 : y2)) - chunkY;
        yincr = -chunkY;
    } // else

    for (i = 0; i < LINEBLITCHUNKS; i++)
    {
        rects[i].x = (int) (fx);
        rects[i].y = (int) (fy);
        rects[i].w = (int) (chunkX + 0.5);
        rects[i].h = (int) (chunkY + 0.5);

        if (rects[i].w == 0)
            rects[i].w++;

        if (rects[i].h == 0)
            rects[i].h++;

        fx += chunkX;
        fy += yincr;
    } // for

    SDL_UpdateRects(GScreen, LINEBLITCHUNKS, rects);
  }
    return 1;
}

struct rccoord _gettextposition(void)
{
    return GTextPos;
}

struct rccoord _settextposition(short row, short column)
{
    static struct rccoord prev;
    prev.row = GTextPos.row;
    prev.col = GTextPos.col;
    GTextPos.row = row;
    GTextPos.col = column;
    return prev;
}


struct videoconfig *_getvideoconfig(struct videoconfig *config)
{
    memset(config, '\0', sizeof (struct videoconfig));
    config->numxpixels = GScreen->w;
    config->numypixels = GScreen->h;
    config->numtextcols = GScreen->w / 10;
    config->numtextrows = GScreen->h / 10;
    return config;
}

/* end of unixbits.c ... */

