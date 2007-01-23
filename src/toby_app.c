#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "toby_app.h"

/*
 * All this math code had brilliant comments explaining it, but as proud as I
 *  was to figure it out in high school...really, it's fundamental
 *  mathematical wankery. Check the repository or Google it.
 */
static inline lua_Number degreesToRadians(lua_Number degrees)
{
    /* !!! FIXME: fixed point... */
    return(degrees * (M_PI / 180.0));
} /* degreesToRadians */


static inline lua_Number radiansToDegrees(lua_Number radians)
{
    /* !!! FIXME: fixed point... */
    return(radians * (180.0 / M_PI));
} /* degreesToRadians */


static inline void calculateLine(lua_Number heading, lua_Number distance,
                                 lua_Number startX, lua_Number startY,
                                 lua_Number *endX, lua_Number *endY)
{
    /* !!! FIXME: fixed point... */
    assert(endX != NULL);
    assert(endY != NULL);

    lua_Number rad = degreesToRadians(heading);
    *endX = (cos(rad) * (lua_Number) distance) + startX;
    *endY = (sin(rad) * (lua_Number) distance) + startY;
} /* calculateLine */


static inline lua_Number pythagorian(lua_Number s1, lua_Number hypotenuse)
{
    /* !!! FIXME: fixed point... */
    return(sqrt((hypotenuse * hypotenuse) - (s1 * s1)));
} /* pythagorian */


/* convert a lua_Number between 0.0 and 1.0 to 0 to 255. */
static inline int to8bit(lua_Number x)
{
    /* !!! FIXME: fixed point... */
    return (int) (x * N(255));
} /* to8bit */


/* !!! FIXME: temporary hack. */
#define LUAHOOK(sym) \
    static int luahook_##sym(lua_State *L) { \
        printf("\t%s()\n", #sym); \
        return 0; \
    }

#define LUAHOOK_RETNUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        printf("\t%s()\n", #sym); \
        lua_pushnumber(L, 0.0f); \
        return 1; \
    }

#define LUAHOOK_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        printf("\t%s(%f)\n", #sym, (float) n1); \
        return 0; \
    }

#define LUAHOOK_STR(sym) \
    static int luahook_##sym(lua_State *L) { \
        const char *s1 = luaL_checkstring(L, 1); \
        printf("\t%s(\"%s\")\n", #sym, s1); \
        return 0; \
    }

#define LUAHOOK_NUM_RETNUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        printf("\t%s(%f)\n", #sym, (float) n1); \
        lua_pushnumber(L, 0.0f); \
        return 1; \
    }

#define LUAHOOK_NUM_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        const lua_Number n2 = luaL_checknumber(L, 2); \
        printf("\t%s(%f, %f)\n", #sym, (float) n1, (float) n2); \
        return 0; \
    }

#define LUAHOOK_NUM_NUM_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        const lua_Number n2 = luaL_checknumber(L, 2); \
        const lua_Number n3 = luaL_checknumber(L, 3); \
        printf("\t%s(%f, %f, %f)\n", #sym, (float) n1, (float) n2, (float) n3); \
        return 0; \
    }

LUAHOOK(showturtle)
LUAHOOK(hideturtle)
LUAHOOK(hometurtle)
LUAHOOK(enablefence)
LUAHOOK(disablefence)
LUAHOOK_STR(drawstring)
LUAHOOK_RETNUM(random)
LUAHOOK_RETNUM(getturtlex)
LUAHOOK_RETNUM(getturtley)
LUAHOOK_RETNUM(getturtlespacewidth)
LUAHOOK_RETNUM(getturtlespaceheight)
LUAHOOK(cleanupturtlespace)
LUAHOOK(setpenup)
LUAHOOK(setpendown)
LUAHOOK(addturtle)
LUAHOOK_NUM(useturtle)
LUAHOOK_NUM_RETNUM(round)
LUAHOOK_NUM_RETNUM(stringlength)
LUAHOOK_NUM(pause)
LUAHOOK_NUM(setangle)
LUAHOOK_NUM_NUM(setturtlexy)


