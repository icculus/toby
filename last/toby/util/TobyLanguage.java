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
 *   language; all one need do is translate all the strings in this
 *   file.
 *
 *      Written by Ryan C. Gordon.
 */

package last.toby.util;

public final class TobyLanguage
{
    public static final String TRANSLATION_BY =
                                    "English language text by Ryan C. Gordon.";

    public static final String USAGE = "USAGE: Toby.class [sourceFile.toby]";
    public static final String WRITTENBY = "Written by Ryan C. Gordon.";
    public static final String COPYRIGHT =
                                    "Copyright (C) 1999 Ryan C. Gordon";

    public static final String ERROR = "Error";
    public static final String NOT_YET_IMPLEMENTED = "Not yet implemented.";
    public static final String NODETREE_LOCKED = "NodeTree already locked.";
    public static final String NODETREE_ULOCKED = "NodeTree already unlocked.";
    public static final String NEW_PROGRAM = "new program";
    public static final String PLEASE_CONFIRM = "Whoa!";
    public static final String SAVE_MODIFIED_PROG = "Save modified program?";
    public static final String FILE_NOT_FOUND = "File \"%0\" not found.";
    public static final String CANNOT_LOAD_FILE = "Cannot load file \"%0\".";
    public static final String FILE_EXISTS = "File exists";
    public static final String OVERWRITE_FILENAME = "Overwrite \"%0\"?";
    public static final String CANNOT_WRITE_TO = "Cannot write to \"%0\".";
    public static final String MISSING_CLASS1 =
                                 "Cannot find a necessary .class file.";
    public static final String MISSING_CLASS2 =
                                 " Perhaps you need to set your CLASSPATH?";
    public static final String MISSING_CLASS3 =
                                 " Also, make sure you have JDK 1.2 or later!";
    public static final String REPLACE_FORMAT_ERR =
                        "Format string contains no \"%\" sequence!";
    public static final String REPLACE_RANGE_ERR =
                        "Replace index must be between (0) and (9).";

    public static final String NO_GUI1 =
                               "Your Java Virtual Machine has thrown" +
                               " an error...chances are, you don't have";
    public static final String NO_GUI2 =
                               " the GUI support you need. Please start" +
                               " your Window system before running this";
    public static final String NO_GUI3 =
                               " program.";

    public static final String HERES_THE_ERR = "For record, here's the error:";

    public static final String MSG_BAD_JAVA =
         "Your Java Virtual Machine is too old; goto http://java.sun.com/ ...";

    public static final String SETICON_BROKEN =
                               "WARNING: setIconImage() is broken on this" +
                               " Java Virtual Machine!";

    public static final String WORK_AROUND_IT =
                                        " (We will work around it, though.)";

    public static final String TOBY_FILE_DESCRIPTION = "TOBY source code";

    public static final String CARET_POSITION = "line %0, column %1";

    public static final String ERR_IN_FUNC = "Error in function \"%0\" %1: %2";
    public static final String SOURCE_LINE = "(source line %0)";

        // Menuitems...
    public static final String MENUNAME_FILE   = "File";
    public static final String MENUITEM_NEW    = "New";
    public static final String MENUITEM_OPEN   = "Open...";
    public static final String MENUITEM_SAVE   = "Save";
    public static final String MENUITEM_SAVEAS = "Save as...";
    public static final String MENUITEM_PRINT  = "Print...";
    public static final String MENUITEM_QUIT   = "Quit";

    public static final String MENUNAME_HELP  = "Help";
    public static final String MENUITEM_HELP  = "Help...";
    public static final String MENUITEM_ABOUT = "About...";

    public static final String MENUNAME_RUN       = "Run";
    public static final String MENUITEM_STARTCODE = "Start program";
    public static final String MENUITEM_STOPCODE  = "Stop program";
    public static final String MENUITEM_CLEAR     = "Cleanup TurtleSpace";

    public static final String MENUNAME_DEBUG     = "Debug";
    public static final String MENUITEM_TRACE     = "Trace";
    public static final String MENUITEM_STEP      = "Step";
    public static final String MENUITEM_WATCHVARS = "Watch variables";


        // Constants representing error messages...
    public final static String INTERNAL_ERROR  = "Internal Error";
    public final static String EXPECTED_TOKEN  = "Expected token";
    public final static String SYNTAX_ERROR    = "Syntax error";
    public final static String BAD_ASSIGNMENT  = "Assignment not allowed";
    public final static String DOUBLE_DEF      = "Duplicate Definition";
    public final static String ORPHAN_CODE     = "Code outside function";
    public final static String INTERNAL        = "Internal error";
    public final static String BAD_TYPE        = "Bad variable type";
    public final static String NOT_VAR         = "Expected variable";
    public final static String NO_MAINLINE     = "Expected mainline";
    public final static String NO_RPAREN       = "Expected \")\"";
    public final static String NO_LPAREN       = "Expected \"(\"";
    public final static String NO_ASSIGN       = "Expected \"=\"";
    public final static String NOT_A_FUNC      = "Undefined function";
    public final static String BADNUM_ARGS     = "Wrong arguments";
    public final static String BAD_IDENT       = "Invalid identifier";
    public final static String NO_ENDFOR       = "FOR without ENDFOR";
    public final static String NO_FOR          = "ENDFOR without FOR";
    public final static String NO_ENDWHILE     = "WHILE without ENDWHILE";
    public final static String NO_WHILE        = "ENDWHILE without WHILE";
    public final static String ORPHAN_ELIF     = "ELSEIF without IF";
    public final static String ORPHAN_ELSE     = "ELSE without IF";
    public final static String ORPHAN_ENDIF    = "ENDIF without IF";
    public final static String TYPE_MMATCH     = "Type mismatch";
    public final static String BAD_ARGUMENT    = "Invalid argument";
    public final static String MAIN_RETVAL     = "Main returns a value";
    public final static String BAD_GLOBAL      = "Expected variable or FUNCTION";
    public final static String EXPECTED_END    = "Expected ENDFUNCTION";
    public final static String NO_RETTYPE      = "Expected RETURNS";
    public final static String ELIF_AFTER_ELSE = "ELSEIF after ELSE";
    public final static String ELSE_AFTER_ELSE = "Multiple ELSE statements";
    public final static String NO_VAR_DECL     = "Can't declare variables here";
    public final static String FUNC_IN_FUNC    = "FUNCTION within function";
    public final static String DIV_BY_ZERO     = "Division by Zero";
    public final static String NOCURTURTLE     = "No current turtle";
    public final static String TURTLE_FENCED   = "Turtle past fence";

    public final static String BAD_CHILD_NODE  = "Adding bad child type to NodeTree";
    public final static String USING_FREE_NODE = "Attempt to use NodeTree" +
                                                 " that has already called" +
                                                 " freeResources()!";

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

