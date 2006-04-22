#ifndef _INCL_UNIXBITS_H_
#define _INCL_UNIXBITS_H_

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

/* compatibility things from Microsoft C's runtime. */

#define stricmp(x,y) strcasecmp(x,y)
#define strnicmp(x,y,z) strncasecmp(x,y,z)
#define stackavail() 1024000
#define _HEAPOK 0
#define _heapchk() 0

#define _GCURSOROFF 0
#define _displaycursor(x)

#define _DEFAULTMODE 0
#define _MAXRESMODE  1

#define _GCLEARSCREEN 0
#define _GVIEWPORT    1

#define _GPSET 0

char *itoa(int val, char *buf, int radix);
void _outtext(char *text);
short _setcolor(short color);
short _getcolor(void);
void _setpixel(short x, short y);
short _setvideomode(short mode);
char getch(void);
void _clearscreen(short target);
void _getimage(short x1, short y1, short x2, short y2, char *image);
void _putimage(short x, short y, char *image, short action);
void _settextwindow(short r1, short c1, short r2, short c2);
int _imagesize(int x1, int y1, int x2, int y2);

struct xycoord
{
    short xcoord;
    short ycoord;
};

struct xycoord _moveto(short x, short y);
short _lineto(short x, short y);

struct rccoord
{
    short row;
    short col;
};

struct rccoord _gettextposition(void);
struct rccoord _settextposition(short row, short column);


struct videoconfig
{
    short numxpixels; /* pixels on X axis */
    short numypixels; /* pixels on Y axis */
    short numtextcols; /* text columns available */
    short numtextrows; /* text rows available */

    /*
    short numcolors;	  	- actual colors available
    short bitsperpixel;  	- bits per pixel
    short numvideopages; 	- available video page count
    short mode;		  	- current video mode
    short adapter;	  	- active display adapter
    short monitor;	  	- active display monitor
    short memory;	  	- adapter video memory in K
    */
};

struct videoconfig *_getvideoconfig(struct videoconfig *config);

#endif

