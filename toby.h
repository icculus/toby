/******************************************************
 * TOBY header file.                                  *
 ******************************************************
 * Copyright (c) Lighting and Sound Technologies 1995 *
 * See copyright info in TOBY.C.                      *
 ******************************************************/

/* Errorlevels. */
#define ERLV_BAD   255
#define ERLV_GOOD  0

/* Maximum length one line of input may be. */
#define MAXCOMMAND 75

/* Boolean TRUE and FALSE. */
#define TRUE          1
#define FALSE         0

/* Version number. */
#define VERSION       "0.10beta"
#define COPYRIGHT     "Copyright (c) Ryan C. Gordon, 1995"

/* error for unknown procedure. */
#define CONFUSED      "I don't know how to "

/* angles to radians conversion. */
#define R_TO_D        57.29578

/* Global LOGO runtime errors. */
#define RT_NOERROR    0
#define RT_QUIT       1
#define RT_STACKERR   2
#define RT_PASTFENCE  3
#define RT_INVALID    4
#define RT_UNKNOWN    5
#define RT_OUTOFMEM   6
#define RT_RETWOFUNCT 7
#define RT_BADPARAM   8
#define RT_BADNAME    9
#define RT_BADLIST    10
#define RT_BADDECLARE 11
#define RT_STACKOUT   12
#define RT_NOSUPPORT  13
#define RT_NOERASE    14

/* Constants for graphic_flags */
#define FENCE         1
#define HIDETURTLE    2
#define PEN_ERASE     4
#define PEN_REVERSE   8
#define PEN_UP        16

/* Size of turtle : size of screen ratio. */
/* Decrease number to enlarge Turtle,     */
/* and increase to make Turtle smaller.   */
#define TURTLERATIO   40

/* Characters of note. */
#define ASCII_ESC     27

typedef char boolean;      /* Either TRUE or FALSE.      */
typedef char errortype;    /* Storage for error numbers. */

struct _linkedlist            /* Linked list structure of LOGO proc/scalars */
{
    char *name;               /* Name of data.                              */
    char *data;               /* Pointer to actual data.                    */
    char *params;             /* Parameters (for procedure lists.)          */
    struct _linkedlist *next; /* pointer to next data in list.              */
                              /*   (NULL if last one.)                      */
}; /*linkedlist*/

typedef struct _linkedlist linkedlist;


/* Function/Procedure declarations. */
/* TOBY.C */
void logoinit(char *sourcefile);
void logostart(void);
void logofinish(char *errormsg, int erlv);
void logo_error(char *errormessage, errortype errnum);
char *logo_run(linkedlist *proc, char *params, linkedlist *lastscalars);
linkedlist *add_args(linkedlist *proc, char *params);
char *primitives(char *instructions, linkedlist **scalars);
char *parse_args(char *arguments, linkedlist *variables, int argc);


/* TOBYDATA.C */
linkedlist *freelist(linkedlist *list);
linkedlist *searchlist(linkedlist *list, char *item);
boolean removelist(linkedlist *list, char *item);
char *setstring(char *puthere, char *newstring);
char *alltrim(char *str);
char *ltrim(char *str);
char *rtrim(char *str);
boolean twostrcmp(char *cmpthis, char *checkone, char *checktwo);
void preprocess(char *unprocessed);
char charinput(FILE *charstream);
char *getline(FILE *fromhere, char *tohere, char *prompt, int maxline);
boolean getyn(char *prompt);
boolean define_proc(FILE *fromhere, char *declaration);
void codeload(char *filename);
boolean codesave(char *fname);
char *findword(char *startword);
int list_length(char *list);
void *allocate(void *basemem, unsigned int heapsize);
void insert_string(char *inserthere, char *str2insert);
int remove_tokens(char *fromhere, int howmany);
void pots(linkedlist *potsthis);
boolean valid_ident(char *declaredname);
linkedlist *addlist(linkedlist *list, char *newitem, char *newdata, 
        char *newparams);


/* TOBYGRFX.C */
void init_crt(void);
void deinit_crt(void);
void saveimage(int x, int y);
void restoreimage(int x, int y);
void home(void);
void resetscreen(void);
void clean(void);
void _outchar(char outthis);
void drawturtle(short x, short y);
void moveturtle(short distance);
void turtleturn(short rotation);
void calcline(short heading, short distance, short startx, short starty, 
                short *retx, short *rety);

/* end of toby.h */

