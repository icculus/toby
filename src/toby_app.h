#ifndef _INCL_TOBY_APP_H_
#define _INCL_TOBY_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

/* Try to handle platforms where lua_Number isn't a double. Whenever
 *  specifying a numeric constant, try to use whole numbers, and wrap them
 *  in N(), so that we don't have the compiler do an unnecessary implicit cast:
 *
 *    if (myLuaNum < N(0)) {}
 */
#ifdef N
#error N is defined somewhere.
#endif
#if defined(LUA_NUMBER_DOUBLE)
#define N(x) x##.0
#elif defined(LUA_NUMBER_FLOAT)
#define N(x) x##.0f
#else
#define N(x) x
#endif

#define STATICARRAYLEN(x) ( (sizeof ((x))) / (sizeof ((x)[0])) )

typedef struct Turtle
{
    lua_Number x;
    lua_Number y;
    lua_Number angle;
    int r, g, b;
    int penDown;
    int visible;
} Turtle;


/*
 * This can block for a LONG time, but it will call back into your
 *  application with the following functions...
 */
void TOBY_runProgram(const char *source_code);



/*
 * The following are supplied by your app, and are called during the
 *  TOBY_runProgram() call...
 */

/* Set up for a new program run. This lets you prepare a backbuffer, etc. */
void TOBY_startRun(void);


/*
 * Let UI pump its event queue. Return non-zero to keep going, zero to
 *  stop program execution, making TOBY_runProgram() return.
 */
int TOBY_pumpEvents(void);


/*
 * Put up a message box with "OK" message. Block until the user dismisses it.
 *  (msg) is UTF-8 encoded Unicode.
 */
void TOBY_messageBox(const char *msg);

/*
 * Draw line between (x1,y1) and (x2,y2) in color r,g,b. All coordinates are
 *  between 0 and 1000, with (0,0) being the top left of the screen and
 *  ...(1000, 1000) being to bottom right. You need to scale to the correct
 *  coordinates for your display.
 */
void TOBY_drawLine(int x1, int y1, int x2, int y2, int r, int g, int b);

/*
 * Render a turtle of size (w,h) with the center at (x,y), facing (angle).
 *  Angle is between 0 and 360, coordinates and sizes are in the same system
 *  as TOBY_drawLine().
 */
void TOBY_drawTurtle(int x, int y, int angle, int w, int h);

/*
 * Clean up turtlespace. Blank it out to color r,g,b.
 */
void TOBY_cleanup(int r, int g, int b);

extern const char *GLuaLicense;

#ifdef __cplusplus
}
#endif

#endif

/* end of toby_app.h ... */

