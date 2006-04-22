/**************************************************************
 * Mikey's LOGO interpreter. (TOBY)                           *
 *  Written by Ryan C. Gordon for his Cultural Perspectives   *
 *  senior exit project in 1995 at Central Bucks High School  *
 *  East.                                                     *
 **************************************************************
 * This program was compiled with Microsoft C 7.0.            *
 **************************************************************
 * Compile with this command line:                            *
 *                                                            *
 * cl /AL toby.c tobydata.c tobygrfx.c /link graphics         *
 **************************************************************
 * COPYRIGHT NOTICE:                                          *
 *                                                            *
 * This work, in both binary executable form, and source      *
 *  code text, is copyright (c) Ryan C. Gordon, 1995. While   *
 *  this program may be freely modified, no amount of         *
 *  modification voids this copyright.                        *
 **************************************************************/

/* Included headers... */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graph.h>
#include <string.h>
#include <memory.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>
#include "toby.h"


/* Global Variables... */
linkedlist *procedures = NULL;  /* user-defined LOGO procedures.         */
linkedlist *globals = NULL;     /* list of global LOGO variables.        */
errortype runtime = RT_NOERROR; /* Runtime status.                       */
boolean shouldsave = FALSE;     /* Should we prompt to save procedures?  */

/* move these? */
char graphic_flags;            /* Bitmap of graphic-related flags.  */
short turtle_x, turtle_y;      /* The Turtle's X and Y coordinates. */
short pen_color;               /* Current pen color.                */
short turtle_heading = 0;      /* the Turtle's heading : 0-360 deg. */

void main(int argc, char *argv[])
/*
 * Short, but sweet; it's the mainline.
 */
{
    logoinit(argv[1]);
    logostart();
    logofinish(NULL, ERLV_GOOD);
} /*main*/


void logoinit(char *sourcefile)
/*
 * This function initializes everything, from screen to memory.
 *   params : *sourcefile == filename of LOGO source to load. NULL for none.
 *  returns : void.
 *     also : Screen mode and global variables may change.
 */
{
    fprintf(stderr, "TOBY v%s\r\nLoading...\r\n", VERSION);
    srand((unsigned) time(NULL));
    init_crt();
    codeload(sourcefile);
    _outtext("Welcome to TOBY.\n");
} /*logoinit*/



void logostart(void)
/*
 * This is an entry point to logo_run()...Input is taken, and if it's
 *   not a procedure definition, puts it into '__main__', and calls 
 *   logo_run().
 *
 *   params : void.
 *  returns : void.
 */
{

    char instruction[MAXCOMMAND];
    char *retval;
    linkedlist *mainproc = addlist(NULL, "__main__", "", "");

    while (runtime != RT_QUIT)
    {

        runtime = RT_NOERROR;     /* reset runtime error variable. */

        getline(NULL, instruction, "? ", MAXCOMMAND);

        if (strnicmp(instruction, "to ", 3) == 0)
            define_proc(NULL, instruction + 3);
        else
        {
            mainproc->data = setstring(mainproc->data, instruction);
            retval = logo_run(mainproc, "", NULL);
            if (retval != NULL)
            {
                free(retval);
                logo_error("Return value without a function.", RT_RETWOFUNCT);
            } /*if*/

        } /*else*/

    } /*while*/

    free(globals);
    freelist(mainproc);

} /*logostart*/



void logofinish(char *errormsg, int erlv)
/*
 * De-init everything (such as video mode) back to what is was before
 *  LOGO changed it.
 *
 *   params : *errormsg == string to print to stderr upon exit.
 *                 erlv == Errorlevel to return to operating system.
 *  returns : Never returns.
 *     also : screen mode changes. Allocated memory altered.
 */
{

    codesave(NULL);
    freelist(procedures);

    deinit_crt();

    if (errormsg == NULL)
        fprintf(stderr, "Thank you for using TOBY %s!\r\n%s\r\n",
                VERSION, COPYRIGHT);
    else
        fprintf(stderr, "%s\r\n", errormsg);

    exit(erlv);   /* program terminates, no return from procedure. */

} /*logofinish*/



