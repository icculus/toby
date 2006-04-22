/**************************************** 
 * Data manipulation routines for TOBY. *
 ****************************************
 * Copyright (c) Ryan C. Gordon, 1995   *
 * See copyright info in TOBY.C.        *
 ****************************************/

#include <stdio.h>
#include <string.h>
#include "toby.h"

#ifdef __GNUC__
#include "unixbits.h"
#else
#include <graph.h>
#include <memory.h>
#include <malloc.h>
/* (this is probably a leftover from EMX...) */
#define access(x,y) (-1)
#endif

extern boolean shouldsave;
extern linkedlist *procedures;


void *allocate(void *basemem, unsigned int heapsize)
/*
 * This is a simplified way to allocate memory, and collect garbage. That
 *  is, if we can't allocate memory, we gracefully exit();.
 *
 *  params : *basemem == Location of previously allocated block. NULL for
 *                         a new one.
 *           heapsize == amount of memory, in bytes, to allocate.
 *           *func    == string of function name called from, in case of
 *                        error.
 * returns : heapsize == Amount of memory, in bytes, to allocate.
 */
{
    void *newpointer;
    
    if (_heapchk() != _HEAPOK)
    {   
        _setvideomode(_DEFAULTMODE);
        exit(ERLV_BAD);
    } /*if*/

    newpointer = realloc(basemem, heapsize);
    if (newpointer == NULL)
    {    
        _setvideomode(_DEFAULTMODE);
        printf("Memory allocation error.\n");
        exit(ERLV_BAD);
    } /*if*/
    
    return(newpointer);

} /*allocate*/



linkedlist *freelist(linkedlist *list)
/*
 * Free all memory used by a linked list, thereby removing it.
 *   params : *list == pointer to first element in linked list
 *                      to remove.
 *  returns : always returns NULL.
 *     also : Allocated memory is altered.
 */
{

    linkedlist *currentlist = list;
    linkedlist *nextitem;

    while (currentlist != NULL)
    {
        nextitem = currentlist->next;
        free(currentlist->name);
        free(currentlist->data);
        free(currentlist->params);
        free(currentlist);
        currentlist = nextitem;
    } /*while*/

    return(NULL);

} /*freelist*/



linkedlist *searchlist(linkedlist *list, char *item)
/*
 * Searches a specified linked list for a case-insensitive name.
 *
 *   params : *list == start of list to search.
 *            *item == string of name to find.
 *  returns : pointer to item in list containing name. NULL if not found.
 */
{

    linkedlist *currentitem = list;

    while ((currentitem != NULL) && (stricmp(currentitem->name, item) != 0))
        currentitem = currentitem->next;  /* jump to next item in list. */

    return(currentitem);

} /*searchlist*/


boolean removelist(linkedlist *list, char *item)
/*
 * Removes one item from a linked list, freeing that item's memory and
 *  re-aligning the list's pointers.
 *
 *   params : *list == start of list to remove item from.
 *            *item == string of name of item to remove.
 *  returns : boolean TRUE if removed successfully, boolean FALSE if not.
 *     also : allocated memory altered.
 */
{
    linkedlist *currentitem = list;
    linkedlist *lastitem = NULL;

    while ((currentitem != NULL) && (stricmp(item, currentitem->name) != 0))
    {
        lastitem = currentitem;
        currentitem = currentitem->next;
    } /*while*/

    if (currentitem == NULL)   /* Name not in list? */
        return(FALSE);

    if (lastitem == NULL)  /* First item in list is to be removed? */
    {
        memcpy(currentitem, currentitem->next, sizeof (linkedlist));
        lastitem = list;
        currentitem = list->next;
    } /*if*/

    lastitem->next = currentitem->next;   /* Realign pointers. */

    free(currentitem->name);
    free(currentitem->data);
    free(currentitem->params);
    free(currentitem);

    return(TRUE);

} /*removelist*/



linkedlist *addlist(linkedlist *list, 
                    char *newitem, 
                    char *newdata, 
                    char *newparams)