static Turtle *currentTurtle = NULL;
static int fenceEnabled = 1;


static inline int checkWholeNum(lua_State *L, int idx)
{
    const lua_Number num = luaL_checknumber(L, idx);
    const int intnum = (int) num;
    if ((lua_Number) intnum != num)
    {
        /* !!! FIXME: fail if not whole number. */
    } /* if */

    return intnum;
} /* checkWholeNum */


static Turtle *newTurtle(void)
{
    Turtle *retval = (Turtle *) calloc(1, sizeof (Turtle));
    if (retval != NULL)
    {
        retval->x = retval->y = N(500);   /* center of turtlespace. */
        retval->r = retval->g = retval->b = N(255);  /* pure white. */
        retval->penDown = 1;
        retval->visible = 1;
    } /* if */
    return retval;
} /* newTurtle */


static inline Turtle *getTurtle(void)
{
    if (currentTurtle == NULL)
        return NULL;  /* !!! FIXME: throw no-current-turtle error. */
    return currentTurtle;
} /* getTurtle */


static inline void verifyPointInsideFence(lua_Number x, lua_Number y)
{
    if (fenceEnabled)
    {
        if ((x < N(0)) || (x > N(1000)) || (y < N(0)) || (y > N(1000)))
        {
            /* !!! FIXME: throw error: turtle hit fence. */
        } /* if */
    } /* if */
} /* verifyPointInsideFence */


static void driveTurtle(lua_Number distance)
{
    Turtle *turtle = getTurtle();
    if (distance != N(0))
    {
        lua_Number x, y;

        calculateLine(turtle->angle, distance, turtle->x, turtle->y, &x, &y);

// !!! FIXME: turtle rendering
//        if (turtle->visible)
//            TOBY_blankTurtle(t);

        if (turtle->penDown)   /* draw the line covering path turtle took? */
        {
            TOBY_drawLine(turtle->x, turtle->y, x, y,
                          turtle->r, turtle->g, turtle->b);
        } /* if */

        turtle->x = x;
        turtle->y = y;

// !!! FIXME: turtle rendering
//        if (turtle->visible)
//            TOBY_renderTurtle(t);

//    turtleSync->release();

        verifyPointInsideFence(x, y);
    } /* if */
} /* driveTurtle */


static int luahook_goforward(lua_State *L)
{
    driveTurtle(luaL_checknumber(L, 1));
    return 0;
} /* luahook_goforward */


static int luahook_gobackward(lua_State *L)
{
    driveTurtle(-luaL_checknumber(L, 1));
    return 0;
} /* luahook_gobackward */


static void setTurtleAngle(lua_Number angle)
{
    Turtle *turtle = getTurtle();
    if (angle != turtle->angle)
    {
// !!! FIXME
//        if (turtle->visible)
//            renderer->blankTurtle(turtle);

        // !!! FIXME: ugh, use modulus...
        while (angle > N(360)) angle -= N(360);
        while (angle < N(0)) angle += N(360);
        turtle->angle = angle;

// !!! FIXME
//        if (turtle->visible)
//            renderer->renderTurtle(turtle);
    } /* if */
} /* setTurtleAngle */


static void turnTurtle(lua_Number degree)
{
    Turtle *turtle = getTurtle();
    if (degree != N(0))
        setTurtleAngle(turtle->angle + degree);
} /* turnTurtle */


static int luahook_turnright(lua_State *L)
{
    turnTurtle(luaL_checknumber(L, 1));
    return 0;
} /* luahook_turnright */


static int luahook_turnleft(lua_State *L)
{
    turnTurtle(-luaL_checknumber(L, 1));
    return 0;
} /* luahook_turnleft */


static inline void setPenColorRGB(int r, int g, int b)
{
    Turtle *turtle = getTurtle();
    turtle->r = r;
    turtle->g = g;
    turtle->b = b;
} /* setPenColorRGB */


