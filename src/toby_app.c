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
#include <math.h>
#include <assert.h>
#include <ctype.h>  // !!! FIXME: lose this with tolower/toupper...
#include "toby_app.h"

typedef struct TurtleStates
{
    Turtle current;
    Turtle shadowed;
} TurtleStates;


/* TurtlesSpace state... */
static int currentTurtleIndex = -1;
static int totalTurtles = 0;
static TurtleStates *turtles = NULL;
static int fenceEnabled = 1;
static int halted = 0;
static TurtleRGB background = { 0, 0, 0 };

void TOBY_background(int *r, int *g, int *b)
{
    *r = background.r;
    *g = background.g;
    *b = background.b;
} /* TOBY_background */


static inline void throwError(lua_State *L, const char *err)
{
    lua_pushstring(L, err);
    lua_error(L);
} /* throwError */


static inline void haltProgram(lua_State *L)
{
    halted = 1;
    throwError(L, "program halted");
} /* haltProgram */


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


/* This can be optimized into one FPU operation on x86 chips. */
static inline void sinAndCos(lua_Number x, lua_Number *sval, lua_Number *cval)
{
    // !!! FIXME: x86 version
    // !!! FIXME: fixed point?
    *sval = sin(x);
    *cval = cos(x);
} /* sinAndCos */


static inline void calculateLine(lua_Number heading, lua_Number distance,
                                 lua_Number startX, lua_Number startY,
                                 lua_Number *endX, lua_Number *endY)
{
    /* !!! FIXME: fixed point... */
    assert(endX != NULL);
    assert(endY != NULL);

    lua_Number rad = degreesToRadians(heading);
    lua_Number sinrad, cosrad;
    sinAndCos(rad, &sinrad, &cosrad);
    *endY = (sinrad * distance) + startY;
    *endX = (cosrad * distance) + startX;
} /* calculateLine */


#if 0   /* not used, yet. */
static inline lua_Number pythagorian(lua_Number s1, lua_Number hypotenuse)
{
    /* !!! FIXME: fixed point... */
    return(sqrt((hypotenuse * hypotenuse) - (s1 * s1)));
} /* pythagorian */
#endif


/* convert a lua_Number between 0.0 and 1.0 to 0 to 255. */
static inline int to8bit(lua_Number x)
{
    /* !!! FIXME: fixed point... */
    return (int) (x * N(255));
} /* to8bit */


static inline int secsToMs(lua_Number secs)
{
    /* !!! FIXME: fixed point... */
    return (int) (secs * N(1000));
} /* secsToMs */


/* Liang-Barsky line clipping algorithm implementation. */
static int clip(lua_Number *minimum, lua_Number *maximum,
                lua_Number directedProjection, lua_Number directedDistance)
{
    if (directedProjection == N(0))
    {
        if (directedDistance < N(0))
            return 0;
    } /* if */
    else
    {
        const lua_Number amount = directedDistance / directedProjection;
        if (directedProjection < N(0)) {
            if (amount > *maximum)
                return 0;
            else if (amount > *minimum)
                *minimum = amount;
        } /* if */
        else
        {
            if (amount < *minimum)
                return 0;
            else if (amount < *maximum)
                *maximum = amount;
        } /* else */
    } /* else */
    return 1;
} /* clip */


int TOBY_clipLine(lua_Number *_x1, lua_Number *_y1,
                  lua_Number *_x2, lua_Number *_y2,
                  lua_Number w, lua_Number h)
{
    const lua_Number x1 = *_x1;
    const lua_Number y1 = *_y1;
    const lua_Number x2 = *_x2;
    const lua_Number y2 = *_y2;
    const lua_Number px = x2 - x1;
    const lua_Number py = y2 - y1;
    lua_Number minimum = N(0);
    lua_Number maximum = N(1);

    if (clip(&minimum, &maximum, -px, x1 - N(0)))
    {
        if (clip(&minimum, &maximum, px, w - x1))
        {
            if (clip(&minimum, &maximum, -py, y1 - N(0)))
            {
                if (clip(&minimum, &maximum, py, h - y1))
                {
                    if (maximum < N(1))
                    {
                        *_x2 = x1 + maximum * px;
                        *_y2 = y1 + maximum * py;
                    } /* if */

                    if (minimum > N(0))
                    {
                        *_x1 += minimum * px;
                        *_y1 += minimum * py;
                    } /* if */
                    return 1;
                } /* if */
            } /* if */
        } /* if */
    } /* if */

    return 0;
} /* TOBY_clipLine */