/*
 * This procedure adds an item to a linked list.
 *
 *   params : *list == linked list to append to. NULL to create a new one.
 *            *newitem == data to put in (char *name) field.
 *            *newdata == data to put in (char *data) field.
 *            *newparams == data to put in (char *params) field.
 *  returns : pointer to new item or list.
 *     also : Changes memory allocation.
 */
{
    linkedlist *current = list;
    linkedlist *last = NULL;

    while (current != NULL)
    {
        last = current;
        current = current->next;
    } /*while*/

    current = (linkedlist *) allocate(NULL, sizeof (linkedlist));

    current->name = setstring(NULL, newitem);
    current->data = setstring(NULL, newdata);
    current->params = setstring(NULL, newparams);
    
    if (last != NULL)   /* Not a new list? */
        last->next = current;

    current->next = NULL;  /* mark new item as end of linked list. */

    return(current);

} /*additem*/



char *setstring(char *puthere, char *newstring)
/*
 * Allocates exact memory for a new (or modified) string.
 *
 *   params : *puthere == address of previous string. NULL to create new one.
 *            *newstring == string to store.
 *  returns : pointer to location of newly set string. If error, won't return.
 *     also : (again) memory allocation changes...
 */
{

    char *returnval;

    returnval = (char *) allocate(puthere, strlen(newstring) + 1);

    strcpy(returnval, newstring);
    
    return(returnval);

} /*setstring*/



char *alltrim(char *str)
/*
 * Trim the spaces off of both ends of a string.
 *   params : *str == string to trim.
 *  returns : always returns (str).
 */
{
    ltrim(str);
    rtrim(str);
    return(str);
} /*alltrim*/


char *ltrim(char *str)
/* 
 * Trim the spaces off the left of a string.
 *
 *   params : *str == string to trim.
 *  returns : always returns (str);
 */
{    
    
    char *trimpoint;        /* Ptr to first non-trimmable char. */
    size_t charcount;       /* String length of trimmed string. */

    for (trimpoint = str; *trimpoint == ' '; trimpoint++);

    charcount = strlen(trimpoint);

    if (trimpoint != str)
        memmove(str, trimpoint, charcount+1);

    return(str);

} /*ltrim*/


char *rtrim(char *str)
/*
 *  Trim the spaces off the right of a string.
 *
 *    params : *str == string to trim.
 *   returns : always returns (str);
 */
{
    char *trimpoint;
    for (trimpoint = str + strlen(str) - 1; *trimpoint == ' '; trimpoint--);
        trimpoint[1] = '\0';
    return(str);
} /*rtrim*/


boolean twostrcmp(char *cmpthis, char *checkone, char *checktwo)
/*
 * Check to see if a string is equal to either of two others.
 *
 *   params : *cmpthis  == String to compare.
 *            *checkone == first string to compare against cmpthis.
 *            *checktwo == second string to compare against cmpthis.
 *  returns : boolean TRUE if cmpthis is equal (case sensitive) to 
 *             either checkone, checktwo, or both, boolean FALSE if
 *             cmpthis isn't equal to either checkone or checktwo.
 */
{
    
    if ((strcmp(cmpthis, checkone) == 0) || (strcmp(cmpthis, checktwo) == 0))
        return(TRUE);
    else
        return(FALSE);

} /*twostrcmp*/



void preprocess(char *unprocessed)
/*
 * Sets up inputted LOGO code in a way that the interpreter can
 *  understand. Everything is capitalized, and extra spaces removed.
 *
 *   params : *unprocessed == String to preprocess.
 *  returns : void.
 */
{
    char *looper;
    
    alltrim(unprocessed);

    for (looper = unprocessed; *looper != '\0'; looper++)
    {
        if (*looper == ' ')
            ltrim(looper + 1);
        
        else if (*looper == '\n')
        {    
            *looper = ' ';
            looper--;
        } /*else if*/
        
        *looper = toupper(*looper);

    } /*for*/

} /*preprocess*/



