/**********************************************
 * Graphical/Video related routines for TOBY. *
 **********************************************
 * Copyright (c) Ryan C. Gordon, 1995         *
 * See copyright info in TOBY.C.              *
 **********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "toby.h"

#ifdef __GNUC__
#include "unixbits.h"
#else
#include <graph.h>
#endif

extern char graphic_flags;            /* Bitmap of flags for graphics.     */
extern short turtle_x;                /* Turtle's X coordinate.            */
extern short turtle_y;                /* Turtle's Y coordinate.            */
extern short pen_color;               /* Current pen color.                */
extern short turtle_heading;          /* the Turtle's heading : 0-360 deg. */

static struct videoconfig crt_info;   /* Information regarding video.      */
static void *video_buffer;            /* Video buffer.                     */
static char sidelength;               /* length of one side of the Turtle. */


/* functions/procedures... */

void init_crt(void)
/*
 * Initializes variables related to graphics and sets up screen for LOGOing.
 *
 *   params : void.
 *  returns : void.
 *     also : Video mode changes, memory potentially allocated.
 */
{
    int buffer_size;              /* Size of a virtual video page. */
    int adjust;

    if (_setvideomode(_MAXRESMODE) == FALSE)
        logofinish("Can't set graphics mode.", ERLV_BAD);

    _getvideoconfig(&crt_info);

    _settextwindow(crt_info.numtextrows - 5, 0, crt_info.numtextrows,
            crt_info.numtextcols);

    pen_color = _getcolor();
    
    /* calculate one side of the Turtle's length. */
    sidelength = (char) ((crt_info.numxpixels / TURTLERATIO) +
            (crt_info.numypixels / TURTLERATIO)) / 2;

    adjust = (sidelength / 2) + 5;

    buffer_size = _imagesize(100 - adjust, 100 - adjust, 
            100 + adjust, 100 + adjust);
    video_buffer = allocate(NULL, buffer_size);

    resetscreen();

} /*init_crt*/



void deinit_crt(void)
/*
 * This routine frees any video buffers and returns the screen to default
 *  video mode.
 *
 *   params : void.
 *  returns : void.
 *     also : Memory allocation and screen mode changed.
 */
{

    _setvideomode(_DEFAULTMODE);
    _clearscreen(_GCLEARSCREEN);
    free(video_buffer);

} /*deinit_crt*/



void saveimage(int x, int y)
/*
 * Copy image on screen to memory buffer, so drawing the Turtle won't destroy
 *  the image.
 *
 *  params : x, y == coordinate of center pixel of turtle.
 * returns : void.
 *    also : make sure that there's enough allocated space pointed to by
 *            video_buffer.
 */
{
    
    int adjust = (sidelength / 2) + 5;
    
    _getimage(x - adjust, y - adjust, 
            x + adjust, y + adjust, video_buffer);

} /*saveimage*/


void restoreimage(int x, int y)
/*
 *  Output saved image from video_buffer to the screen at specified
 *    coordinates.
 *
 *   params : x, y == center coordinates where you want image restored.
 *  returns : void.
 */
{
    int adjust = (sidelength / 2) + 5;
    
    _putimage(x - adjust, y - adjust, video_buffer, _GPSET);

} /*restoreimage*/



void home(void)    
/*
 * Move the Turtle to the center of the screen, and face him north.
 *
 *   params : void.
 *  returns : void.
 */
{

    restoreimage(turtle_x, turtle_y);
    
    turtle_heading = 0;
    
    turtle_x = (crt_info.numxpixels / 2);
    turtle_y = ((crt_info.numypixels / crt_info.numtextrows) *
                    (crt_info.numtextrows - 5)) / 2; 
    
    drawturtle(turtle_x, turtle_y);

} /*home*/



void resetscreen(void)
/*
 * This procedure blanks all used pages of video memory, and resets the
 *   turtle's location to center screen.
 *
 *   params : void.
 *  returns : void.
 */
{

    clean();
    home();

} /*resetscreen*/


void clean(void)
/* 
 *  Clear screen without changing turtle...
 *
 *   params : void.
 *  returns : void.
 */
{
    /* Y axis of separator between graphics and text windows... */
    int y = ((crt_info.numypixels / crt_info.numtextrows) * 
            (crt_info.numtextrows - 6) - 1); 
    
    _clearscreen(_GVIEWPORT);
    
    /* Clear out buffer. */
    saveimage(crt_info.numxpixels / 2, crt_info.numypixels / 2);      
    
    _moveto(0, y); 
    _lineto(crt_info.numxpixels, y);

} /*clean*/


void _outchar(char outthis)
/*
 * _outtext() for a single character.
 *
 *   params : outthis == character to output.
 *  returns : void.
 */
{

    char outstr[] = {outthis, '\0'};  /* convert to a null-term'd string. */

    _outtext(outstr);                 /* ...and print it. */

} /*_outchar*/