static inline int checkWholeNum(lua_State *L, int idx)
{
    const lua_Number num = luaL_checknumber(L, idx);
    const int intnum = (int) num;
    if ( ((lua_Number) intnum) != num )
        throwError(L, "Expected whole number");

    return intnum;
} /* checkWholeNum */


static int allocateTurtle(lua_State *L)
{
    Turtle *current = NULL;
    TurtleStates *ptr = (TurtleStates *) realloc(turtles,
                        sizeof (TurtleStates) * (totalTurtles + 1));
    if (ptr == NULL)
        throwError(L, "Out of memory");

    turtles = ptr;
    ptr = &turtles[totalTurtles];
    memset(ptr, '\0', sizeof (TurtleStates));

    current = &ptr->current;
    current->pos.x = current->pos.y = N(500);   /* center of turtlespace. */
    current->width = current->height = N(20);
    current->pen.r = current->pen.g = current->pen.b = N(255);  /* white. */
    current->angle = 270;  /* due north. */
    current->penDown = 1;
    current->recalcPoints = 1;
    current->visible = 1;
    current->dirty = 1;

    return totalTurtles++;
} /* allocateTurtle */


static inline Turtle *getTurtle(lua_State *L)
{
    if (currentTurtleIndex < 0)
        throwError(L, "No current turtle");
    return &turtles[currentTurtleIndex].current;
} /* getTurtle */


/*
 * See comments in Turtle::calcTriangle() for detailed discussion:
 *  http://svn.icculus.org/toby/branches/toby_rewrite_4/src/turtlespace/Turtle.cpp?view=markup
 */
static inline void calculateTurtleTriangle(Turtle *turtle)
{
    if (turtle->recalcPoints)
    {
        const lua_Number w = turtle->width;
        const lua_Number h = turtle->height;
        const lua_Number angle = turtle->angle;
        const lua_Number halfW = w / N(2);
        TurtlePoint *pt = turtle->points;
        lua_Number tailX, tailY;

        calculateLine(angle + N(180), h / N(2), N(0), N(0), &tailX, &tailY);
        calculateLine(angle, h, tailX, tailY, &pt[0].x, &pt[0].y);
        calculateLine(angle + N(90), halfW, tailX, tailY, &pt[1].x, &pt[1].y);
        calculateLine(angle - N(90), halfW, tailX, tailY, &pt[2].x, &pt[2].y);
        pt[3].x = tailX;
        pt[3].y = tailY;

        turtle->recalcPoints = 0;
    } /* if */
} /* calculateTurtleTriangle */


/*
 * Figure out which turtles are "dirty" since the last call, and update them
 *  in TurtleSpace, blanking the old ("shadowed") state and rendering the
 *  current state.
 */
void TOBY_renderAllTurtles(int allDirty, int intoBackingStore)
{
    int i;
    for (i = 0; i < totalTurtles; i++)
    {
        Turtle *current = &turtles[i].current;
        Turtle *shadowed = &turtles[i].shadowed;
        if ((allDirty) || (current->dirty))
        {
            calculateTurtleTriangle(current);
            current->dirty = 0;
            if (shadowed->visible)
                TOBY_blankTurtle(shadowed);
            if (current->visible)
            {
                if (intoBackingStore)
                    TOBY_drawTurtle(current, 1);
                TOBY_drawTurtle(current, 0);
            } // if
            memcpy(shadowed, current, sizeof (Turtle));
        } /* if */
    } /* for */
} /* TOBY_renderAllTurtles */


static void setTurtleAngle(lua_State *L, lua_Number angle)
{
    Turtle *turtle = getTurtle(L);
    if (angle != turtle->angle)
    {
        // !!! FIXME: ugh, use modulus...
        while (angle >= N(360)) angle -= N(360);
        while (angle < N(0)) angle += N(360);
        turtle->angle = angle;
        turtle->dirty = turtle->recalcPoints = 1;
    } /* if */
} /* setTurtleAngle */


