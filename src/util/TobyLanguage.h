/*
 * TOBY -- An abstract interpreter engine and system for learning.
 * Copyright (C) 1999  Ryan C. Gordon.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _INCLUDE_TOBYLANGUAGE_H_
#define _INCLUDE_TOBYLANGUAGE_H_

#include "util/TobyObject.h"

/*
 *  This class contains all the literal strings used throughout Toby.
 *   By keeping these all in one place, it becomes trivial to change
 *   not only individual text within the program, but also the entire
 *   language.
 *
 *  To use a different language than English, just specify
 *    --langfile=/path/langfile or --langfile=url://hostname/path/langfile
 *    on the command line.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class TobyLanguage
{
private:
    TobyLanguage(void) {}   // prevent public construction.

public:
    static char *NAME;
    static char *USAGE;
    static char *TRANSLATION_BY;
    static char *WRITTENBY;
    static char *COPYRIGHT;
    static char *ERROR;
    static char *NOT_YET_IMPLEMENTED;
    static char *NODETREE_LOCKED;
    static char *NODETREE_ULOCKED;
    static char *BAD_CHILD_NODE;
    static char *USING_FREE_NODE;
    static char *NEW_PROGRAM;
    static char *PLEASE_CONFIRM;
    static char *SAVE_MODIFIED_PROG;
    static char *FILE_NOT_FOUND;
    static char *CANNOT_LOAD_FILE;
    static char *FILE_EXISTS;
    static char *OVERWRITE_FILENAME;
    static char *CANNOT_WRITE_TO;
    static char *TOOMANYFILES;
    static char *REPLACE_FORMAT_ERR;
    static char *REPLACE_RANGE_ERR;
    static char *NO_GUI1;
    static char *NO_GUI2;
    static char *NO_GUI3;
    static char *HERES_THE_ERR;
    static char *TOBY_FILE_DESCRIPTION;
    static char *CARET_POSITION;
    static char *ERR_IN_FUNC;
    static char *ERR_OUT_FUNC;
    static char *SHOULDNOTBE;
    static char *MENUNAME_FILE;
    static char *MENUITEM_NEW;
    static char *MENUITEM_OPEN;
    static char *MENUITEM_SAVE;
    static char *MENUITEM_SAVEAS;
    static char *MENUITEM_PRINT;
    static char *MENUITEM_QUIT;
    static char *MENUNAME_HELP;
    static char *MENUITEM_HELP;
    static char *MENUITEM_ABOUT;
    static char *MENUNAME_RUN;
    static char *MENUITEM_STARTCODE;
    static char *MENUITEM_STOPCODE;
    static char *MENUITEM_CLEAR;
    static char *MENUNAME_DEBUG;
    static char *MENUITEM_TRACE;
    static char *MENUITEM_STEP;
    static char *MENUITEM_WATCHVARS;
    static char *INTERNAL_ERROR;
    static char *EXPECTED_TOKEN;
    static char *SYNTAX_ERROR;
    static char *BAD_ASSIGNMENT;
    static char *DOUBLE_DEF;
    static char *ORPHAN_CODE;
    static char *BAD_TYPE;
    static char *NOT_VAR;
    static char *NO_MAINLINE;
    static char *NO_RPAREN;
    static char *NO_LPAREN;
    static char *NO_ASSIGN;
    static char *NOT_A_FUNC;
    static char *BADNUM_ARGS;
    static char *BAD_IDENT;
    static char *NO_ENDFOR;
    static char *NO_FOR;
    static char *NO_ENDWHILE;
    static char *NO_WHILE;
    static char *ORPHAN_ELIF;
    static char *ORPHAN_ELSE;
    static char *ORPHAN_ENDIF;
    static char *TYPE_MMATCH;
    static char *BAD_ARGUMENT;
    static char *MAIN_RETVAL;
    static char *BAD_GLOBAL;
    static char *EXPECTED_END;
    static char *NO_RETTYPE;
    static char *ELIF_AFTER_ELSE;
    static char *ELSE_AFTER_ELSE;
    static char *NO_VAR_DECL;
    static char *FUNC_IN_FUNC;
    static char *DIV_BY_ZERO;
    static char *NOCURTURTLE;
    static char *TURTLE_FENCED;
    static char *OUT_OF_RANGE;
    static char *OVERFLOW;
    static char *NOT_AN_ARRAY;
    static char *NO_RARRAY;
    static char *NO_LARRAY;
    static char *NOT_WHOLE_NUM;
    static char *LANGIOEXCEPT;
    static char *LANGBOGUSLINE;
    static char *LANGNOSUCHCLASS;
    static char *LANGNOSUCHFIELD;
    static char *LANGNOTPUBLIC;
    static char *LANGNOTSTATIC;
    static char *LANGNOTSTRING;
    static char *LANGISFINAL;
    static char *LANGSETFAILED;
    static char *FILE_NOT_OPEN;
    static char *END_OF_FILE;

#if 0
// !!! PUT THIS BACK IN!

        /*
         * Read a string from a langfile. This will read until a non-blank
         *  line that isn't a comment is found.
         *
         *    params: ir == InputReader that is reading a langfile.
         *   returns: char *in a given language.
         *  @exception IOException on i/o errors.
         */
    static String readLangString(InputReader ir) throw (IOException)
    {
        String retVal = null;

        do
        {
            retVal = br.readLine();

            if (retVal == null)
                return(null);

            retVal = retVal.trim();
            if ((retVal.length() > 0) && (retVal.charAt(0) != '#'))
                return(retVal);

        } while (true);
    } // readLangString


    static Class retrieveLangClass(String cName, int line, String file)
    {
        Class retVal = null;
        try
        {
            retVal = Class.forName(cName);
        } // try
        catch (ClassNotFoundException cnfe)
        {
            String s = TobyLanguage.LANGNOSUCHCLASS;
            s = replaceFmtTokenInStr(0, s, cName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
        } // catch

        return(retVal);
    } // retrieveLangClass


    static Field retrieveLangField(Class c, String fName,
                                            int line, String file)
    {
        Field retVal = null;

        try
        {
            retVal = c.getField(fName);
        } // try
        catch (NoSuchFieldException nsfe)
        {
            String s = TobyLanguage.LANGNOSUCHFIELD;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
            return(null);
        } // catch

        int mods = retVal.getModifiers();

        if (java.lang.reflect.Modifier.isPublic(mods) == false)
        {
            String s = TobyLanguage.LANGNOTPUBLIC;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
            return(null);
        } // if

        else if (java.lang.reflect.Modifier.isStatic(mods) == false)
        {
            String s = TobyLanguage.LANGNOTSTATIC;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
            return(null);
        } // else if

        else if (java.lang.reflect.Modifier.isFinal(mods) == true)
        {
            String s = TobyLanguage.LANGISFINAL;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
            return(null);
        } // else if

        else if (retVal.getType().getName().equals("java.lang.String") == false)
        {
            String s = TobyLanguage.LANGNOTSTRING;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, line);
            s = replaceFmtTokenInStr(2, s, file);
            System.err.println(s);
            return(null);
        } // else if

        return(retVal);
    } // retrieveLangField


    static boolean setLangField(String cName, String fName, String val,
                                        int line, String file)
    {
        Class c = retrieveLangClass(cName, line, file);
        if (c == null)
            return(false);

        Field f = retrieveLangField(c, fName, line, file);
        if (f == null)
            return(false);

        try
        {
            f.set(null, val);
        } // try
        catch (Exception e)
        {
            String s = TobyLanguage.LANGSETFAILED;
            s = replaceFmtTokenInStr(0, s, fName);
            s = replaceFmtTokenInStr(1, s, e.getMessage());
            s = replaceFmtTokenInStr(2, s, line);
            s = replaceFmtTokenInStr(3, s, file);
            System.err.println(s);
            return(false);
        } // catch

        return(true);
    } // setLangField


    static void reportBogusLangLine(int lineNum, String fname)
    {
        String s = TobyLanguage.LANGBOGUSLINE;
        s = replaceFmtTokenInStr(0, s, lineNum);
        s = replaceFmtTokenInStr(1, s, fname);
        System.err.println(s);
    } // reportBogusLangLine


    static void loadLangFile(LineNumberReader lnr, String file) throws IOException
    {
        String langString = null;

        while ( (langString = readLangString(lnr)) != null )
        {
            int pos = langString.indexOf('=');
            int line = lnr.getLineNumber();
            if (pos == -1)
            {
                reportBogusLangLine(line, file);
                continue;
            } // if

            String fullName = langString.substring(0, pos).trim();
            String value = langString.substring(pos + 1).trim();

            pos = fullName.lastIndexOf('.');
            if (pos == -1)
            {
                reportBogusLangLine(line, file);
                continue;
            } // if

            String className = fullName.substring(0, pos);
            String fieldName = fullName.substring(pos + 1);

            setLangField(className, fieldName, value, line, file);
        } // while
    } // loadLangFile
#endif

}; // class TobyLanguage

#endif  // !defined _INCLUDE_TOBYLANGUAGE_H_

// end of TobyLanguage.h ...