static int luahook_setpencolorrgb(lua_State *L)
{
    const lua_Number r = luaL_checknumber(L, 1);
    const lua_Number g = luaL_checknumber(L, 2);
    const lua_Number b = luaL_checknumber(L, 3);

    if ( (r < N(0)) || (r > N(1)) )
        ; /* !!! FIXME: throw error. */
    else if ( (g < N(0)) || (g > N(1)) )
        ; /* !!! FIXME: throw error. */
    else if ( (b < N(0)) || (b > N(1)) )
        ; /* !!! FIXME: throw error. */
    else
        setPenColorRGB(to8bit(r), to8bit(g), to8bit(b));

    return 0;
} /* luahook_setpencolorrgb */


static int luahook_setpencolor(lua_State *L)
{
    /*
     * How I got these numbers:
     *   These color choices are ripped directly from QuickBasic's
     *   COLOR command in text mode. So, (1) is dark blue, (2) is
     *   dark green, etc...Then I used the GIMP color tool
     *   (http://www.gimp.org/), to find the equivalents in RGB format.
     */
    static const struct { int r; int g; int b; } colors[] =
    {
        { 0, 0, 0 },       // black
        { 0, 26, 196 },    // dark blue
        { 0, 153, 45 },    // dark green
        { 0, 144, 138 },   // dark cyan
        { 160, 0, 0 },     // dark red
        { 197, 0, 202 },   // dark magenta
        { 129, 90, 16 },   // brown
        { 200, 200, 200 }, // gray
        { 90, 90, 90 },    // dark gray
        { 0, 200, 255 },   // bright blue
        { 0, 90, 38 },     // bright green
        { 91, 110, 255 },  // bright cyan
        { 255, 0, 0 },     // bright red
        { 255, 0, 255 },   // bright pink
        { 255, 255, 0 },   // bright yellow
        { 255, 255, 255 }, // bright white
    };

    const int color = checkWholeNum(L, 1);
    if ( (color < 0) || (color >= (STATICARRAYLEN(colors))) )
        ; /* !!! FIXME: throw error. */
    else
        setPenColorRGB(colors[color].r, colors[color].g, colors[color].b);
    return 0;
} /* luahook_setpencolor */