void drawturtle(short x, short y)
/*
 * Draw a 'turtle', that little triangle, on the screen at x, y coordinates.
 *  params  : the x and y is the center pixel of the triangle.
 *  returns : void.
 */
{
    char looper;          /* Loop control.                       */
    short temp_heading;   /* Calculates each angle of triangle.  */
    short temp_x;         /* Calculate coordinates of...         */
    short temp_y;         /*   ..each angle of the triangle.     */
    
    saveimage(turtle_x, turtle_y);
    
    if (!(graphic_flags & HIDETURTLE))
    {

        _setcolor(10);   /* green? */
        
        calcline(turtle_heading, (sidelength / 2), x, y, &temp_x, &temp_y);

        temp_heading = turtle_heading + 150;     /* Set up for...        */
        _moveto(temp_x, temp_y);                 /*   ...actual drawing. */

        for (looper = 1; looper <= 3; looper++)
        {
            calcline(temp_heading, sidelength, temp_x, temp_y,
                    &temp_x, &temp_y);

            _lineto(temp_x, temp_y);

            temp_heading += 120;   /* Equilateral triangle; 60 degrees.   */
                                   /* (straight line == 60 + 120 == 180.) */
        } /*for*/
        
        _setcolor(9);  /* Blue? */
        calcline(turtle_heading + 180, sidelength, temp_x, temp_y, 
                &temp_x, &temp_y);
        _lineto(temp_x, temp_y);
        
        _setcolor(pen_color);
    
    } /*if*/

} /*drawturtle*/



void moveturtle(short distance)
/* 
 * Move the Turtle's location to another point in Turtlespace. If the pen
 *  is down, so to speak, a line is drawn, otherwise, the location of the
 *  turtle only is changed. If passing the fence should generate an error, 
 *  that will be checked, and runtime is toggled accordingly.
 *
 *   params : distance == distance turtle should move. 
 *            (heading and current location are stored in module-wide
 *              variables.)
 *  returns : void.
 */
{

    restoreimage(turtle_x, turtle_y);
    _moveto(turtle_x, turtle_y);
    
    calcline(turtle_heading, distance, turtle_x, turtle_y, 
            &turtle_x, &turtle_y);
    
    if (graphic_flags & PEN_UP)
        ;    
    
    else if (graphic_flags & PEN_REVERSE)
        /* not yet implemented. */;
    
    else if (graphic_flags & PEN_ERASE)
    {    
        _setcolor(0);
        _lineto(turtle_x, turtle_y);
    } /*else if*/
    
    else      /* Pen is down, and writing... */
    {    
        _setcolor(pen_color);
        _lineto(turtle_x, turtle_y);
    } /*else*/
    
    /* Has the Turtle overstepped the 'fence'? Should it cause an error?... */

    if ((graphic_flags & FENCE) && ((turtle_x < 0) || (turtle_x > crt_info.numxpixels) ||
            (turtle_y < 0) || (turtle_y > crt_info.numypixels)))
        logo_error("Turtle has passed fence.", RT_PASTFENCE);

    drawturtle(turtle_x, turtle_y);

} /*moveturtle*/



void turtleturn(short rotation)
/*
 * Rotate the turtle, setting a new heading.
 *
 *   params : rotation == number of degrees, positive or negative, to 
 *                        rotate Turtle.
 *  returns : void. turtle_heading is modified, though.
 */
{
    restoreimage(turtle_x, turtle_y);

    turtle_heading = turtle_heading + rotation;

    while (turtle_heading >= 360)
        turtle_heading -= 360;
    
    while (turtle_heading < 0)
        turtle_heading += 360;
    
    drawturtle(turtle_x, turtle_y);

} /*turtleturn*/


void calcline(short heading, short distance, short startx, short starty, 
                short *retx, short *rety)
/*
 * This procedure calculates coordinates for a line. No line is actually
 *  drawn by this procedure, though.
 *
 *   params : heading  == 0 - 360 degree direction line goes.
 *            distance == total space line should cover.
 *            startx   == Starting x coordinate for line.
 *            starty   == Starting y coordinate for line.
 *            *retx    == (Return value) ending x coordinate.
 *            *rety    == (Return value) ending y coordinate.
 *   returns : officially, void. however, retx and rety are modified.
 */
{

    short calcx;
    short calcy;
    double rad;

    heading -= 90;       /* Make 0 degrees point north. */
    
    rad = (heading / R_TO_D);          /* Radian conversion. */
    calcx = (cos(rad) * distance);
    calcy = (sin(rad) * distance);
    
    *retx = startx + calcx;   /* return values. */
    *rety = starty + calcy;

} /*calcline*/