void logo_error(char *errormessage, errortype errnum)
/*
 * Outputs an error message, corresponding to errnum.
 *
 *   params : errnum        == errornumber to print.
 *            *errormessage == string information to print.
 *  returns : void.
 *     also : swaps video pages and sets global variable 'runtime'.
 */
{

    runtime = errnum;

    _outtext(errormessage);
    _outchar('\n');

} /*logo_error*/



char *logo_run(linkedlist *proc, char *params, linkedlist *lastscalars)
{
    int count;
    char *instructions;
    linkedlist *scalars;
    linkedlist *listwork;
    char *work;
    char *returnval;

    if (stackavail() < 100)
        logo_error("Out of stack space.", RT_STACKOUT);
    
    if (runtime)                /* Has an error occured? */
        return(NULL);

    /* --- Parse arguments, and fill (linkedlist *) scalars with them --- */

    for (count = 0, work = proc->params; *work != '\0'; count++)
        work = findword(work + 1);

    if (count)
    {    
        work = parse_args(params, lastscalars, count);
        scalars = add_args(proc, work);
        free(work);    
    } /*if*/
    
    else
        scalars = NULL;
    
    /* ------------------------------------------------------------------ */
    
    instructions = setstring(NULL, proc->data);
    preprocess(instructions);
    instructions = allocate(instructions, strlen(instructions) + 2);
    instructions[strlen(instructions) + 1] = '\0';

    while ((*instructions != '\0') && (!runtime))
    {
        /******************************************************************/
        /* Note we search user-defined procedures first. This allows the  */
        /* user to 'overload' LOGO primitives, in effect redefining their */
        /* function. However, a few cannot be overloaded.                 */
        /******************************************************************/
        
        if (strnicmp(instructions, "OUTPUT", 6) == 0)
        {
            remove_tokens(instructions, 1);
            returnval = parse_args(instructions, scalars, 1);
            free(instructions);
            freelist(scalars);
            return(returnval);
        } /*if*/
        
        work = findword(instructions);        

        if (*work == '\0')
            listwork = searchlist(procedures, instructions);
        else
        {
            *work = '\0';
            listwork = searchlist(procedures, instructions);
            *work = ' ';
        } /*else*/
        
        if (listwork != NULL)   /* Is there a procedure to call? */
        {    
            remove_tokens(instructions, 1);
            ltrim(instructions);
            returnval = logo_run(listwork, instructions, scalars); 
        } /*if*/    
        
        else   /* NOT a user-defined procedure? */
            returnval = primitives(instructions, &scalars);
        
        if (returnval != NULL)    /* If there was a return value... */
        {
            /* Put return values at start of instructions... */

            instructions = allocate(instructions, strlen(instructions) +
                    strlen(returnval) + 1);

            insert_string(instructions, returnval);
            free(returnval);
            returnval = NULL;
        } /*if*/

    } /*while*/

    free(instructions);
    freelist(scalars);
    return(NULL);

} /*logo_run*/



linkedlist *add_args(linkedlist *proc, char *arguments)
/*
 *  Set up a linked list for a user-defined procedure's parameters.
 *    The params must already be 'parsed' before calling this, so it's
 *    just a matter of plugging them in.
 *
 *   params : *proc      == user-defined procedure we're setting up params for.
 *            *arguments == string of parsed arguments.
 *  returns : pointer to linked list of LOGO variables.
 */
{

    char *wordptr;                  /* Work pointer for proc->params   */
    char *nextword;                 /* next word in proc->params.      */
    linkedlist *returnval = NULL;   /* Return value.                   */
    linkedlist *current = NULL;     /* Work variable for a linkedlist. */
    int len;                        /* Work variable for a LOGO list.  */

    if (runtime)
        return(NULL);

    for (wordptr = proc->params; *wordptr != '\0'; wordptr = nextword)
    {
        nextword = findword(wordptr + 1);

        if (*nextword == '\0')
        {
            if (returnval != NULL)
                current = addlist(returnval, wordptr, "", "");
            else
                current = returnval = addlist(NULL, wordptr, "", "");
        } /*if*/

        else
        {
            *nextword = '\0';
            if (returnval != NULL)
                current = addlist(returnval, wordptr, "", "");
            else
                current = returnval = addlist(NULL, wordptr, "", "");

            *nextword = ' ';
        } /*else*/

        if (*arguments == '\0')
        {
            logo_error("Invalid number of parameters.", RT_BADPARAM);
            freelist(returnval);
            return(NULL);
        } /*else if*/

        else if (*arguments == '[')  /* a list? */
        {
            len = list_length(arguments);
            if (len == -1)
            {
                logo_error("Invalid parameter.", RT_BADPARAM);
                freelist(returnval);
                return(NULL);
            } /*if*/

        } /*else if*/

        else
            len = findword(arguments) - arguments;

        current->data = allocate(current->data, len + 1);

        strncpy(current->data, arguments, len);
        current->data[len] = '\0';

        arguments += len;

        if (*arguments != '\0')
            arguments++;

        if (*nextword == ' ')
            nextword++;

    } /*for*/

    return(returnval);

} /*add_args*/



