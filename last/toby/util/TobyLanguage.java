/*
 * TOBY -- A LOGO-like interpreted language, written in 100% pure Java.
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

/**
 *  This class contains all the literal strings used throughout Toby.
 *   By keeping these all in one place, it becomes trivial to change
 *   not only individual text within the program, but also the entire
 *   language.
 *
 *  To use a different language than English, just specify
 *    --langfile=/path/langfile or --langfile=url://hostname/path/langfile
 *    on the command line.
 *
 *     @author Ryan C. Gordon. (icculus@lokigames.com)
 */

package last.toby.util;

import java.io.*;
import java.net.*;
import java.lang.reflect.*;

public class TobyLanguage
{

        // Do NOT translate these strings!
        //  The program will default to English (using the below strings)
        //  if it doesn't load langfiles to use a different language.
        //  Translations should be done from a langfile.

    public static String USAGE = "USAGE: Toby.class [--langfile=xxx] [sourceFile.toby]";

    public static String TRANSLATION_BY =
                                    "English language text by Ryan C. Gordon.";

    public static String WRITTENBY = "Written by Ryan C. Gordon.";
    public static String COPYRIGHT = "Copyright (C) 1999 Ryan C. Gordon";

    public static String ERROR = "Error";
    public static String NOT_YET_IMPLEMENTED = "Feature not yet implemented.";
    public static String NODETREE_LOCKED = "NodeTree already locked.";
    public static String NODETREE_ULOCKED = "NodeTree already unlocked.";
    public static String BAD_CHILD_NODE  = "Adding bad child type to NodeTree";
    public static String USING_FREE_NODE = "Attempt to use NodeTree" +
                                           " that has already called" +
                                           " freeResources()!";

    public static String NEW_PROGRAM = "new program";
    public static String PLEASE_CONFIRM = "Whoa!";
    public static String SAVE_MODIFIED_PROG = "Save modified program?";
    public static String FILE_NOT_FOUND = "File \"%0\" not found.";
    public static String CANNOT_LOAD_FILE = "Cannot load file \"%0\".";
    public static String FILE_EXISTS = "File exists";
    public static String OVERWRITE_FILENAME = "Overwrite \"%0\"?";
    public static String CANNOT_WRITE_TO = "Cannot write to \"%0\".";
    public static String MISSING_CLASS1 =
                                 "Cannot find a necessary .class file.";
    public static String MISSING_CLASS2 =
                                 " Perhaps you need to set your CLASSPATH?";
    public static String MISSING_CLASS3 =
                                 " Also, make sure you have JDK 1.2 or later!";

    public static String TOOMANYFILES = "More than one file specified on" +
                                        " commandline! Just using the" +
                                        " first one.";

    public static String REPLACE_FORMAT_ERR =
                        "Format string contains no \"%\" sequence!";
    public static String REPLACE_RANGE_ERR =
                        "Replace index must be between (0) and (9).";

    public static String NO_GUI1 = "Your Java Virtual Machine has thrown" +
                                   " an error...chances are, you don't have";
    public static String NO_GUI2 = " the GUI support you need. Please start" +
                                   " your Window system before running this";
    public static String NO_GUI3 = " program.";

    public static String HERES_THE_ERR = "For record, here's the error:";

    public static String MSG_BAD_JAVA =
         "Your Java Virtual Machine is too old; goto http://java.sun.com/ ...";

    public static String SETICON_BROKEN =
                               "WARNING: setIconImage() is broken on this" +
                               " Java Virtual Machine!";

    public static String WORK_AROUND_IT = " (We will work around it, though.)";

    public static String TOBY_FILE_DESCRIPTION = "TOBY source code";

    public static String CARET_POSITION = "line %0, column %1";

    public static String ERR_IN_FUNC = "%0 on line %1 in function %2";
    public static String ERR_OUT_FUNC = "%0 on line %1";

    public static String SHOULDNOTBE = "This should not happen." +
                                       " Email icculus@lokigames.com!" +
                                       " (English only!)";

        // Menuitems...
    public static String MENUNAME_FILE   = "File";
    public static String MENUITEM_NEW    = "New";
    public static String MENUITEM_OPEN   = "Open...";
    public static String MENUITEM_SAVE   = "Save";
    public static String MENUITEM_SAVEAS = "Save as...";
    public static String MENUITEM_PRINT  = "Print...";
    public static String MENUITEM_QUIT   = "Quit";

    public static String MENUNAME_HELP  = "Help";
    public static String MENUITEM_HELP  = "Help...";
    public static String MENUITEM_ABOUT = "About...";

    public static String MENUNAME_RUN       = "Run";
    public static String MENUITEM_STARTCODE = "Start program";
    public static String MENUITEM_STOPCODE  = "Stop program";
    public static String MENUITEM_CLEAR     = "Cleanup TurtleSpace";

    public static String MENUNAME_DEBUG     = "Debug";
    public static String MENUITEM_TRACE     = "Trace";
    public static String MENUITEM_STEP      = "Step";
    public static String MENUITEM_WATCHVARS = "Watch variables";