char charinput(FILE *charstream)
/*
 * Gets one character from specified input device.
 *
 *   params : *charstream == stream to read from. If NULL, use keyboard.
 *  returns : character read.
 */
{

    if (charstream == NULL)
       return(getch());
    else
       return(fgetc(charstream));

} /*charinput*/



char *_getline(FILE *fromhere, char *tohere, char *prompt, int maxline)
/* 
 * Reads a line of input, ending with an endline char. Allows input from a 
 *  a file or keyboard, a maximum input size, and string editing.
 *
 *   params : *fromhere == stream to get input from. NULL for keyboard input.
 *              *tohere == buffer to store input in.
 *              *prompt == prompt string to output before getting input.
 *              maxline == maximum characters to accept.
 *  returns : always returns (tohere).
 *     bugs : There could be general problems if maxline allows for input to
 *            spill onto a second line. This can be fixed without a major
 *            re-write, but I'm a lazy, lazy man.  :)
 */
{

    int currentchar = 0;          /* Current pointer offset for *tohere. */
    struct rccoord xy;            /* For storing screen coordinates.     */
    boolean getout = FALSE;       /* Should we continue getting chars?   */
    int looper;                   /* Loop control.                       */

    if (fromhere == NULL)
    {    
        _outtext(prompt);
        _displaycursor(_GCURSOROFF);
    } /*if*/

    while (!getout)
    {
        if ((fromhere != NULL) && (feof(fromhere)))
        {            
            tohere[currentchar] = '\0';
            getout = TRUE;
        } /*if*/

        else
        {
            tohere[currentchar] = charinput(fromhere);

            switch (tohere[currentchar])
            {
                case '\0':
                    switch (charinput(fromhere))
                    {

                        /*extended keys...finish this.*/

                    } /*switch*/
                    break;

                case ASCII_ESC:
                    if (fromhere != NULL)
                    {
                        xy = _gettextposition();
                        xy.col -= currentchar;
                        
                        _settextposition(xy.row, xy.col);
                        
                        for (looper = 0; looper <= currentchar; looper++)
                            _outchar(' ');
                        
                        _settextposition(xy.row, xy.col);
                        currentchar = 0;
                    } /*if*/
                    break;

                case '\a':  /* audible bell.     */
                case '\t':  /* tab character.    */
                case '\n':  /* newline.          */    
                    break;  /* ignore all those. */

                case '\b':  /* backspace.    */
                    if ((currentchar > 0) && (fromhere == NULL))
                    {
                        xy = _gettextposition();
                        xy.col--;
                        currentchar--;
                        _settextposition(xy.row, xy.col);
                        _outtext(" ");
                        _settextposition(xy.row, xy.col);
                    } /*if*/
                    break;

                case '\r':  /* carraige return. */
                    if (fromhere == NULL)
                    {    
                        _displaycursor(_GCURSOROFF);
                        _outtext("\n");
                    } /*if*/

                    tohere[currentchar] = '\0';
                    getout = TRUE;
                    break;
                          
                default:
                    if (currentchar < maxline)
                    {
                        if (fromhere == NULL)
                            _outchar(tohere[currentchar]);

                        currentchar++;
                    } /*if*/
                    break;

            } /*switch*/
        
        } /*else*/

    } /*while*/

    return(tohere);

} /*_getline*/


boolean getyn(char *prompt)
/*
 * Outputs a prompt, and gets a 'Y'es or 'N'o answer from keyboard.
 *
 *   params : *prompt == pointer to prompt to output.
 *  returns : Boolean TRUE if answer is yes, Boolean FALSE if answer is no.
 */
{
    
    char readch;
    
    _outtext(prompt);
    _outtext(" [y/n] : ");

    do
    {
        readch = toupper(getch());
    } while ((readch != 'Y') && (readch != 'N'));

    _outchar(readch);
    _outchar('\n');

    if (readch == 'Y')
        return(TRUE);
    else
        return(FALSE);

} /*getyn*/



