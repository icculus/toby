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

#include "util/TobyLanguage.h"


        // Do NOT translate the strings in here!
        //  The program will default to English (using the below strings)
        //  if it doesn't load langfiles to use a different language.
        //  Translations should be done from a separate langfile.

char *TobyLanguage::NAME = "Toby";

char *TobyLanguage::USAGE = "USAGE: Toby.class [--langfile=xxx] [sourceFile.toby]";

char *TobyLanguage::TRANSLATION_BY =
                                    "English language text by Ryan C. Gordon.";

char *TobyLanguage::WRITTENBY = "Written by Ryan C. Gordon.";
char *TobyLanguage::COPYRIGHT = "Copyright (C) 1999 Ryan C. Gordon";

char *TobyLanguage::ERROR = "Error";
char *TobyLanguage::NOT_YET_IMPLEMENTED = "Feature not yet implemented.";
char *TobyLanguage::NODETREE_LOCKED = "NodeTree already locked.";
char *TobyLanguage::NODETREE_ULOCKED = "NodeTree already unlocked.";
char *TobyLanguage::BAD_CHILD_NODE  = "Adding bad child type to NodeTree";
char *TobyLanguage::USING_FREE_NODE = "Attempt to use NodeTree"
                                      " that has already called"
                                      " freeResources()!";

char *TobyLanguage::NEW_PROGRAM = "new program";
char *TobyLanguage::PLEASE_CONFIRM = "Whoa!";
char *TobyLanguage::SAVE_MODIFIED_PROG = "Save modified program?";
char *TobyLanguage::FILE_NOT_FOUND = "File \"%0\" not found.";
char *TobyLanguage::CANNOT_LOAD_FILE = "Cannot load file \"%0\".";
char *TobyLanguage::FILE_EXISTS = "File exists";
char *TobyLanguage::OVERWRITE_FILENAME = "Overwrite \"%0\"?";
char *TobyLanguage::CANNOT_WRITE_TO = "Cannot write to \"%0\".";

char *TobyLanguage::TOOMANYFILES = "More than one file specified on"
                                   " commandline! Just using the first one.";

char *TobyLanguage::REPLACE_FORMAT_ERR =
                        "Format string contains no \"%\" sequence!";
char *TobyLanguage::REPLACE_RANGE_ERR =
                        "Replace index must be between (0) and (9).";

char *TobyLanguage::NO_GUI1 = "There was an error while starting the"
                              " graphics engine...";
char *TobyLanguage::NO_GUI2 = " Chances are, you don't have the GUI support"
                              " you need. Please start";
char *TobyLanguage::NO_GUI3 = " your Window system before running"
                              " this program.";

char *TobyLanguage::HERES_THE_ERR = "For record, here's the error:";

char *TobyLanguage::TOBY_FILE_DESCRIPTION = "TOBY source code";

char *TobyLanguage::CARET_POSITION = "line %0, column %1";

char *TobyLanguage::ERR_IN_FUNC = "%0 on line %1 in function %2";
char *TobyLanguage::ERR_OUT_FUNC = "%0 on line %1";

char *TobyLanguage::SHOULDNOTBE = "This should not happen."
                                  " Email icculus@linuxgames.com!"
                                  " (English only!)";

        // Menuitems...
char *TobyLanguage::MENUNAME_FILE   = "File";
char *TobyLanguage::MENUITEM_NEW    = "New";
char *TobyLanguage::MENUITEM_OPEN   = "Open...";
char *TobyLanguage::MENUITEM_SAVE   = "Save";
char *TobyLanguage::MENUITEM_SAVEAS = "Save as...";
char *TobyLanguage::MENUITEM_PRINT  = "Print...";
char *TobyLanguage::MENUITEM_QUIT   = "Quit";

char *TobyLanguage::MENUNAME_HELP  = "Help";
char *TobyLanguage::MENUITEM_HELP  = "Help...";
char *TobyLanguage::MENUITEM_ABOUT = "About...";

char *TobyLanguage::MENUNAME_RUN       = "Run";
char *TobyLanguage::MENUITEM_STARTCODE = "Start program";
char *TobyLanguage::MENUITEM_STOPCODE  = "Stop program";
char *TobyLanguage::MENUITEM_CLEAR     = "Cleanup TurtleSpace";

char *TobyLanguage::MENUNAME_DEBUG     = "Debug";
char *TobyLanguage::MENUITEM_TRACE     = "Trace";
char *TobyLanguage::MENUITEM_STEP      = "Step";
char *TobyLanguage::MENUITEM_WATCHVARS = "Watch variables";


        // Constants representing error messages...