static int luahook_setangle(lua_State *L)
{
    setTurtleAngle(L, luaL_checknumber(L, 1));
    return 0;
} /* luahook_setangle */


static inline void turnTurtle(lua_State *L, lua_Number degree)
{
    Turtle *turtle = getTurtle(L);
    if (degree != N(0))
        setTurtleAngle(L, turtle->angle + degree);
} /* turnTurtle */


static int luahook_turnright(lua_State *L)
{
    turnTurtle(L, luaL_checknumber(L, 1));
    return 0;
} /* luahook_turnright */


static int luahook_turnleft(lua_State *L)
{
    turnTurtle(L, -luaL_checknumber(L, 1));
    return 0;
} /* luahook_turnleft */


static inline void testFence(lua_State *L, const Turtle *turtle)
{
    const lua_Number x = turtle->pos.x;
    const lua_Number y = turtle->pos.y;
    if ( (x < N(0)) || (x > N(1000)) || (y < N(0)) || (y > N(1000)) )
        throwError(L, "Turtle outside fence");
} /* testFence */


static void setTurtleXY(lua_State *L, lua_Number x, lua_Number y)
{
    Turtle *turtle = getTurtle(L);
    turtle->pos.x = x;
    turtle->pos.y = y;
    turtle->dirty = 1;

    if (fenceEnabled)
        testFence(L, turtle);
} /* setTurtleXY */


static int luahook_setturtlexy(lua_State *L)
{
    const lua_Number x = luaL_checknumber(L, 1);
    const lua_Number y = luaL_checknumber(L, 2);
    setTurtleXY(L, x, y);
    return 0;
} /* luahook_setturtlexy */


static int luahook_hometurtle(lua_State *L)
{
    setTurtleXY(L, N(500), N(500));
    return 0;
} /* luahook_hometurtle */


static void driveTurtle(lua_State *L, lua_Number distance)
{
    Turtle *turtle = getTurtle(L);
    if (distance != N(0))
    {
        lua_Number x1 = turtle->pos.x;
        lua_Number y1 = turtle->pos.y;
        lua_Number unclipped_x2, unclipped_y2;
        lua_Number x2, y2;
 
        calculateLine(turtle->angle, distance, x1, y1, &x2, &y2);
        unclipped_x2 = x2;
        unclipped_y2 = y2;
        if (turtle->penDown)   /* draw the line covering path turtle took? */
        {
            /* only draw if SOMETHING is inside TurtleSpace... */
            if (TOBY_clipLine(&x1, &y1, &x2, &y2, N(1000), N(1000)))
            {
                const TurtleRGB *pen = &turtle->pen;
                TOBY_drawLine(x1, y1, x2, y2, pen->r, pen->g, pen->b);
            } /* if */
        } /* if */
 
        setTurtleXY(L, unclipped_x2, unclipped_y2);
    } /* if */
} /* driveTurtle */


static int luahook_goforward(lua_State *L)
{
    driveTurtle(L, luaL_checknumber(L, 1));
    return 0;
} /* luahook_goforward */


static int luahook_gobackward(lua_State *L)
{
    driveTurtle(L, -luaL_checknumber(L, 1));
    return 0;
} /* luahook_gobackward */


static int luahook_getturtlex(lua_State *L)
{
    lua_pushnumber(L, getTurtle(L)->pos.x);
    return 1;
} /* luahook_getturtlex */


static int luahook_getturtley(lua_State *L)
{
    lua_pushnumber(L, getTurtle(L)->pos.y);
    return 1;
} /* luahook_getturtley */


static int luahook_getturtlespacewidth(lua_State *L)
{
    lua_pushnumber(L, N(1000));   // !!! FIXME: allow this to change?
    return 1;
} /* luahook_getturtlespacewidth */


static int luahook_getturtlespaceheight(lua_State *L)
{
    lua_pushnumber(L, N(1000));   // !!! FIXME: allow this to change?
    return 1;
} /* luahook_getturtlespaceheight */


static int luahook_setpenup(lua_State *L)
{
    getTurtle(L)->penDown = 0;
    return 0;
} /* luahook_setpenup */