        // Constants representing error messages...
    public static String INTERNAL_ERROR  = "Internal Error";
    public static String EXPECTED_TOKEN  = "Expected token";
    public static String SYNTAX_ERROR    = "Syntax error";
    public static String BAD_ASSIGNMENT  = "Assignment not allowed";
    public static String DOUBLE_DEF      = "Duplicate Definition";
    public static String ORPHAN_CODE     = "Code outside function";
    public static String INTERNAL        = "Internal error";
    public static String BAD_TYPE        = "Bad variable type";
    public static String NOT_VAR         = "Expected variable";
    public static String NO_MAINLINE     = "Expected mainline";
    public static String NO_RPAREN       = "Expected \")\"";
    public static String NO_LPAREN       = "Expected \"(\"";
    public static String NO_ASSIGN       = "Expected \"=\"";
    public static String NOT_A_FUNC      = "Undefined function";
    public static String BADNUM_ARGS     = "Wrong arguments";
    public static String BAD_IDENT       = "Invalid identifier";
    public static String NO_ENDFOR       = "FOR without ENDFOR";
    public static String NO_FOR          = "ENDFOR without FOR";
    public static String NO_ENDWHILE     = "WHILE without ENDWHILE";
    public static String NO_WHILE        = "ENDWHILE without WHILE";
    public static String ORPHAN_ELIF     = "ELSEIF without IF";
    public static String ORPHAN_ELSE     = "ELSE without IF";
    public static String ORPHAN_ENDIF    = "ENDIF without IF";
    public static String TYPE_MMATCH     = "Type mismatch";
    public static String BAD_ARGUMENT    = "Invalid argument";
    public static String MAIN_RETVAL     = "MAIN returns a value";
    public static String BAD_GLOBAL      = "Expected variable or FUNCTION";
    public static String EXPECTED_END    = "Expected ENDFUNCTION";
    public static String NO_RETTYPE      = "Expected RETURNS";
    public static String ELIF_AFTER_ELSE = "ELSEIF after ELSE";
    public static String ELSE_AFTER_ELSE = "Multiple ELSE statements";
    public static String NO_VAR_DECL     = "Can't declare variables here";
    public static String FUNC_IN_FUNC    = "FUNCTION within function";
    public static String DIV_BY_ZERO     = "Division by Zero";
    public static String NOCURTURTLE     = "No current turtle";
    public static String TURTLE_FENCED   = "Turtle past fence";

    public static String LANGIOEXCEPT    = "LANGUAGE: IOException reading" +
                                           " language data.";
    public static String LANGBOGUSLINE   = "LANGUAGE: Line %0 is bogus in %1.";
    public static String LANGNOSUCHCLASS = "LANGUAGE: No such class [%0] on" +
                                           " line %1 in %2.";
    public static String LANGNOSUCHFIELD = "LANGUAGE: No such field [%0] on" +
                                           " line %1 in %2.";
    public static String LANGNOTPUBLIC   = "LANGUAGE: Field [%0] is not" +
                                           " public on line %1 in %2.";
    public static String LANGNOTSTATIC   = "LANGUAGE: Field [%0] is not" +
                                           " static on line %1 in %2.";
    public static String LANGNOTSTRING   = "LANGUAGE: Field [%0] is not" +
                                           " a string on line %1 in %2.";
    public static String LANGISFINAL     = "LANGUAGE: Field [%0] is final" +
                                           " on line %1 in %2.";
    public static String LANGSETFAILED   = "LANGUAGE: Setting field [%0] " +
                                           " failed with message [%1]" +
                                           " on line %2 in %3.";



        /**
         * Read a string from a langfile. This will read until a non-blank
         *  line that isn't a comment is found.
         *
         *      @param br BufferedReader that is reading a langfile.
         *     @return String in a given language.
         *  @exception IOException on i/o errors.
         */
    public static String readLangString(BufferedReader br) throws IOException
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


    public static Class retrieveLangClass(String cName, int line, String file)
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


    public static Field retrieveLangField(Class c, String fName,
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


    public static boolean setLangField(String cName, String fName, String val,
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


    public static void reportBogusLangLine(int lineNum, String fname)
    {
        String s = TobyLanguage.LANGBOGUSLINE;
        s = replaceFmtTokenInStr(0, s, lineNum);
        s = replaceFmtTokenInStr(1, s, fname);
        System.err.println(s);
    } // reportBogusLangLine


    public static void loadLangFile(LineNumberReader lnr, String file) throws IOException
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



        /**
         * !!! comment out of date.
         * Return a copy of (fmt), with its first occurance of
         *  "%s" replaced with (replacer).
         *
         *  @param n index of % formatting token to replace.
         *  @param fmt String to copy, with replacement.
         *  @param val string to to take the place of "%s".
         *  @throw IllegalArgumentException if (fmt) has no "%s" sequence.
         */
    public static String replaceFmtTokenInStr(int n, String fmt, String val)
    {
        String retVal = null;
        int idx;

        if ((n < 0) || (n > 9))
            throw(new IllegalArgumentException(REPLACE_RANGE_ERR));
        else
        {
            idx = fmt.indexOf("%" + n);

            if (idx == -1)
                throw(new IllegalArgumentException(REPLACE_FORMAT_ERR));
            else
            {
                StringBuffer sb = new StringBuffer();
                sb.append(fmt.substring(0, idx));
                sb.append(val);
                sb.append(fmt.substring(idx + 2));
                retVal = sb.toString();
                sb.setLength(0);
            } // else
        } // else
        return(retVal);
    } // replaceFmtTokenInStr (overloaded; takes String.)


    public static String replaceFmtTokenInStr(int n, String fmt, int val)
    {
        return(replaceFmtTokenInStr(n, fmt, "" + val)); // !!! hack.
    } // replaceFmtTokenInStr (overloaded; takes int.)

} // TobyLanguage

// end of TobyLanguage.java ...