boolean define_proc(FILE *fromhere, char *declaration)
/* 
 * Defines a LOGO procedure, and sets up data so logo_run() can make use
 *  of it.
 *
 *   params : *fromhere    == stream to get procedure from. NULL for keyboard
 *                             input.
 *            *declaration == string of declaration. (Name, and params)
 *            *procedures  == where to put procedure.
 *  returns : TRUE if procedure define, FALSE otherwise.
 *     also : memory allocation changed.
 */
{
    char declaredname[MAXCOMMAND];   /* procedure's name.           */
    char params[MAXCOMMAND + 1];     /* Store parameters.           */
    char procedureline[MAXCOMMAND];  /* input for individual lines. */
    char *totalproc;                 /* Total inputted lines.       */
    int memcount = 1;                /* Total allocated memory.     */
    boolean getout;                  /* Should we keep inputting?   */

    alltrim(declaration);
    
    totalproc = findword(declaration);
    
    if (*totalproc == ' ')
    {    
        *totalproc = '\0';
        strcpy(params, totalproc + 1);
    } /*if*/
    
    else
        *params = '\0';
    
    /* Add second null for parsing's sake. */    
    params[strlen(params) + 1] = '\0';
    strcpy(declaredname, declaration);

    if (
        (stricmp(declaredname, "OUTPUT") == 0) ||
        (stricmp(declaredname, "TO"    ) == 0) ||
        (stricmp(declaredname, "SAVE"  ) == 0) ||
        (stricmp(declaredname, "LOAD"  ) == 0) ||
        (stricmp(declaredname, "QUIT"  ) == 0)   
       )
    {
        _outchar('\"');
        _outtext(declaredname);
        _outchar('\"');
        logo_error(" cannot be overloaded.", RT_BADDECLARE);
        return(FALSE);
    } /*if*/
    
    if (!valid_ident(declaredname))
    {
        logo_error("Procedure name is not a valid identifier.", RT_BADDECLARE);
        return(FALSE);
    } /*if*/

    /* Make sure declaration is valid... */
    for (totalproc = params; *totalproc != '\0'; 
            totalproc = findword(totalproc + 1) + 1)
    {
        
        if (*totalproc != ':') /* Not a variable? */
        {    
            logo_error("Parameter name is not a valid identifier.",
            RT_BADDECLARE);
            return(FALSE);
        } /*if*/
        
        if (*findword(totalproc) != '\0')
        {
            *findword(totalproc) = '\0';
            getout = !valid_ident(totalproc + 1);
            *findword(totalproc) = ' ';
        } /*if*/
        
        else
            getout = !valid_ident(totalproc + 1);

        if (getout)
        {
            logo_error("Parameter name is not a valid identifier.",
                RT_BADDECLARE);
            return(FALSE);
        } /*if*/
    
    } /*for*/
    
    if (searchlist(procedures, declaredname) != NULL)
    {              
        if (getyn("That procedure is already defined. Overwrite it?"))
            removelist(procedures, declaredname);
        else
            return(FALSE);
    } /*if*/
            
    totalproc = setstring(NULL, "");
    
    getout = FALSE;
    while (!getout)
    {
        if ((fromhere != NULL) && (feof(fromhere)))
        {
            _outtext("Unexpected end of procedure \"");
            _outtext(declaredname);
            _outtext("\".\n");
            getout = TRUE;
        } /*if*/
    
        _getline(fromhere, procedureline, "> ", MAXCOMMAND - 1);
        
        if (stricmp(procedureline, "END") == 0)
            getout = TRUE;
        
        else if (strnicmp(procedureline, "TO ", 3) == 0)
            _outtext("Nested procedures are illegal. Line ignored.\n");
        
        else
        {
            strcat(procedureline, "\n");
            memcount += strlen(procedureline);
            totalproc = (char *) allocate(totalproc, memcount);
            strcat(totalproc, procedureline);
        } /*else*/
    
    } /*while*/

    if (memcount > 1)                    /* Remove that last '\n'... */
        totalproc[memcount-2] = '\0';

    if (procedures == NULL)
        procedures = addlist(NULL, declaredname, totalproc, params);
    else
        addlist(procedures, declaredname, totalproc, params);
    
    _outchar('\"');
    _outtext(declaredname);
    _outtext("\" defined.\n");
    shouldsave = TRUE;
    
    free(totalproc);
    
    return(TRUE);

} /*define_proc*/