static int luahook_setpendown(lua_State *L)
{
    getTurtle(L)->penDown = 1;
    return 0;
} /* luahook_setpendown */


static inline void setPenColorRGB(lua_State *L, int r, int g, int b)
{
    TurtleRGB *pen = &(getTurtle(L)->pen);
    pen->r = r;
    pen->g = g;
    pen->b = b;
} /* setPenColorRGB */


static int luahook_setpencolorrgb(lua_State *L)
{
    const lua_Number r = luaL_checknumber(L, 1);
    const lua_Number g = luaL_checknumber(L, 2);
    const lua_Number b = luaL_checknumber(L, 3);

    if ( (r < N(0)) || (r > N(1)) )
        throwError(L, "Red value is not between 0.0 and 1.0");
    else if ( (g < N(0)) || (g > N(1)) )
        throwError(L, "Green value is not between 0.0 and 1.0");
    else if ( (b < N(0)) || (b > N(1)) )
        throwError(L, "Blue value is not between 0.0 and 1.0");
    else
        setPenColorRGB(L, to8bit(r), to8bit(g), to8bit(b));

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
    static const TurtleRGB colors[] =
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
        throwError(L, "Color value is not between 0 and 15");
    else
        setPenColorRGB(L, colors[color].r, colors[color].g, colors[color].b);
    return 0;
} /* luahook_setpencolor */


static int luahook_showturtle(lua_State *L)
{
    Turtle *turtle = getTurtle(L);
    turtle->visible = 1;
    turtle->dirty = 1;
    return 0;
} /* luahook_showturtle */


static int luahook_hideturtle(lua_State *L)
{
    Turtle *turtle = getTurtle(L);
    turtle->visible = 0;
    turtle->dirty = 1;
    return 0;
} /* luahook_hideturtle */


static int luahook_enablefence(lua_State *L)
{
    int i;

    fenceEnabled = 1;

    for (i = 0; i < totalTurtles; i++)
        testFence(L, &turtles[i].current);

    return 0;
} /* luahook_enablefence */


static int luahook_disablefence(lua_State *L)
{
    fenceEnabled = 0;
    return 0;
} /* luahook_disablefence */


static int luahook_cleanupturtlespace(lua_State *L)
{
    // !!! FIXME: let user choose color?
    TOBY_cleanup(background.r, background.g, background.b);
    return 0;
} /* luahook_getturtlespaceheight */


// !!! FIXME: all must handle utf8....

static int luahook_stringlength(lua_State *L)
{
    // !!! FIXME: this is doing bytes, not chars...
    lua_pushinteger(L, (lua_Integer) strlen(luaL_checklstring(L, 1, NULL)));
    return 1;
} /* luahook_stringlength */


static int luahook_leftstring(lua_State *L)
{
    const char *str = luaL_checklstring(L, 1, NULL);
    const size_t origLen = strlen(str);
    char *buf = NULL;
    int charCount = checkWholeNum(L, 2);
    if (charCount < 0)
        throwError(L, "Negative string length");
    else if (charCount > origLen)
        charCount = origLen;

    // !!! FIXME: can we push a string without null terminating it to save the copy?
    buf = (char *) alloca(charCount + 1);
    memcpy(buf, str, charCount);
    buf[charCount] = '\0';

    lua_pushstring(L, buf);
    return 1;
} /* luahook_leftstring */


static int luahook_rightstring(lua_State *L)
{
    const char *str = luaL_checklstring(L, 1, NULL);
    const size_t origLen = strlen(str);
    int charCount = checkWholeNum(L, 2);
    if (charCount < 0)
        throwError(L, "Negative string length");
    else if (charCount > origLen)
        charCount = origLen;

    lua_pushstring(L, str + (origLen - charCount));
    return 1;
} /* luahook_rightstring */


static int luahook_joinstrings(lua_State *L)
{
    // !!! FIXME: this code sucks.
    const char *str1 = luaL_checklstring(L, 1, NULL);
    const char *str2 = luaL_checklstring(L, 2, NULL);
    char *buf = (char *) malloc(strlen(str1) + strlen(str2) + 1);
    if (buf == NULL)
        throwError(L, "Out of memory");
    strcpy(buf, str1);
    strcat(buf, str1);
    lua_pushstring(L, buf);
    free(buf);
    return 1;
} /* luahook_joinstrings */