static void add_toby_functions(lua_State *L)
{
    #define SET_LUAHOOK(sym) \
        lua_pushcfunction(L, luahook_##sym); \
        lua_setglobal(L, #sym);
    SET_LUAHOOK(showturtle)
    SET_LUAHOOK(hideturtle)
    SET_LUAHOOK(hometurtle)
    SET_LUAHOOK(enablefence)
    SET_LUAHOOK(disablefence)
    SET_LUAHOOK(drawstring)
    SET_LUAHOOK(random)
    SET_LUAHOOK(getturtlespacewidth)
    SET_LUAHOOK(getturtlespaceheight)
    SET_LUAHOOK(getturtlex)
    SET_LUAHOOK(getturtley)
    SET_LUAHOOK(cleanupturtlespace)
    SET_LUAHOOK(setpencolorrgb)
    SET_LUAHOOK(setpenup)
    SET_LUAHOOK(setpendown)
    SET_LUAHOOK(addturtle)
    SET_LUAHOOK(useturtle)
    SET_LUAHOOK(round)
    SET_LUAHOOK(stringlength)
    SET_LUAHOOK(pause)
    SET_LUAHOOK(setangle)
    SET_LUAHOOK(goforward);
    SET_LUAHOOK(gobackward);
    SET_LUAHOOK(turnright);
    SET_LUAHOOK(turnleft);
    SET_LUAHOOK(setpencolor);
    SET_LUAHOOK(setturtlexy);
    #undef SET_LUAHOOK
}


static int luahook_stackwalk(lua_State *L)
{
    const char *errstr = lua_tostring(L, 1);

#if 0
    lua_Debug ldbg;
    int i = 0;

    if (errstr != NULL)
        logDebug("%s\n", errstr);

    logDebug("Lua stack backtrace:");

    // start at 1 to skip this function.
    for (i = 1; lua_getstack(L, i, &ldbg); i++)
    {
        char *ptr = (char *) scratchbuf_128k;
        size_t len = sizeof (scratchbuf_128k);
        int bw = snprintfcat(&ptr, &len, "#%d", i-1);
        const int maxspacing = 4;
        int spacing = maxspacing - bw;
        while (spacing-- > 0)
            snprintfcat(&ptr, &len, " ");

        if (!lua_getinfo(L, "nSl", &ldbg))
        {
            snprintfcat(&ptr, &len, "???\n");
            logDebug((const char *) scratchbuf_128k);
            continue;
        } // if

        if (ldbg.namewhat[0])
            snprintfcat(&ptr, &len, "%s ", ldbg.namewhat);

        if ((ldbg.name) && (ldbg.name[0]))
            snprintfcat(&ptr, &len, "function %s ()", ldbg.name);
        else
        {
            if (strcmp(ldbg.what, "main") == 0)
                snprintfcat(&ptr, &len, "mainline of chunk");
            else if (strcmp(ldbg.what, "tail") == 0)
                snprintfcat(&ptr, &len, "tail call");
            else
                snprintfcat(&ptr, &len, "unidentifiable function");
        } // if

        logDebug((const char *) scratchbuf_128k);
        ptr = (char *) scratchbuf_128k;
        len = sizeof (scratchbuf_128k);

        for (spacing = 0; spacing < maxspacing; spacing++)
            snprintfcat(&ptr, &len, " ");

        if (strcmp(ldbg.what, "C") == 0)
            snprintfcat(&ptr, &len, "in native code");
        else if (strcmp(ldbg.what, "tail") == 0)
            snprintfcat(&ptr, &len, "in Lua code");
        else if ( (strcmp(ldbg.source, "=?") == 0) && (ldbg.currentline == 0) )
            snprintfcat(&ptr, &len, "in Lua code (debug info stripped)");
        else
        {
            snprintfcat(&ptr, &len, "in Lua code at %s", ldbg.short_src);
            if (ldbg.currentline != -1)
                snprintfcat(&ptr, &len, ":%d", ldbg.currentline);
        } // else
        logDebug((const char *) scratchbuf_128k);
    } // for
#endif

    lua_pushstring(L, errstr ? errstr : "");
    return 1;
} // luahook_stackwalk


void TOBY_runProgram(const char *source_code)
{
    lua_State *L;

    TOBY_cleanup(0, 0, 0);
    currentTurtle = newTurtle();

    L = luaL_newstate();
    if (L == NULL)
        return;

// !!! FIXME
//    lua_atpanic(L, luahook_fatal);

    add_toby_functions(L);

    lua_pushcfunction(L, luahook_stackwalk);
    if (luaL_loadstring(L, source_code) != 0)
        TOBY_messageBox("lua_loadstring() failed");
    else
    {
        /* Call new chunk on top of the stack (lua_pcall will pop it off). */
        if (lua_pcall(L, 0, 0, -2) != 0)  // retvals are dumped.
            TOBY_messageBox("lua_pcall() failed");
        /* if this didn't panic, we succeeded. */
    } /* if */
    lua_pop(L, 1);   // dump stackwalker.

    free(currentTurtle);
    currentTurtle = NULL;
    lua_close(L);
} /* TOBY_runProgram */


const char *GLuaLicense =
"Lua:\n"
"\n"
"Copyright (C) 1994-2006 Lua.org, PUC-Rio.\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
"of this software and associated documentation files (the \"Software\"), to deal\n"
"in the Software without restriction, including without limitation the rights\n"
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
"copies of the Software, and to permit persons to whom the Software is\n"
"furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in\n"
"all copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE\n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n"
"THE SOFTWARE.\n"
"\n";

/* end of lua_glue.c ... */