char *primitives(char *instructions, linkedlist **scalars)
/*
 * This function handles LOGO 'primitives', that is, built-in procedures.
 *
 *   params : *instructions == string to check against primitives.
 *            **scalars     == local variables available to functions.
 *  returns : pointer to allocated return value. NULL if none.
 */
{

    char *returns = NULL;            /* parse parameters of a primitive */
    char *parameters;                /* ptr to start of parameters.     */
    char *returnval = NULL;          /* Actual primitives() return val. */
    linkedlist *listwork;            /* Dummy var for lists.            */
    char workspace[MAXCOMMAND * 2];  /* General workspace.              */
    int looper;

    if (stackavail() < 100)
        logo_error("Out of stack space.", RT_STACKOUT);
    
    if (runtime)
        return(NULL);
    
    parameters = findword(instructions);   /* get past command to params. */
    if (*parameters != '\0')
    {    
        *parameters = '\0';   /* put end-of-string on instruction. */
        parameters++;
    } /*if*/
    
    /* Actual primitives start here... */
    if (strcmp(instructions, "REPEAT") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        strcpy(workspace, returns);
        free(returns);
        
        returnval = parse_args(parameters, *scalars, 1);
        returns = NULL;

        for (looper = 1; looper <= atoi(workspace); looper++)
        {    
            returns = setstring(returns, returnval);        
            returns = parse_args(returns, *scalars, -10);
        } /*for*/
        
        free(returnval);
        returnval = NULL;
    
    } /*if (REPEAT)*/
    
    else if (twostrcmp(instructions, "FORWARD", "FD"))
    {
        returns = parse_args(parameters, *scalars, 1);
        moveturtle(atoi(returns));
    } /*else if (FORWARD)*/

    else if (twostrcmp(instructions, "BACK", "BK"))
    {
        returns = parse_args(parameters, *scalars, 1);
        moveturtle(-atoi(returns));
    } /*else if (BACK)*/
    
    else if (twostrcmp(instructions, "RIGHT", "RT"))
    {
        returns = parse_args(parameters, *scalars, 1);
        turtleturn(atoi(returns));
    } /*else if (RIGHT)*/

    else if (twostrcmp(instructions, "LEFT", "LT"))
    {
        returns = parse_args(parameters, *scalars, 1);
        turtleturn(-atoi(returns));
    } /*else if (LEFT)*/

    else if (strcmp(instructions, "PRINT") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        _outtext(returns);
        _outchar('\n');
    } /*else if (PRINT)*/

    else if (strcmp(instructions, "SETPOS") == 0)
    {
        returns = parse_args(parameters, *scalars, 2);
        restoreimage(turtle_x, turtle_y);
        turtle_x = atoi(returns);
        turtle_y = atoi(findword(returns) + 1);
        drawturtle(turtle_x, turtle_y);
    } /*else if (SETPOS)*/
    
    else if (strcmp(instructions, "SETX") == 0)
    {   
        returns = parse_args(parameters, *scalars, 1);
        restoreimage(turtle_x, turtle_y);
        turtle_x = atoi(returns);
        drawturtle(turtle_x, turtle_y);
    } /*else if (SETX)*/
    
    else if (strcmp(instructions, "SETY") == 0)
    {   
        returns = parse_args(parameters, *scalars, 1);
        restoreimage(turtle_x, turtle_y);
        turtle_y = atoi(returns);
        drawturtle(turtle_x, turtle_y);
    } /*else if (SETY)*/

    else if (strcmp(instructions, "DOT") == 0)
    {
        returns = parse_args(parameters, *scalars, 2);
        restoreimage(turtle_x, turtle_y);
        _setpixel(atoi(returns), atoi(findword(returns) + 1));
        drawturtle(turtle_x, turtle_y);
    } /*else if (DOT)*/
    
    else if (twostrcmp(instructions, "SETHEADING", "SETH"))
    {
        returns = parse_args(parameters, *scalars, 1);
        restoreimage(turtle_x, turtle_y);
        
        turtle_heading = atoi(returns);
        
        while (turtle_heading < 0)
            turtle_heading += 360;
        
        while (turtle_heading >= 360)
            turtle_heading -= 360;
    
        drawturtle(turtle_x, turtle_y);

    } /*else if (SETHEADING)*/
    
    else if (strcmp(instructions, "HEADING") == 0)
    {    
        returnval = allocate(NULL, 5);
        returnval = itoa(turtle_heading, returnval, 10);
    } /*else if (HEADING)*/

    else if (strcmp(instructions, "POS") == 0)
    {
        returnval = allocate(NULL, 30);
        sprintf(returnval, "[%d %d]", turtle_x, turtle_y);
    } /*else if (POS)*/

    else if (strcmp(instructions, "XCOR") == 0)
    {
        returnval = allocate(NULL, 10);
        returnval = itoa(turtle_x, returnval, 10);
    } /*else if (XCOR)*/
    
    else if (strcmp(instructions, "YCOR") == 0)
    {
        returnval = allocate(NULL, 10);
        returnval = itoa(turtle_y, returnval, 10);
    } /*else if (YCOR)*/

    else if (strcmp(instructions, "SETPC") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        if (isdigit(*returns))
        {    
            pen_color = atoi(returns);
            _setcolor(pen_color);
        } /*if*/
        
       /* else  !!!
            logo_error("  */
    } /*else if (SETPC)*/
    
    else if (twostrcmp(instructions, "PENCOLOR", "PC"))
    {
        returnval = allocate(NULL, 5);
        itoa(pen_color, returnval, 10);
    } /*else if (PENCOLOR)*/
    
    else if (strcmp(instructions, "ADDP") == 0)
    {
        returns = parse_args(parameters, *scalars, 2);
        returnval = allocate(NULL, 10);
        if (strchr(returns, '.') != NULL)   /* floating point? */
            gcvt(atof(returns) + atof(findword(returns) + 1), 5, returnval);
        else
            itoa(atoi(returns) + atoi(findword(returns) + 1), returnval, 10);
    
    } /*else if (ADDP)*/
    
    else if (strcmp(instructions, "SUBP") == 0)
    {
        returns = parse_args(parameters, *scalars, 2);
        returnval = allocate(NULL, 10);
        if (strchr(returns, '.') != NULL)   /* floating point? */
            gcvt(atof(returns) - atof(findword(returns) + 1), 5, returnval);
        else
            itoa(atoi(returns) - atoi(findword(returns) + 1), returnval, 10);
    
    } /*else if (SUBP)*/
    
    else if (strcmp(instructions, "SQRT") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        returnval = allocate(NULL, 15);
        sprintf(returnval, "%lf", sqrt(atoi(returns)));
    } /*else if (SQRT)*/
    
    else if (strcmp(instructions, "ROUND") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        returnval = allocate(NULL, 15);
        looper = atof(returns);                /* Truncate... */
        if (fmod(atof(returns), 1.0) >= 0.5)   
            looper++;                          /* Round up. */
        returnval = itoa(looper, returnval, 10);
        strcat(returnval, ".0");
    } /*else if (ROUND)*/
    
    else if (strcmp(instructions, "INT") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        returnval = allocate(NULL, 15);
        sprintf(returnval, "%d", (int) atof(returns));
    } /*else if (INT)*/

    /*else if (strcmp(instructions, "QUOTIENT") == 0)
    {
        returns = parse_args(parameters, *scalars, 2);
        looper!!!
    } /*else if (QUOTIENT)*/
    
    else if (strcmp(instructions, "REMAINDER") == 0)
    {
    } /*else if (REMAINDER)*/

    else if (strcmp(instructions, "RANDOM") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        if (isdigit(*returns))
        {
            returnval = allocate(NULL, 10);
            returnval = itoa((rand() % atoi(returns) - 1) , returnval, 10);
        } /*if*/
    } /*else if*/

    else if (twostrcmp(instructions, "PENUP", "PU"))
    {
        graphic_flags = (graphic_flags | PEN_UP);
        graphic_flags = (graphic_flags & !PEN_ERASE);
        graphic_flags = (graphic_flags & !PEN_REVERSE);
    } /*else if (PENUP)*/
    
    else if (twostrcmp(instructions, "PENDOWN", "PD"))
    {
        graphic_flags = (graphic_flags & !PEN_UP);
        graphic_flags = (graphic_flags & !PEN_ERASE);
        graphic_flags = (graphic_flags & !PEN_REVERSE);
    } /*else if (PENUP)*/
    
    else if (twostrcmp(instructions, "PENERASE", "PE"))
    {
        graphic_flags = (graphic_flags & !PEN_UP);
        graphic_flags = (graphic_flags | PEN_ERASE);
        graphic_flags = (graphic_flags & !PEN_REVERSE);
    } /*else if (PENUP)*/
    
    else if (twostrcmp(instructions, "PENREVERSE", "PX"))
    {
        /*
        graphic_flags = (graphic_flags & !PEN_UP);
        graphic_flags = (graphic_flags & !PEN_ERASE);
        graphic_flags = (graphic_flags | !PEN_REVERSE);
        */
        logo_error("PENREVERSE not yet supported.", RT_NOSUPPORT);
    } /*else if (PENUP)*/
    
    else if (strcmp(instructions, "HOME") == 0)
        home();

    else if (twostrcmp(instructions, "CLEARSCREEN", "CS"))
        resetscreen();

    else if (strcmp(instructions, "CLEAN") == 0)
        clean();
    
    else if (strcmp(instructions, "MAKE") == 0)
    {
        returns = parse_args(findword(parameters), *scalars, 1);
        *findword(parameters) = '\0';
        if (*parameters != '\"')
            logo_error("Invalid identifier.", RT_BADNAME);
        else if (!valid_ident(parameters + 1))
            logo_error("Invalid identifier.", RT_BADNAME);
        else
        {
            *parameters = ':';
            listwork = searchlist(*scalars, parameters);
            if (listwork == NULL)
            {
                /* Globals get checked 2nd, in case of duplicate names... */
                
                listwork = searchlist(globals, parameters);
                if (listwork == NULL)
                {    
                    if (globals == NULL)
                        listwork = globals = addlist(NULL, parameters, "", "");
                    else
                        listwork = addlist(globals, parameters, "", "");
                } /*if*/
            } /*if*/

            else
            {
                if (*scalars == NULL)
                    listwork = *scalars = addlist(NULL, parameters, "", "");
                else
                    listwork = addlist(*scalars, parameters, "", "");
            } /*else*/

            if (listwork != NULL)
                listwork->data = setstring(listwork->data, returns);

            remove_tokens(parameters, 1);        

        } /*else*/

    } /*else if (MAKE)*/

    else if (strcmp(instructions, "LOCAL") == 0)
    {
        if (*parameters != '\"')
            logo_error("Invalid identifier.", RT_BADNAME);
        else if (!valid_ident(returns + 1))
            logo_error("Invalid identifier.", RT_BADNAME);
        else
        {
            strcpy(workspace, returns);
            listwork = searchlist(*scalars, workspace);
            if (listwork != NULL)
                logo_error("Duplicate LOCAL variable.", RT_BADNAME);
            else
            {    
                if (*scalars == NULL)
                    *scalars = addlist(NULL, workspace, "", "");
                else
                    addlist(*scalars, workspace, "", "");
            } /*else*/

            remove_tokens(parameters, 1);
        
        } /*else*/

    } /*else if (LOCAL)*/

    else if (twostrcmp(instructions, "HIDETURTLE", "HT"))
    {
        graphic_flags = (graphic_flags | HIDETURTLE);
        restoreimage(turtle_x, turtle_y);
    } /*else if (HIDETURTLE)*/

    else if (twostrcmp(instructions, "SHOWTURTLE", "ST"))
    {
        graphic_flags = (graphic_flags & !HIDETURTLE);
        saveimage(turtle_x, turtle_y);
        drawturtle(turtle_x, turtle_y);
    } /*else if (SHOWTURTLE)*/

    else if (strcmp(instructions, "FENCE") == 0)
        graphic_flags = (graphic_flags & FENCE);

    /* WRAP and WINDOW? */
    
    else if (stricmp(instructions, "SAVE") == 0)
    {    
        returns = parse_args(parameters, *scalars, 1);
        if (!shouldsave)
            _outtext("No procedures need saving.\n");
        else if (*returns != '\"')
            codesave(returns);
        else
            codesave(returns + 1);
    } /*else if (SAVE)*/
    
    else if (stricmp(instructions, "LOAD") == 0)
    {    
        returns = parse_args(parameters, *scalars, 1);
        if (*returns != '\"')
            codeload(returns);
        else        
            codeload(returns + 1);
    } /*else if (LOAD)*/

    else if (stricmp(instructions, "POTS") == 0)
        pots(procedures);
    
    else if (strcmp(instructions, "GLOBALS") == 0)
        pots(globals);

    else if (strcmp(instructions, "ERASE") == 0)
    {
        returns = parse_args(parameters, *scalars, 1);
        if (*returns == '\"')    
            memmove(returns, returns + 1, strlen(returns));

        listwork = searchlist(procedures, returns);
        if (listwork != NULL)
        {
            _outtext("ERASE procedure \"");
            _outtext(listwork->name);
            _outchar('\"');
            if (getyn("?"))
            {    
                if (removelist(procedures, returns))
                    _outtext("Erased.\n");
                else
                    logo_error("Could not erase!\n", RT_NOERASE);
            } /*if*/
        } /*if*/

        else
        {    
            _outchar('\"');
            _outtext(returns);
            _outtext("\" -- ");
            logo_error("No such procedure.", RT_BADNAME);    
        } /*else*/
    
    } /*else if (ERASE)*/
    
    else if (strcmp(instructions, "QUIT") == 0)
        runtime = RT_QUIT;
    
    else   /* Unknown command? */
    {
        sprintf(workspace, "%s%s.", CONFUSED, instructions);
        logo_error(workspace, RT_UNKNOWN);
    } /*else*/

    /* Get rid of primitive token that was called. */
    *findword(instructions) = ' ';
    remove_tokens(instructions, 1);
    ltrim(instructions);

    if (returns != NULL)
        free(returns);

    return(returnval);

} /*primitives*/