static int luahook_substring(lua_State *L)
{
    const char *str = luaL_checklstring(L, 1, NULL);
    size_t origLen = strlen(str);
    char *buf = NULL;
    const int pos = checkWholeNum(L, 2);
    int charCount = checkWholeNum(L, 3);

    if (charCount < 0)
        throwError(L, "Negative string length");

    if (pos < 0)
        throwError(L, "Negative string position");  // !!! FIXME: just clamp instead?

    origLen -= pos;
    if (origLen <= 0)
        lua_pushstring(L, "");
    else
    {
        if (charCount > origLen)
            charCount = origLen;
        buf = (char *) alloca(charCount + 1);
        memcpy(buf, str+pos, charCount);
        buf[charCount] = '\0';
        lua_pushstring(L, buf);
    } /* else */

    return 1;
} /* luahook_substring */


static int luahook_uppercasestring(lua_State *L)
{
    const char *str = luaL_checklstring(L, 1, NULL);
    char *buf = (char *) alloca(strlen(str) + 1);
    while (*str)
        *(buf++) = toupper(*(str++));
    *buf = '\0';
    lua_pushstring(L, buf);
    return 1;
} /* luahook_uppercasestring */


static int luahook_lowercasestring(lua_State *L)
{
    const char *str = luaL_checklstring(L, 1, NULL);
    char *buf = (char *) alloca(strlen(str) + 1);
    while (*str)
        *(buf++) = tolower(*(str++));
    *buf = '\0';
    lua_pushstring(L, buf);
    return 1;
} /* luahook_lowercasestring */


static int luahook_drawstring(lua_State *L)
{
    const Turtle *turtle = getTurtle(L);
    const char *utf8str = luaL_checklstring(L, 1, NULL);
    if (!TOBY_drawString(turtle->pos.x, turtle->pos.y, utf8str, turtle->angle,
                         turtle->pen.r, turtle->pen.g, turtle->pen.b))
    {
        throwError(L, "Platform doesn't support string drawing");
    } /* if */

    return 0;
} /* luahook_drawstring */


static int luahook_random(lua_State *L)
{
    lua_pushnumber(L, random());  // !!! FIXME: seed this.
    return 1;
} /* luahook_random */


static int luahook_round(lua_State *L)
{
    // !!! FIXME: fixed point.
    lua_pushinteger(L, (lua_Integer) (luaL_checknumber(L, 1) + 0.5));
    return 1;
} /* luahook_setpendown */


static int luahook_pause(lua_State *L)
{
    const lua_Number secs = luaL_checknumber(L, 1);
    const int ms = secsToMs(secs);
    if (!TOBY_delay(ms))
        haltProgram(L);
    return 0;
} /* luahook_pause */


static int luahook_addturtle(lua_State *L)
{
    allocateTurtle(L);
    return 0;
} /* luahook_addturtle */


static int luahook_useturtle(lua_State *L)
{
    const int newidx = checkWholeNum(L, 1);
    if ((newidx < 0) || (newidx >= totalTurtles))
        throwError(L, "Not a valid turtle");

    currentTurtleIndex = newidx;
    return 0;
} /* luahook_useturtle */


static int luahook_print(lua_State *L)
{
    printf("%s\n", luaL_checklstring(L, 1, NULL));
    return 0;
} /* luahook_print */