char *TobyLanguage::INTERNAL_ERROR  = "Internal error";
char *TobyLanguage::EXPECTED_TOKEN  = "Expected token";
char *TobyLanguage::SYNTAX_ERROR    = "Syntax error";
char *TobyLanguage::BAD_ASSIGNMENT  = "Assignment not allowed";
char *TobyLanguage::DOUBLE_DEF      = "Duplicate definition";
char *TobyLanguage::ORPHAN_CODE     = "Code outside function";
char *TobyLanguage::BAD_TYPE        = "Bad variable type";
char *TobyLanguage::NOT_VAR         = "Expected variable";
char *TobyLanguage::NO_MAINLINE     = "Expected mainline";
char *TobyLanguage::NO_RPAREN       = "Expected \")\"";
char *TobyLanguage::NO_LPAREN       = "Expected \"(\"";
char *TobyLanguage::NO_ASSIGN       = "Expected \"=\"";
char *TobyLanguage::NOT_A_FUNC      = "Undefined function";
char *TobyLanguage::BADNUM_ARGS     = "Wrong arguments";
char *TobyLanguage::BAD_IDENT       = "Invalid identifier";
char *TobyLanguage::NO_ENDFOR       = "FOR without ENDFOR";
char *TobyLanguage::NO_FOR          = "ENDFOR without FOR";
char *TobyLanguage::NO_ENDWHILE     = "WHILE without ENDWHILE";
char *TobyLanguage::NO_WHILE        = "ENDWHILE without WHILE";
char *TobyLanguage::ORPHAN_ELIF     = "ELSEIF without IF";
char *TobyLanguage::ORPHAN_ELSE     = "ELSE without IF";
char *TobyLanguage::ORPHAN_ENDIF    = "ENDIF without IF";
char *TobyLanguage::TYPE_MMATCH     = "Type mismatch";
char *TobyLanguage::BAD_ARGUMENT    = "Invalid argument";
char *TobyLanguage::MAIN_RETVAL     = "MAIN returns a value";
char *TobyLanguage::BAD_GLOBAL      = "Expected variable or FUNCTION";
char *TobyLanguage::EXPECTED_END    = "Expected ENDFUNCTION";
char *TobyLanguage::NO_RETTYPE      = "Expected RETURNS";
char *TobyLanguage::ELIF_AFTER_ELSE = "ELSEIF after ELSE";
char *TobyLanguage::ELSE_AFTER_ELSE = "Multiple ELSE statements";
char *TobyLanguage::NO_VAR_DECL     = "Can't declare variables here";
char *TobyLanguage::FUNC_IN_FUNC    = "FUNCTION within function";
char *TobyLanguage::DIV_BY_ZERO     = "Division by zero";
char *TobyLanguage::NOCURTURTLE     = "No current turtle";
char *TobyLanguage::TURTLE_FENCED   = "Turtle past fence";
char *TobyLanguage::OUT_OF_RANGE    = "Out of range";
char *TobyLanguage::OVERFLOW        = "Overflow";
char *TobyLanguage::NOT_AN_ARRAY    = "Expected array";
char *TobyLanguage::NO_RARRAY       = "Expected \"]\"";
char *TobyLanguage::NO_LARRAY       = "Expected \"[\"";
char *TobyLanguage::NOT_WHOLE_NUM   = "Expected whole number";

char *TobyLanguage::LANGIOEXCEPT    = "LANGUAGE: IOException reading"
                                      " language data.";
char *TobyLanguage::LANGBOGUSLINE   = "LANGUAGE: Line %0 is bogus in %1.";
char *TobyLanguage::LANGNOSUCHCLASS = "LANGUAGE: No such class [%0] on"
                                      " line %1 in %2.";
char *TobyLanguage::LANGNOSUCHFIELD = "LANGUAGE: No such field [%0] on"
                                      " line %1 in %2.";
char *TobyLanguage::LANGNOTPUBLIC   = "LANGUAGE: Field [%0] is not"
                                      " on line %1 in %2.";
char *TobyLanguage::LANGNOTSTATIC   = "LANGUAGE: Field [%0] is not"
                                      " on line %1 in %2.";
char *TobyLanguage::LANGNOTSTRING   = "LANGUAGE: Field [%0] is not"
                                      " a string on line %1 in %2.";
char *TobyLanguage::LANGISFINAL     = "LANGUAGE: Field [%0] is final"
                                      " on line %1 in %2.";
char *TobyLanguage::LANGSETFAILED   = "LANGUAGE: Setting field [%0] "
                                      " failed with message [%1]"
                                      " on line %2 in %3.";

// end of TobyLanguage.cpp ...