char *parse_args(char *arguments, linkedlist *variables, int argc)
/*
 *  Parse arguments, in case of functions, variables, lists, or literals.
 *
 *  params : *arguments == start of arguments for function.
 *           *variables == available local scalars for use as arguments.
 *           argc       == amount of parameters to parse. if < 0, then
 *                         everything in the list is parsed. (for RUN and
 *                         such).
 * returns : pointer to start of allocated space with parsed arguments.
 */
{

    int looper;
    char *returnval;
    char *work;
    char *storage;
    linkedlist *listwork;
    
    if (stackavail() < 100)
        logo_error("Out of stack space.", RT_STACKOUT);
    
    if (runtime)
        return(NULL);
    
    if (*arguments == ' ')
        arguments++;

    if ((argc > 1) || (argc < 0))  /* Variable arguments or arguments supplied in list? */
    {
        if ((*arguments != '[') || (list_length(arguments) == -1))
            logo_error("Invalid argument.", RT_BADPARAM);
        else
        {    
            work = allocate(NULL, list_length(arguments));
            strncpy(work, arguments + 1, list_length(arguments) - 1);
            work[list_length(arguments) - 2] = '\0';
        } /*else*/
    
    } /*else if*/
        
    else
        work = setstring(NULL, arguments);

    remove_tokens(arguments, 1);
    arguments = work;
    

    if (argc < 0)             /* band-aid for loop control. */
        looper = argc - 1;
    else
        looper = 1;

    returnval = setstring(NULL, "");  /* Initialize string. */
    
    
    for (; looper <= argc; looper++)
    {
        if (runtime)
        {
            free(returnval);
            free(arguments);
            return(NULL);
        } /*if*/
        
        switch (*arguments)
        {
            /* case '(': ...and other math... */
            
            case '\0':     /* end of string? */
                if (argc >= 0)
                {
                    logo_error("Invalid number of arguments.", RT_BADPARAM);
                    work = arguments;
                    storage = NULL;
                } /*if*/
                break;
                
            case ']':      /* end of a list? */
                logo_error("Invalid number of arguments.", RT_BADPARAM);
            
            case '[':      /* a list? */
                if (list_length(arguments) == -1)
                    logo_error("Invalid argument.", RT_BADLIST);
                else
                {    
                    storage = allocate(NULL, list_length(arguments) + 1);
                    strncpy(storage, arguments, list_length(arguments));
                    storage[list_length(arguments)] = '\0';
                } /*else*/
                break;
                
            case ':':      /* a variable? */
                work = findword(arguments);
                storage = allocate(NULL, work - arguments + 1);
                
                if (*work == '\0')
                    strcpy(storage, arguments);
                else
                {
                    *work = '\0';
                    strcpy(storage, arguments);
                    *work = ' ';
                } /*else*/
                
                listwork = searchlist(variables, storage);
                if (listwork == NULL)   /* Not local variable? */
                {
                    listwork = searchlist(globals, storage);
                    if (listwork == NULL)
                        logo_error("Unknown variable.", RT_BADNAME);
                } /*if*/
                
                if (!runtime)
                {    
                    storage = allocate(storage, strlen(listwork->data) + 1);
                    strcpy(storage, listwork->data);
                } /*if*/
                break;

            case '\"':     /* a literal? */
                storage = allocate(NULL, findword(arguments) - arguments + 1);
                strncpy(storage, arguments, findword(arguments) - arguments);
                storage[findword(arguments) - arguments] = '\0';
                break;

            default:       /* number or potential procedure? */
                work = findword(arguments);
                
                if (atoi(arguments))
                {
                    if (*work == '\0')
                        storage = setstring(NULL, arguments);
                    else
                    {
                        *work = '\0';
                        storage = setstring(NULL, arguments);
                        *work = ' ';
                    } /*else*/
                } /*if*/
                
                else
                {
                    if (*work == '\0')
                        listwork = searchlist(procedures, arguments);
                    else
                    {
                        *work = '\0';
                        listwork = searchlist(procedures, arguments);
                        *work = ' ';
                    } /*else*/
                
                    if (listwork != NULL)       /* user-defined procedure? */
                        work = logo_run(listwork, work, variables);
                    else                                    /* Primitive? */
                    {    
                        work = primitives(arguments, &variables);
                        arguments = allocate(arguments, strlen(arguments) + 4);
                        insert_string(arguments, "\"X "); /* dummy token. */
                    } /*else*/

                    if (work != NULL)
                    {    
                        storage = setstring(NULL, work);
                        free(work);
                    } /*if*/
                    else
                        storage = NULL;
                    
                } /*else*/
                break;
        
        } /*switch*/

        if (*arguments != '\0')
        {    
            remove_tokens(arguments, 1);
            ltrim(arguments);
            if (argc < 0)
                looper--;
        } /*if*/
        
        if (storage != NULL)
        {    
            returnval = allocate(returnval, strlen(returnval) + 
                    strlen(storage) + 2);
        
            strcat(returnval, storage);
            strcat(returnval, " ");
            free(storage);
        } /*if*/

    } /*for*/

    free(arguments);
    rtrim(returnval);
    return(returnval);

} /*parse_args*/