void codeload(char *filename)
/* 
 *  Load a LOGO program from disk.
 *
 *      params : *filename == filename to get program from.
 *     returns : *procedures == list to add procedures to.
 *     returns : void.
 */
{

    FILE *logostream;
    char bytebucket[MAXCOMMAND];

    codesave(NULL);  /* save any current procedures, if necessary. */
    
    if (filename != NULL)
    {
        logostream = fopen(filename, "rb");

        if (logostream == NULL)
        {   
            _outtext("Cannot open \"");
            _outtext(filename);
            _outtext("\".\n");
        } /*if*/
    
        else
        {
            if (procedures != NULL)
                procedures = freelist(procedures);
            
            while (!feof(logostream))
            {
                _getline(logostream, bytebucket, "", MAXCOMMAND);
                if (strnicmp(bytebucket, "to ", 3) == 0)
                    define_proc(logostream, bytebucket + 3);
            } /*while*/
        
            fclose(logostream);    

            shouldsave = FALSE;
        
        } /*else*/
    
    } /*if*/

} /*codeload*/


boolean codesave(char *fname)
/*
 * Save LOGO procedures to disk.
 *
 *   params : void.
 *  returns : boolean TRUE if saved, boolean FALSE if not.
 */
{
    char filename[MAXCOMMAND];
    char *copydata;
    FILE *savestream = NULL;
    linkedlist *listloop;
    
    if (shouldsave) 
    {    
        if (!getyn("There are new or modified procedures. Save them?"))
            return(FALSE);
    } /*if*/
    else
        return(FALSE);
    
    if (fname == NULL)    
        _getline(NULL, filename, "Filename to save : ", MAXCOMMAND);
    else
        strcpy(filename, fname);
    
    if (*filename != '\0')
    {
        
        if (access(filename, 00) == 0)   /* Does file exist? */
        {   
            if (!getyn("That file already exists. Overwrite it?"))
                return(FALSE);
        } /*if*/
        
        /*else
        {*/    
            savestream = fopen(filename, "wb");
            if (savestream == NULL)
            {    
                _outtext("Could not open file! Aborted.\n");
                return(FALSE);
            } /*if*/
        /*}*/ /*else*/

        for (listloop = procedures; listloop != NULL; listloop = listloop->next)
        {    
            fprintf(savestream, "TO %s %s\r\n", listloop->name, 
                    listloop->params);
                
            for (copydata = listloop->data; *copydata != '\0'; copydata++)
            {
                if (*copydata == '\n')
                    fprintf(savestream, "\r\n");
                else
                    fputc(*copydata, savestream);
            } /*for*/

            fprintf(savestream, "\r\nEND\r\n\r\n");

        } /*for*/

        fclose(savestream);
        shouldsave = FALSE;   /* update global variable. */
        return(TRUE);
    
    } /*if*/

    else
    {    
        _outtext("SAVE aborted.\n");
        return(FALSE);
    } /*else*/

} /*codesave*/



char *findword(char *startword)
/*
 * Find a word, based on a space or NULL character separator.
 *
 *   params : *startword == begining of word.
 *  returns : pointer to character separator.
 */
{
    
    char *returnvalue;
    
    returnvalue = strchr(startword, ' ');

    if (returnvalue != NULL)
        return(returnvalue);        
    else
        return(strchr(startword, '\0'));

} /*findword*/