static void add_toby_functions(lua_State *L)
{
    #define SET_LUAHOOK(sym) \
        lua_pushcfunction(L, luahook_##sym); \
        lua_setglobal(L, #sym);
    SET_LUAHOOK(showturtle);
    SET_LUAHOOK(hideturtle);
    SET_LUAHOOK(hometurtle);
    SET_LUAHOOK(enablefence);
    SET_LUAHOOK(disablefence);
    SET_LUAHOOK(drawstring);
    SET_LUAHOOK(random);
    SET_LUAHOOK(getturtlespacewidth);
    SET_LUAHOOK(getturtlespaceheight);
    SET_LUAHOOK(getturtlex);
    SET_LUAHOOK(getturtley);
    SET_LUAHOOK(cleanupturtlespace);
    SET_LUAHOOK(setpencolorrgb);
    SET_LUAHOOK(setpenup);
    SET_LUAHOOK(setpendown);
    SET_LUAHOOK(addturtle);
    SET_LUAHOOK(useturtle);
    SET_LUAHOOK(round);
    SET_LUAHOOK(stringlength);
    SET_LUAHOOK(rightstring);
    SET_LUAHOOK(leftstring);
    SET_LUAHOOK(substring);
    SET_LUAHOOK(uppercasestring);
    SET_LUAHOOK(lowercasestring);
    SET_LUAHOOK(joinstrings);
    SET_LUAHOOK(pause);
    SET_LUAHOOK(setangle);
    SET_LUAHOOK(goforward);
    SET_LUAHOOK(gobackward);
    SET_LUAHOOK(turnright);
    SET_LUAHOOK(turnleft);
    SET_LUAHOOK(setpencolor);
    SET_LUAHOOK(setturtlexy);
    SET_LUAHOOK(print);
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


static void luaDebugHook(lua_State *L, lua_Debug *ar)
{
    if (!TOBY_pumpEvents())
        haltProgram(L);
} /* luaDebugHook */


static inline void resetProgramState(void)
{
    free(turtles);
    turtles = NULL;
    currentTurtleIndex = -1;
    totalTurtles = 0;
    fenceEnabled = 1;
    halted = 0;
} /* resetProgramState */


static void luaErrorMsgBox(lua_State *L)
{
    const char *errstr = lua_tostring(L, -1);
    TOBY_messageBox(errstr);
    lua_pop(L, 1);  /* dump error string. */
} /* luaErrorMsgBox */


void TOBY_runProgram(const char *source_code, int run_for_printing)
{
    const int mask = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT;
    lua_State *L;

    resetProgramState();

    if (run_for_printing)
        background.r = background.g = background.b = 255;  // white.
    else
        background.r = background.g = background.b = 0;  // black.

    L = luaL_newstate();
    if (L == NULL)
        return;

// !!! FIXME
//    lua_atpanic(L, luahook_fatal);
    lua_sethook(L, luaDebugHook, mask, 1000);
    add_toby_functions(L);

    lua_pushcfunction(L, luahook_stackwalk);
    if (luaL_loadstring(L, source_code) != 0)
        luaErrorMsgBox(L);
    else
    {
        TOBY_startRun();
        TOBY_cleanup(background.r, background.g, background.b);
        currentTurtleIndex = allocateTurtle(L);

        /* Call new chunk on top of the stack (lua_pcall will pop it off). */
        if (lua_pcall(L, 0, 0, -2) != 0)  // retvals are dumped.
        {
            if (!halted)  /* (halted) means stop requested, not error. */
                luaErrorMsgBox(L);
        } /* if */
        TOBY_renderAllTurtles(1, 1);
        TOBY_stopRun();
    } /* if */
    lua_pop(L, 1);   // dump stackwalker.

    resetProgramState();
    lua_close(L);
} /* TOBY_runProgram */



const char *GLicense =
"Toby, a programming language for learning.\n"
"http://icculus.org/toby/\n"
"Copyright (C) 2007 Ryan C. Gordon <icculus@icculus.org>\n"
"\n"
"This program is free software; you can redistribute it and/or modify "
"it under the terms of the GNU General Public License as published by "
"the Free Software Foundation; either version 2 of the License, or "
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful, "
"but WITHOUT ANY WARRANTY; without even the implied warranty of "
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License "
"along with this program; if not, write to the Free Software "
"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, "
"USA.\n"
"\n"
"\n"
"Toby uses code from Lua. The Lua copyright follows.\n"
"\n"
"Copyright (C) 1994-2006 Lua.org, PUC-Rio.\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy "
"of this software and associated documentation files (the \"Software\"), to deal "
"in the Software without restriction, including without limitation the rights "
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
"copies of the Software, and to permit persons to whom the Software is "
"furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in "
"all copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE "
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN "
"THE SOFTWARE.\n"
"\n";

/* end of toby_app.c ... */

