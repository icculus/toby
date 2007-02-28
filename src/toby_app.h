/*
 * Toby -- A programming language for learning.
 * Copyright (C) 2007  Ryan C. Gordon.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

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


typedef struct TurtlePoint
{
    lua_Number x;
    lua_Number y;
} TurtlePoint;

typedef struct TurtleRGB
{
    int r;
    int g;
    int b;
} TurtleRGB;

typedef struct Turtle
{
    TurtlePoint pos;
    lua_Number width;
    lua_Number height;
    lua_Number angle;
    TurtlePoint points[4];
    TurtleRGB pen;
    int penDown;
    int visible;
    int recalcPoints;  /* non-zero if (points) needs to be recalculated. */
    int dirty;  /* non-zero if turtle needs redrawing in TurtleSpace. */
} Turtle;


typedef struct TobyDebugInfo
{
    const char *name;
    const char *value;
    int linenum;
} TobyDebugInfo;


/* !!! FIXME: comment this */
void TOBY_background(int *r, int *g, int *b);


/* !!! FIXME: comment this */
void TOBY_renderAllTurtles(void *udata);


/* !!! FIXME: comment these */
const TobyDebugInfo *TOBY_getCallstack(int *elementCount);
const TobyDebugInfo *TOBY_getVariables(int stackframe, int *elementCount);


/* !!! FIXME: comment all these */
void TOBY_setDelayTicksPerLine(long ms);
long TOBY_getDelayTicksPerLine(void);
void TOBY_haltProgram(void);
void TOBY_stepProgram(void);
void TOBY_continueProgram(void);
int TOBY_isPaused(void);
int TOBY_isStepping(void);
int TOBY_isRunning(void);
int TOBY_isStopping(void);
void TOBY_stepProgram(void);


/*
 * Pause for (ms) milliseconds, calling TOBY_pumpEvents() every 50 ms or so,
 *  and TOBY_yieldCPU() to give up time between pumps. Returns 0 if
 *  TOBY_haltProgram() got called before or during the function's run,
 *  non-zero otherwise. Will pump the event queue at least once, even if ms
 *  is <= 0.
 */
int TOBY_delay(long ms);


/*
 * This can block for a LONG time, but it will call back into your
 *  application with the following functions...
 */
void TOBY_runProgram(const char *source_code, int run_for_printing);

/* !!! FIXME: comment me. */
void TOBY_pauseReached(int currentline, int breakpoint, int pauseTicks);


/*
 * The following are supplied by your app, and are called during the
 *  TOBY_runProgram() call...
 */

/* Set up for a new program run. This lets you prepare a backbuffer, etc. */
void TOBY_startRun(void);


/* Notify app that the program run has finished. */
void TOBY_stopRun(void);


/*
 * Let UI pump its event queue. Return non-zero to keep going, zero to
 *  stop program execution, making TOBY_runProgram() return.
 */
void TOBY_pumpEvents(void);

/* !!! FIXME: comment me. */
void TOBY_putToScreen(void);

/*
 * Put up a message box with "OK" message. Block until the user dismisses it.
 *  (msg) is UTF-8 encoded Unicode.
 */
void TOBY_messageBox(const char *msg);

/*
 * Draw line between (x1,y1) and (x2,y2) in color r,g,b. All coordinates are
 *  between 0 and 999, with (0,0) being the top left of the screen and
 *  ...(999, 999) being to bottom right. You need to scale to the correct
 *  coordinates for your display.
 *
 * All lines are clipped prior to this call, so they will never be outside
 *  the 0-999 range. If they don't intersect TurtleSpace at all, this
 *  function will not be called.
 */
void TOBY_drawLine(lua_Number x1, lua_Number y1, lua_Number x2, lua_Number y2,
                   int r, int g, int b);


/* !!! FIXME: comment me. */
int TOBY_drawString(lua_Number x, lua_Number y, const char *utf8str,
                    lua_Number angle, int r, int g, int b);


/*
 * Render a turtle of size (w,h) with the center at (x,y), facing (angle).
 *  Angle is between 0 and 360, coordinates and sizes are in the same system
 *  as TOBY_drawLine().
 * !!! FIXME: document (data).
 */
void TOBY_drawTurtle(const Turtle *turtle, void *data);


/*
 * Clean up turtlespace. Blank it out to color r,g,b (0 to 255 each).
 */
void TOBY_cleanup(int r, int g, int b);

/*
 * Get the time, in milliseconds, that the process has been running.
 */
long TOBY_getTicks(void);

/*
 * Surrender the CPU to other processes for (ms) milliseconds. This doesn't
 *  have to exact if the OS's scheduler doesn't have good precision, and it
 *  can just busy loop if the OS doesn't have facilities.
 * You don't have to pump the event queue here...callers are expected to
 *  alternate between this and TOBY_pumpEvents() if they are wasting time.
 */
void TOBY_yieldCPU(int ms);

/*
 * Clip a line defined by (*x1,*y1)-(*x2,*y2) to a rectangle of (0,0)-(w,h).
 *  x1, y1, x2, y2 are updated to reflect clipping. Returns zero if line
 *  is completely outside the rectangle, non-zero if some portion of the line
 *  intersects the rectangle.
 *
 * Frontends don't generally call this, since TOBY_drawLine() will clip the
 *  lines before calling the frontend (and not call it if the line isn't inside
 *  TurtleSpace), meaning frontends may not need to manipulate a clip region
 *  at all.
 */
int TOBY_clipLine(lua_Number *x1, lua_Number *y1,
                  lua_Number *x2, lua_Number *y2,
                  lua_Number w, lua_Number h);


extern const char *GLicense;
extern const char *GBuildVer;

#ifdef __cplusplus
}
#endif

#endif

/* end of toby_app.h ... */