int list_length(char *list)
/*
 * Calculates length, in bytes, of a LOGO list.
 *
 *   params : *list == starting bracket of list.
 *  returns : count of bytes, counting start and end brackets.
 *            returns -1 if not a valid list.
 */
{
    int count;
    int listdepth = 1;

    for (count = 1, list++; listdepth > 0; list++, count++)
    {
    
        switch (*list)
        {
            case '[':         /* List in a list? */
                listdepth++;  
                break;
            
            case ']':
                listdepth--;
                break;
            
            case '\0':         /* Unexpected end of list? */
                return(-1);
                break;
        }  /*switch*/

    } /*for*/

    return(count);

} /*list_length*/



void insert_string(char *inserthere, char *str2insert)
/*
 * Insert a string in front of another. Memory allocation is not changed,
 *   so make sure you account for this.
 *
 *   params : *inserthere == start of string to insert into.
 *  returns : *str2insert == string to insert.
 */
{
    memmove(inserthere + strlen(str2insert), inserthere, 
            strlen(inserthere) + 1);

    memmove(inserthere, str2insert, strlen(str2insert));

} /*insert_string*/


int remove_tokens(char *fromhere, int howmany)
/*
 * Remove a number of tokens from the beggining of a string. Memory
 *   allocation does not change, however.
 *
 *   params : *fromhere == Where to remove tokens from.
 *             howmany  == amount of tokens to remove.
 *  returns : 0, always.
 */
{

    int count;                 /* Token counter.                */
    int listcount;             /* stores list_length() returns. */
    char *newstart;            /* New start of string.          */
    char *blank;
    char *endblank;

    for (count = 1, newstart = fromhere; count <= howmany; count++)
    {    
        if (*newstart == '[')   /* a list? */
        {    
            listcount = list_length(newstart);
            if (listcount == -1)
            {    
                logo_error("Invalid list.", RT_BADLIST);
                return(0);
            } /*if*/

            else
                newstart = newstart + listcount;
        } /*if*/
        
        else
            newstart = findword(newstart + 1);
    
    } /*for*/
    
    endblank = strchr(fromhere, '\0');
    memmove(fromhere, newstart, strlen(newstart) + 1);
    
    for (blank = strchr(fromhere, '\0'); blank < endblank; blank++);
        *blank = '\0';

    ltrim(fromhere);
    
    return(0);

} /*remove_tokens*/


void pots(linkedlist *potsthis)
/*
 * Print out procedure names, via the POTS primitive.
 *
 *  params : void.
 * returns : void.
 */
{
    linkedlist *item;
    char counter = 0;
    
    if (procedures == NULL)
        _outtext("Nothing to show.\n");
    else
    {
        for (item = potsthis; item != NULL; item = item->next)
        {
            _outtext(item->name);
            _outchar(' ');
            _outtext(item->params);
            _outchar('\n');
            counter++;
            if (counter == 4)
            {
                counter = 0;
                _outtext("[Press any key]");
                getch();
                _outchar('\n');
            } /*if*/
            
        } /*for*/
    
    } /*else*/

} /*pots*/


boolean valid_ident(char *declaredname)
/* 
 * Check if string is a valid identifier. (Doesn't start with a number, 
 *  and doesn't contain certain characters, and is more than 0 bytes...)
 *
 *    params : *declaredname == string to check.
 *   returns : boolean TRUE if valid, boolean FALSE if not.
 */
{
    
    if (*declaredname == '\0')
        return(FALSE);

    else if ((*declaredname >= '0') && (*declaredname <= '9'))
        return(FALSE);
    
    else if (
             (strchr(declaredname, ':' ) != NULL) ||
             (strchr(declaredname, '\"') != NULL) ||
             (strchr(declaredname, '+')  != NULL) ||
             (strchr(declaredname, '-')  != NULL) ||
             (strchr(declaredname, '*')  != NULL) ||
             (strchr(declaredname, '/')  != NULL) ||
             (strchr(declaredname, '[')  != NULL) ||
             (strchr(declaredname, ']')  != NULL) 
            )
        return(FALSE);
    
    else
        return(TRUE);

} /*valid_ident*/
