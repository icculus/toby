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

package last.toby.parsers.toby;

import java.io.*;
import java.util.*;
import last.toby.parsers.*;
import last.toby.util.*;
import last.toby.exceptions.*;
import last.toby.interpreter.*;

    // !!! lots of comments missing from functions near bottom.
    // !!! lots of comments will need a rewrite, too.

    /**
     *  This class contains individual TOBY procedures. All parsing of a given
     *   procedure is done here. Once all the TobyProcedures are created (and
     *   therefore all the program's code has been parsed for bogus syntax),
     *   the TobyInterpreter class will perform the actual execution of the
     *   procedures, starting with the TobyProcedure named "main".
     *
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public final class TobyParser extends last.toby.parsers.Parser
{
        // Constants representing TOBY operators...
    public final static String OPER_LPAREN     = "(";
    public final static String OPER_RPAREN     = ")";
    public final static String OPER_SEPARATOR  = ",";
    public final static String OPER_ASSIGNMENT = "=";
    public final static String OPER_EQUALS     = "==";
    public final static String OPER_MULTIPLY   = "*";
    public final static String OPER_DIVIDE     = "/";
    public final static String OPER_MODULO     = "%";
    public final static String OPER_ADD        = "+";
    public final static String OPER_SUBTRACT   = "-";
    public final static String OPER_GREATER    = ">";
    public final static String OPER_LESS       = "<";
    public final static String OPER_GREATEREQL = ">=";
    public final static String OPER_LESSEQL    = "<=";

       // Constants representing intrinsic data types...
    public final static String KEYWORD_NUMBER  = "number";
    public final static String KEYWORD_BOOLEAN = "boolean";
    public final static String KEYWORD_NOTHING = "nothing";

       // Constants representing general TOBY keywords...
    public final static String KEYWORD_IF          = "if";
    public final static String KEYWORD_ELSEIF      = "elseif";
    public final static String KEYWORD_ELSE        = "else";
    public final static String KEYWORD_ENDIF       = "endif";
    public final static String KEYWORD_BEGINFOR    = "for";
    public final static String KEYWORD_TO          = "to";
    public final static String KEYWORD_DOWNTO      = "downto";
    public final static String KEYWORD_STEP        = "step";
    public final static String KEYWORD_ENDFOR      = "endfor";
    public final static String KEYWORD_RETURN      = "return";
    public final static String KEYWORD_BEGINFUNC   = "function";
    public final static String KEYWORD_ENDFUNC     = "endfunction";
    public final static String KEYWORD_TRUE        = "true";
    public final static String KEYWORD_FALSE       = "false";
    public final static String KEYWORD_RETURNS     = "returns";
    public final static String KEYWORD_BEGINWHILE  = "while";
    public final static String KEYWORD_ENDWHILE    = "endwhile";
    public final static String KEYWORD_MAINLINE    = "main";

       // Constants representing standard TOBY function names...
    public final static String PROCNAME_ADDTURTLE      = "addturtle";
    public final static String PROCNAME_USETURTLE      = "useturtle";
    public final static String PROCNAME_REMTURTLE      = "removeturtle";
    public final static String PROCNAME_REMALLTURTLES  = "removeallturtles";
    public final static String PROCNAME_HIDETURTLE     = "hideturtle";
    public final static String PROCNAME_SHOWTURTLE     = "showturtle";
    public final static String PROCNAME_HIDEALLTURTLES = "hideallturtles";
    public final static String PROCNAME_SHOWALLTURTLES = "showallturtles";
    public final static String PROCNAME_FORWARD        = "goforward";
    public final static String PROCNAME_BACKWARD       = "gobackward";
    public final static String PROCNAME_RIGHT          = "turnright";
    public final static String PROCNAME_LEFT           = "turnleft";
    public final static String PROCNAME_SETPEN         = "setpencolor";
    public final static String PROCNAME_SETPENRGB      = "setpencolorrgb";
    public final static String PROCNAME_PENUP          = "setpenup";
    public final static String PROCNAME_PENDOWN        = "setpendown";
    public final static String PROCNAME_ISPENUP        = "ispenup";
    public final static String PROCNAME_ISPENDOWN      = "ispendown";
    public final static String PROCNAME_RANDOM         = "random";
    public final static String PROCNAME_ROUND          = "round";
    public final static String PROCNAME_TSPACEHIGH     = "getturtlespaceheight";
    public final static String PROCNAME_TSPACEWIDE     = "getturtlespacewidth";
    public final static String PROCNAME_STOP           = "stopprogram";
    public final static String PROCNAME_SETANGLE       = "setangle";
    public final static String PROCNAME_GETANGLE       = "getangle";
    public final static String PROCNAME_PAUSE          = "pause";
    public final static String PROCNAME_GETTURTLEX     = "getturtlex";
    public final static String PROCNAME_GETTURTLEY     = "getturtley";
    public final static String PROCNAME_SETTURTXY      = "setturtlexy";
    public final static String PROCNAME_HOMETURTLE     = "hometurtle";
    public final static String PROCNAME_HOMEALLTURTLES = "homeallturtles";
    public final static String PROCNAME_CLEANUP        = "cleanupturtlespace";
    public final static String PROCNAME_ENABLEFENCE    = "enablefence";
    public final static String PROCNAME_DISABLEFENCE   = "disablefence";
    public final static String PROCNAME_SETFENCE       = "setfence";

        // Static variables...
    protected static String[] operatorTable  = null;
    protected static String[] intrinsicTable = null;
    protected static String[] keywordTable = null;
    protected static String[] stdFuncTable = null;

        // Instance variables...
    private TobyTokenizer toker = null;
    private GlobalLogicContext global = null;
    private boolean allowVarDeclaration = true;

    public TobyParser()
    {
        buildTables();
    } // Constructor

    public static boolean isTobyDigit(char ch)
    {
        boolean retVal = false;

        if ((ch >= '0') && (ch <= '9'))
            retVal = true;

//        _D("isTobyDigit", "char ('" + ch + "') returns {" + retVal + "}.");

        return(retVal);
    } // isTobyDigit

    public static boolean isTobyLetter(char ch)
    {
        boolean retVal = false;

        if ( ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) )
            retVal = true;

//        _D("isTobyLetter", "char ('" + ch + "') returns {" + retVal + "}.");

        return(retVal);
    } // isTobyLetter

    private synchronized static void buildTables()
    {
        if (operatorTable == null)
            buildOperatorTable();

        if (intrinsicTable == null)
            buildIntrinsicTable();

        if (keywordTable == null)
            buildKeywordTable();

        if (stdFuncTable == null)
            buildStdFuncTable();
    } // buildTables


    public static String staticKeywordReference(String token)
    /**
     * To speed up interpretation, the parse phase replaces the individual
     *  tokens with references to our static keywords. For example, if
     *  a token of "endfunction" is found, we'll replace that token with
     *  KEYWORD_ENDFUNC, so that during interpretation, we need not do
     *  character-by-character compares on tokens, but just check if their
     *  references are equal. This is much faster, although it requires
     *  slightly more overhead at parse time (since we're effectively
     *  moving all the string comparisons to here...)
     *
     *     params : token == token to evaluate.
     *    returns : reference to a static String (from TobyInterpreter), or
     *               (token) itself, if it isn't in our token tables.
     */
    {
        int rc;

        rc = searchArray(operatorTable, token);
        if (rc != -1)
            return(operatorTable[rc]);

        rc = searchAlphabeticArray(keywordTable, token);
        if (rc != -1)
            return(keywordTable[rc]);

        rc = searchAlphabeticArray(stdFuncTable, token);
        if (rc != -1)
            return(stdFuncTable[rc]);

        rc = searchAlphabeticArray(intrinsicTable, token);
        if (rc != -1)
            return(intrinsicTable[rc]);

        return(token);
    } // staticKeywordReference



    private static void buildOperatorTable()
    /**
     *  Creates the operator table, an array of Strings, in order of
     *   precedence, of TOBY operators, such as "+", "-", and "*"...
     *
     *     params : void.
     *    returns : void.
     */
    {
        operatorTable = new String[] {
                                         OPER_SEPARATOR,
                                         OPER_LESS,
                                         OPER_GREATER,
                                         OPER_LESSEQL,
                                         OPER_GREATEREQL,
                                         OPER_EQUALS,
                                         OPER_ASSIGNMENT,
                                         OPER_SUBTRACT,
                                         OPER_ADD,
                                         OPER_MODULO,
                                         OPER_DIVIDE,
                                         OPER_MULTIPLY,
                                         OPER_RPAREN,
                                         OPER_LPAREN
                                     };
     } // buildOperatorTable



    private static void buildIntrinsicTable()
    /**
     *  Creates the intrinsic table, an array of Strings of
     *   keywords representing various intrinsic data types, like
     *   numbers and strings, etc...
     *
     *  THIS ARRAY MUST BE IN ALPHABETIC ORDER!
     *
     *     params : void.
     *    returns : void.
     */
    {
        intrinsicTable = new String[] {
                                          KEYWORD_BOOLEAN,
                                          KEYWORD_NOTHING,
                                          KEYWORD_NUMBER
                                      };

        alphabetizeArrayElements(intrinsicTable);
    } // buildIntrinsicTable



    private static void buildKeywordTable()
    /**
     *  Creates the keyword table, an array of Strings of
     *   keywords representing miscellaneous reserved words.
     *
     *       THIS ARRAY MUST BE IN ALPHABETIC ORDER!
     *
     *     params : void.
     *    returns : void.
     */
    {
        keywordTable = new String[] {
                                        KEYWORD_FALSE,
                                        KEYWORD_BEGINFUNC,
                                        KEYWORD_ELSE,
                                        KEYWORD_ENDFUNC,
                                        KEYWORD_BEGINFOR,
                                        KEYWORD_ENDIF,
                                        KEYWORD_ENDFOR,
                                        KEYWORD_RETURNS,
                                        KEYWORD_ENDWHILE,
                                        KEYWORD_ELSEIF,
                                        KEYWORD_IF,
                                        KEYWORD_RETURN,
                                        KEYWORD_STEP,
                                        KEYWORD_TO,
                                        KEYWORD_DOWNTO,
                                        KEYWORD_TRUE,
                                        KEYWORD_BEGINWHILE
                                    };

        alphabetizeArrayElements(keywordTable);
    } // buildKeywordTable


    private static void buildStdFuncTable()
    /**
     *  Creates the standard functions table, an array of Strings
     *   representing built-in TOBY function names.
     *
     *      THIS ARRAY MUST BE IN ALPHABETIC ORDER!
     *
     *     params : void.
     *    returns : void.
     */
    {
        stdFuncTable = new String[] {
                                        PROCNAME_BACKWARD,
                                        PROCNAME_FORWARD,
                                        PROCNAME_CLEANUP,
                                        PROCNAME_SHOWTURTLE,
                                        PROCNAME_HIDETURTLE,                    
                                        PROCNAME_SHOWALLTURTLES,
                                        PROCNAME_HIDEALLTURTLES,
                                        PROCNAME_GETANGLE,
                                        PROCNAME_SETPEN,
                                        PROCNAME_SETPENRGB,
                                        PROCNAME_GETTURTLEX,
                                        PROCNAME_GETTURTLEY,
                                        PROCNAME_SETTURTXY,
                                        PROCNAME_SETANGLE,
                                        PROCNAME_HOMETURTLE,
                                        PROCNAME_ISPENUP,
                                        PROCNAME_ISPENDOWN,
                                        PROCNAME_LEFT,
                                        PROCNAME_RIGHT,
                                        PROCNAME_PAUSE,
                                        PROCNAME_PENDOWN,
                                        PROCNAME_PENUP,
                                        PROCNAME_RANDOM,
                                        PROCNAME_ROUND,
                                        PROCNAME_STOP,
                                        PROCNAME_TSPACEHIGH,
                                        PROCNAME_TSPACEWIDE,
                                        PROCNAME_ADDTURTLE,
                                        PROCNAME_USETURTLE,
                                        PROCNAME_REMTURTLE,
                                        PROCNAME_REMALLTURTLES,
                                        PROCNAME_HOMEALLTURTLES,
                                        PROCNAME_SETFENCE,
                                        PROCNAME_ENABLEFENCE,
                                        PROCNAME_DISABLEFENCE
                                    };

        alphabetizeArrayElements(stdFuncTable);
    } // buildStdFuncTable


    public static boolean isReservedTobyWord(String str)
    {
        boolean retVal = false;

        if (searchArray(operatorTable, str) != -1)
            retVal = true;
        else if (searchAlphabeticArray(intrinsicTable, str) != -1)
            retVal = true;
        else if (searchAlphabeticArray(keywordTable, str) != -1)
            retVal = true;
        else if (searchAlphabeticArray(stdFuncTable, str) != -1)
            retVal = true;

        return(retVal);
    } // isReservedTobyWord


    public static boolean isIntrinsic(String str)
    {
        return((searchAlphabeticArray(intrinsicTable, str) != -1) ?
                                        true : false);
    } // isIntrinsic


    public static boolean isOperator(String str)
    {
        return((searchArray(operatorTable, str) != -1) ? true : false);
    } // isIntrinsic



        /**
         *  From last.toby.parsers.Parser:
         *   Build procedures and parse code.
         *
         *  @param code Reader that contains source code to parse.
         * @return GlobalLogicContext of procedures, etc defined in source.
         *  @throw ParseException on syntax errors, etc...
         */
    public GlobalLogicContext parseCodeImpl(Reader code) throws ParseException
    {
        try
        {
            try
            {
                this.toker = new TobyTokenizer(code);
                this.global = new GlobalLogicContext(KEYWORD_MAINLINE);

                this.global.setCaseSensitive(false);
                this.toker.setReferenceReplacePolicy(true);
                this.allowVarDeclaration = true;  // for global context.

                parseSourceLines(this.global);
            } // try
            catch (IOException ioe)
            {
                // !!! this sucks.
                ParseException._throw(TobyLanguage.INTERNAL);
            } // catch
        } // try

        catch (ParseException pe)
        {
            if (pe.exceptionLine == pe.LINENUM_UNDEFINED)
                pe.exceptionLine = this.toker.lineno();

            global.freeResources(true);
            global = null;
            throw(pe);
        } // catch

        finally
        {
            try
            {
                toker.close();
            } // try
            catch (IOException ioe)
            {
                // don't care. We tried. (*shrug*)
            } // catch

            toker = null;
        } // finally

        return(global);
    } // Constructor


    private Intrinsic buildVarReference(LogicContext parent, String value)
                                            throws ParseException
    {
        if (isReservedTobyWord(value))
            ParseException._throw(TobyLanguage.NOT_VAR);

        if (isValidTobyIdentifier(value) == false)
            ParseException._throw(TobyLanguage.BAD_IDENT);

        return(new VarReferenceIntrinsic(value, parent, toker.lineno()));
    } // buildVarReference

    public static boolean isValidTobyNumber(String token)
    {
//        _D("isValidTobyNumber", "checking [" + token + "].");

        boolean retVal = true;

        try
        {
            double rc = Double.parseDouble(token);
        } // try
        catch (NumberFormatException nfe)
        {
            retVal = false;  // not a valid number.
        } // catch

        return(retVal);
    } // isValidTobyNumber

    public static boolean isValidTobyIdentifier(String identifier)
    {
//        _D("isValidTobyIdentifier", "checking [" + identifier + "].");

        int max = identifier.length();

        if (max == 0)
            return(false);

        if (isTobyLetter(identifier.charAt(0)) == false)
            return(false);

        for (int i = 1; i < max; i++)
        {
            char ch = identifier.charAt(i);
            if ((isTobyDigit(ch) == false) && (isTobyLetter(ch) == false))
                return(false);
        } // for

        return(true);
    } // isValidTobyIdentifer

    private NumberIntrinsic buildNumberIntrinsic(String value)
    {
        double rc;


        try
        {
            rc = Double.parseDouble(value);
//            _D("buildNumberIntrinsic", "value [" + value + "] is" +
//               " (" + rc + ").");
        } // try
        catch (NumberFormatException nfe)
        {
//            _D("buildNumberIntrinsic", "value [" + value + "] is NaN.");
           return(null);
        } // catch

        return(new NumberIntrinsic(rc));
    } // buildNumberIntrinsic


    private Intrinsic buildBooleanIntrinsic(String value)
    {
        boolean boolVal;

        if (value == KEYWORD_FALSE)
            boolVal = false;
        else if (value == KEYWORD_TRUE)
            boolVal = true;
        else
        {
//            _D("buildBooleanIntrinsic", "value [" +value+ "] is NOT boolean.");
            return(null);
        } // else

//        _D("buildBooleanIntrinsic", "value [" + value + "] is boolean.");

        return(new BooleanIntrinsic(boolVal));
    } // buildBooleanIntrinsic


    private Intrinsic buildIntrinsicFromValue(LogicContext parent, String val)
                                                throws ParseException
    {
        Intrinsic retVal = null;

        if (val.length() <= 0)
            ParseException._throw(TobyLanguage.INTERNAL);

        retVal = buildNumberIntrinsic(val);
        if (retVal != null)
            return(retVal);

        retVal = buildBooleanIntrinsic(val);
        if (retVal != null)
            return(retVal);

            // This probably needs to be last.
        retVal = buildVarReference(parent, val);
        if (retVal != null)
            return(retVal);

        ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!
        return(null);
    } // buildIntrinsicFromValue


    private Intrinsic buildIntrinsicFromType(String type) throws ParseException
    {
        Intrinsic retVal = null;

        if (type == KEYWORD_NUMBER)
            retVal = new NumberIntrinsic();
        else if (type == KEYWORD_BOOLEAN)
            retVal = new BooleanIntrinsic();
        else
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        return(retVal);
    } // buildIntrinsicFromType


    private void parseSourceLines(LogicContext parent) throws ParseException,
                                                              IOException
    {
        boolean getOut = false;

        do
        {
            toker.removeBlankLines();

            if (toker.nextToken() == toker.TT_EOF)
            {
                if ((parent instanceof GlobalLogicContext) == false)
                    ParseException._throw(TobyLanguage.EXPECTED_END);
                else
                    getOut = true;
            } // if

            else
            {
                String instruction = toker.sval;

//                _D("parseSourceLines", "token is ["+instruction+"].");

                if (isIntrinsic(instruction))
                    getOut = parseVarDeclPermitAssign(instruction, parent);

                else if (instruction == KEYWORD_BEGINFUNC)
                {
                    this.allowVarDeclaration = true;
                    getOut = parseBeginFunction(parent);
                } // else if
                else
                {
                    this.allowVarDeclaration = false;

                    if (instruction == KEYWORD_BEGINFOR)
                        getOut = parseFor(parent);

                    else if (instruction == KEYWORD_ENDFOR)
                        getOut = parseEndFor(parent);

                    else if (instruction == KEYWORD_ENDFUNC)
                        getOut = parseEndFunction(parent);

                    else if (instruction == KEYWORD_BEGINWHILE)
                        getOut = parseWhile(parent);

                    else if (instruction == KEYWORD_ENDWHILE)
                        getOut = parseEndWhile(parent);

                    else if (instruction == KEYWORD_RETURN)
                        getOut = parseReturn(parent);

                    else if (instruction == KEYWORD_IF)
                        getOut = parseIf(parent);

                    else if (instruction == KEYWORD_ELSEIF)
                        getOut = parseElseIf(parent);

                    else if (instruction == KEYWORD_ELSE)
                        getOut = parseElse(parent);

                    else if (instruction == KEYWORD_ENDIF)
                        getOut = parseEndIf(parent);

                    else
                    {
                        toker.pushBack();
                        getOut = parseExpression(parent);
                    } // else
                } // else
            } // else
        } while (getOut == false);
    } // parseSourceLines



        /**
         *  In programming language lingo, all functions consist of a
         *   DECLARATION and a DEFINITION.
         *
         *  For example, in Java, the declaration is the header that tells
         *   what the function is called, what parameters it takes, and
         *   what it returns. Other attributes, like what it throws, if it is
         *   static, final, etc...are DECLARED at that point in the program.
         *
         *  The definition is the actual source code that makes up the body of
         *   the function. The algorithm is DEFINED at that point in the
         *   program.
         *
         *        public void moveTo(int x, int y)      // <-- declaration.
         *        {                                     // <-\
         *            moreCode(y, x + 10);              // <  --- definition.
         *        } // moveTo                           // <-/
         *                 !!! update javadocs, below.
         *    @param toker This tokenizer should be just past the
         *                 KEYWORD_FUNC token, leaving us at the
         *                 function name.
         *    @throw ParseException on panic.
         */
    private boolean parseBeginFunction(LogicContext parent) throws ParseException,
                                                                IOException
    {
        if ((parent instanceof GlobalLogicContext) == false)
            ParseException._throw(TobyLanguage.FUNC_IN_FUNC);

        String nameToken = parseFunctionName();
        FunctionLogicContext context;

        context = new FunctionLogicContext(toker.lineno(), nameToken);

        try
        {
            parseDeclaration(context);

//            _D("parseBeginFunction", "is [" + nameToken + "] double defined?");
            if (((GlobalLogicContext) parent).findFunction(nameToken) != null)
                ParseException._throw(TobyLanguage.DOUBLE_DEF);
//            _D("parseBeginFunction", "survived double definition check.");

            parent.addChild(context);
            parseSourceLines(context);
        } // try
        catch (ParseException pe)
        {
                // Add the procedure name to error details...
            pe.procName = nameToken;
            throw(pe);
        } // catch

        return(false);
    } // parseBeginFunction

        /**
         * Assuming that (toker) should be staring directly at the function
         *  name in the declaration, this method will make sure all is
         *  copacetic and return the function name.
         *
         *   @return String containing new function's name.
         *    @throw ParseException on syntax errors.
         *    @throw IOException on Tokenizer read failure.
         */
    private String parseFunctionName() throws ParseException, IOException
    {
        String nameToken = toker.getWord();

//        _D("parseFunctionName", "name is [" + nameToken + "].");

            // We check for main() separately, since it IS valid, but it
            //  will be considered to be reserved by isValidTobyIdentifier().
        if (nameToken != KEYWORD_MAINLINE)
        {
            if (isValidTobyIdentifier(nameToken) == false)
                ParseException._throw(TobyLanguage.BAD_IDENT);
        } // if

        return(nameToken);
    } // parseFunctionName


        /**
         *  This method parses the TOBY function declaration into
         *   something this class can deal with.
         *
         *    @throw ParseException on syntax errors.
         *    @throw IOException on Tokenizer read failure.
         */
    private void parseDeclaration(FunctionLogicContext context)
                                     throws ParseException, IOException
    {
        parseParameters(context);
        parseReturnType(context);
        toker.verifyEndLine();
    } // parseDeclaration


        /**
         *   This method is called after parseFunctionName(), and (toker)
         *    should return a '(' as the next token. This will be the start
         *    of this TOBY function's parameter list. Syntax is as follows:
         *   
         * function myFunc(varType varName1, varType varNameN) returns varType
         *
         *   If the parameter list is void (has no parameters) then the syntax
         *    is:
         *
         * function myFunc() returns varType
         *
         *   Have fun.
         *
         *    @param context FunctionLogicContext we are setting up.
         *    @throw ParseException on syntax errors.
         *    @throw IOException on Tokenizer read failure.
         */
    private void parseParameters(FunctionLogicContext context)
                                        throws ParseException, IOException
    {
        if (toker.getWord() != OPER_LPAREN)
            ParseException._throw(TobyLanguage.NO_LPAREN);

            // are there actually any params?
        if (toker.getWord() != OPER_RPAREN)
        {
            toker.pushBack();
            paramParseLoop(context);
        } // if
    } // parseParameters


        /**
         * This method is called if the parameters declaration is not void.
         *
         *    @throw ParseException on syntax errors.
         *    @throw IOException on Tokenizer read failure.
         */
    private void paramParseLoop(FunctionLogicContext context)
                                     throws ParseException, IOException
    {
        boolean getOut = false;
        String token;
        String paramType;
        String paramName;

        do
        {
            paramType = toker.getWord();
            if (isIntrinsic(paramType) == false)
                ParseException._throw(TobyLanguage.BAD_TYPE);

            parseVarDeclaration(paramType, context, true);
            token = toker.getWord();
            if (token == OPER_RPAREN)
                getOut = true;
            else if (token != OPER_SEPARATOR)
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);
        } while (!getOut);
    } // paramParseLoop


        /**
         * Following the parameter declaration, a function declaration
         *  must have a return type. This is declared as "returns varType",
         *  where varType may be "number", "boolean", "nothing" (void), etc...
         * This function calculates the return type for this Toby procedure.
         *
         *    @throw ParseException on syntax errors.
         *    @throw IOException on Tokenizer read failure.
         */
    private void parseReturnType(FunctionLogicContext context)
                                            throws ParseException, IOException
    {
        if (toker.getWord() != KEYWORD_RETURNS)
            ParseException._throw(TobyLanguage.NO_RETTYPE);

        String retType = toker.getWord();

// !!!
//        if (isIntrinsic(retType) == true)
//            context.setRetType(retType);
//        else


        if (isIntrinsic(retType) == false)
            ParseException._throw(TobyLanguage.BAD_TYPE);
    } // parseReturnType


    private boolean parseEndFunction(LogicContext parent) throws ParseException,
                                                                 IOException
    {
        if ((parent instanceof FunctionLogicContext) == false)
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        toker.verifyEndLine();  // throws ParseException if not EOL.

        return(true);
    } // parseEndFunction


    private String parseVarDeclaration(String type, LogicContext context,
                                        boolean isParam)
                                            throws ParseException, IOException
    {
            // This is a Toby-enforced limitation. Var declares only allowed
            //  in the Global and Function scopes.
        if ( ((context instanceof FunctionLogicContext) == false) &&
             ((context instanceof GlobalLogicContext) == false) )
            ParseException._throw(TobyLanguage.NO_VAR_DECL);

            // Toby-enforced limitation: var declares must be first in context.
        if (this.allowVarDeclaration == false)
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        String identifier = toker.getWord();
        LogicContext previousDecl = context.checkScopesForSymbol(identifier);

            // this check is only sufficient since we limit var declares to
            //  functions and globals.
        if (previousDecl == context)
            ParseException._throw(TobyLanguage.DOUBLE_DEF);

        Intrinsic intr = buildIntrinsicFromType(type);

        if (context instanceof GlobalLogicContext)
        {
//            _D("parseVarDecl", "adding Global var. type [" + type +
//                               "], name [" + identifier + "].");
            ((GlobalLogicContext) context).addGlobal(type, identifier, intr);
        } // if
        else
        {
            FunctionLogicContext fc = (FunctionLogicContext) context;

            // !!! dunno if I like this. Can we move this
            // !!!  isParam stuff elsewhere?
            if (isParam)
                fc.addParameter(type, identifier, intr);
            else
                fc.addLocal(type, identifier, intr);
        } // else

        return(identifier);
    } // parseVarDeclaration


    private boolean parseVarDeclPermitAssign(String type, LogicContext context)
                                            throws ParseException, IOException
    {
        String identifier = parseVarDeclaration(type, context, false);

            // !!! temporary Toby limitation.
//        if (context instanceof GlobalLogicContext)
//            ParseException._throw(TobyLanguage.SYNTAX_ERROR);

        if (toker.nextToken() != toker.TT_WORD)
            toker.pushBack();
        else
        {
            if (toker.sval != OPER_ASSIGNMENT)
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);

            Vector exprVect = toker.vectorToEndOfLine();
            if ((exprVect == null) || (exprVect.size() == 0))
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

            exprVect.insertElementAt(identifier, 0);  // put var in expr.
            exprVect.insertElementAt(OPER_ASSIGNMENT, 1);
            ExpressionLogicContext expr = buildExpression(exprVect, context);
            context.addChild(expr);
        } // if

        toker.verifyEndLine();
        return(false);
    } // parseVarDeclPermitAssign


    private boolean parseFor(LogicContext parent) throws ParseException,
                                                         IOException
    {
        ForLoopLogicContext context;
        String identifier;
        int lineNum = toker.lineno();
        String delims[] = {KEYWORD_TO, KEYWORD_DOWNTO};

        Vector vect = toker.vectorToTokenDelimiters(delims);
        if ((vect == null) || (vect.size() == 0))
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        identifier = (String) vect.elementAt(0);

        if (vect.elementAt(1) != OPER_ASSIGNMENT)
            ParseException._throw(TobyLanguage.NO_ASSIGN);

        ExpressionLogicContext startExpr = buildExpression(vect, parent);

            // get KEYWORD_TO/KEYWORD_DOWNTO...
        String upOrDown = toker.getWord();
        vect = toker.vectorToDelimiterOrEnd(KEYWORD_STEP);
        vect.insertElementAt(identifier, 0);
        if (upOrDown == KEYWORD_TO)
            vect.insertElementAt(OPER_LESSEQL, 1);
        else
            vect.insertElementAt(OPER_GREATEREQL, 1);
        ExpressionLogicContext endExpr = buildExpression(vect, parent);

        vect.clear();
        ExpressionLogicContext stepExpr = null;
        toker.nextToken();
        if (toker.ttype == toker.TT_WORD)
        {
            if (toker.sval != KEYWORD_STEP)
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!
            else
                vect = toker.vectorToEndOfLine();
        } // if
        else if (toker.ttype == toker.TT_EOL)  // no STEP?
            vect.insertElementAt(upOrDown == KEYWORD_TO ? "1.0" : "-1.0", 0);
        else
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        vect.insertElementAt(identifier, 0);
        vect.insertElementAt(OPER_ASSIGNMENT, 1);
        vect.insertElementAt(identifier, 2);
        vect.insertElementAt(OPER_ADD, 3);
        stepExpr = buildExpression(vect, parent);

        context = new ForLoopLogicContext(lineNum, startExpr,
                                          endExpr, stepExpr);

        parent.addChild(context);
        parseSourceLines(context);
        return(false);
    } // parseFor


    private boolean parseEndFor(LogicContext parent) throws ParseException,
                                                            IOException
    {
        if ((parent instanceof ForLoopLogicContext) == false)
            ParseException._throw(TobyLanguage.NO_FOR);

        toker.verifyEndLine();  // throws ParseException if not EOL.

        return(true);
    } // parseEndFor


    private boolean parseWhile(LogicContext parent) throws ParseException,
                                                           IOException
    {
        WhileLoopLogicContext context;
        int lineNum = toker.lineno();
        Vector v = toker.vectorToEndOfLine();
        ExpressionLogicContext expr = buildExpression(v, parent);

        context = new WhileLoopLogicContext(lineNum, null, expr, null);
        parent.addChild(context);
        parseSourceLines(context);
        return(false);
    } // parseWhile


    private boolean parseEndWhile(LogicContext parent) throws ParseException,
                                                              IOException
    {
        if ((parent instanceof WhileLoopLogicContext) == false)
            ParseException._throw(TobyLanguage.SYNTAX_ERROR);  // !!!

        toker.verifyEndLine();  // throws ParseException if not EOL.

        return(true);
    } // parseEndWhile


    private boolean parseReturn(LogicContext parent) throws ParseException,
                                                            IOException
    {
        Vector v = toker.vectorToEndOfLine();
        ExpressionLogicContext elc = buildExpression(v, parent);
        ReturnLogicContext rlc = new ReturnLogicContext(toker.lineno(), elc);
        parent.addChild(rlc);
        return(false);
    } // parseReturn


    private boolean parseIf(LogicContext parent) throws ParseException,
                                                        IOException
    {
        int lineNum = toker.lineno();
        ConditionalLogicContext context;
        ExpressionLogicContext condExpr;

        condExpr = buildExpression(toker.vectorToEndOfLine(), parent);
        context = new ConditionalLogicContext(lineNum, condExpr);
        parent.addChild(context);
        parseSourceLines(context);
        return(false);
    } // parseIf


    private boolean parseElseIf(LogicContext parent) throws ParseException,
                                                            IOException
    {
        if ((parent instanceof ConditionalLogicContext) == false)
            ParseException._throw(TobyLanguage.ORPHAN_ELIF);

        ElseLogicContext elc = new ElseLogicContext(toker.lineno());
        ((ConditionalLogicContext) parent).setElse(elc);

        parseIf(elc);

        return(true);
    } // parseElseIf


    private boolean parseElse(LogicContext parent) throws ParseException,
                                                          IOException
    {
        if ((parent instanceof ConditionalLogicContext) == false)
            ParseException._throw(TobyLanguage.ORPHAN_ELSE);

        ElseLogicContext elc = new ElseLogicContext(toker.lineno());
        ((ConditionalLogicContext) parent).setElse(elc);

        parseSourceLines(elc);
        return(true);
    } // parseElse


    private boolean parseEndIf(LogicContext parent) throws ParseException,
                                                           IOException
    {
        if ( ((parent instanceof ConditionalLogicContext) == false) &&
             ((parent instanceof ElseLogicContext) == false) )
            ParseException._throw(TobyLanguage.ORPHAN_ENDIF);

        toker.verifyEndLine();

        return(true);
    } // parseEndIf


    private boolean parseExpression(LogicContext parent) throws ParseException,
                                                                IOException
    {
        Vector v = toker.vectorToEndOfLine();
        ExpressionLogicContext rc = buildExpression(v, parent);
        parent.addChild(rc);
        return(false);
    } // parseExpression


    private ExpressionLogicContext buildExpression(Vector tokens,
                                                   LogicContext parent)
                                                       throws ParseException
    {
//            StringBuffer sb = new StringBuffer();
//            sb.append("Called with tokens:");
//            for (int i = 0; i < tokens.size(); i++)
//            {
//                sb.append(" [");
//                sb.append(tokens.elementAt(i).toString());
//                sb.append("]");
//            } // for
//
//            _D("buildExpression", sb.toString());
//            sb.setLength(0);

        ExpressionLogicContext thisNode;
        int index;
        ExpressionLogicContext retVal = null;
        boolean getOut = false;

        do
        {
            thisNode = null;
            index = findHighestPrecedence(tokens);
            if (index < 0)  // all operators are gone.
            {
                getOut = true;
                thisNode = parseExpressionWithoutOps(tokens, parent);
            } // if
            else
            {
                if (tokens.elementAt(index) == OPER_LPAREN)
                {
                    thisNode = parseParentheses(tokens, index, parent);
                        // a bit hacky. !!!
                    if (thisNode instanceof FunctionCallExprLogicContext)
                    {
                        tokens.insertElementAt(thisNode, index - 1);
                        thisNode = null;
                    } // if
                } // if
                else
                    thisNode = parseBinaryOperator(tokens, index, parent);
            } // else

            if (thisNode != null)
                retVal = thisNode;
        } while (getOut == false);

        return(retVal);
    } // buildExpression


        /**
         * This method searches an expression for the operator with the
         *  highest precedence. This assures that mathematical equasions
         *  come up with accurate results. Please refer to
         *  buildOperatorTable() for a list of understood
         *  operators, and their precedence.
         *
         *     @param tokens Vector of tokens that make up an expression.
         *    @return index of highest precedence operator. -2 if no tokens
         *            (just NodeTrees), -1 if no operators (but tokens)
         *            were found.
         */
    private int findHighestPrecedence(Vector tokens) throws ParseException
    {
        int i;
        int deepest = 0;
        int depth = 0;
        int max = tokens.size();
        int retVal = -2;
        int highestPrecedence = -1;
        int rc;
        Object obj;

        for (i = 0; i < max; i++)
        {
            obj = tokens.elementAt(i);

            if (obj instanceof String)  // not a nodetree?
            {
                if (retVal == -2)   // Definitely at least a token in here.
                    retVal = -1;

                if (obj == OPER_LPAREN)
                    depth++;
                else if (obj == OPER_RPAREN)
                    depth--;

                rc = searchArray(operatorTable, (String) obj);
                if (rc >= 0)
                {
                    if ((rc >= highestPrecedence) && (depth >= deepest))
                    {
                        deepest = depth;
                        highestPrecedence = rc;
                        retVal = i;
                    } // if
                } // if
            } // if
        } // for

//            if (retVal == -2)
//                _D("findHighestPrecedence", "No tokens left.");
//            else if (retVal == -1)
//                _D("findHighestPrecedence", "No operators.");
//            else
//            {
//                _D("findHighestPrecedence", "Highest op is #" + retVal +
//                     ". ([" + ((String) tokens.elementAt(retVal)) + "]).");
//            } // else

        return(retVal);
    } // findHighestPrecedence


    private ExpressionLogicContext parseExpressionWithoutOps(Vector tokens,
                                                            LogicContext parent)
                                                        throws ParseException
    {
        ExpressionLogicContext retVal = null;
        int vectSize = tokens.size();

        if (vectSize > 0)
        {
            if (vectSize > 1)
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);

            Object obj = tokens.remove(0);
            if (obj instanceof ExpressionLogicContext)
                retVal = (ExpressionLogicContext) obj;
            else
            {
                retVal = new IntrinsicLogicContext(toker.lineno(),
                             buildIntrinsicFromValue(parent, (String) obj));
            } // else
        } // if

        return(retVal);
    } // parseExpressionWithoutOps


    private ExpressionLogicContext parseBinaryOperator(Vector tokens,
                                                       int index,
                                                       LogicContext parent)
                                                         throws ParseException
    {
        ExpressionLogicContext retVal = null;

//        _D("parseBinaryOperator", "index is (" + index + ").");

        if ((index <= 0) || (index >= tokens.size() - 1))
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

        int lineNum = toker.lineno();
        Object rvalue = tokens.remove(index + 1);
        String opStr = (String) tokens.remove(index);
        Object lvalue = tokens.remove(index - 1);

            // !!! move this to a separate function.
        if (opStr == OPER_MULTIPLY)
            retVal = new MultiplicationExpressionLogicContext(lineNum);
        else if (opStr == OPER_DIVIDE)
            retVal = new DivisionExpressionLogicContext(lineNum);
        else if (opStr == OPER_SUBTRACT)
            retVal = new SubtractionExpressionLogicContext(lineNum);
        else if (opStr == OPER_ADD)
            retVal = new AdditionExpressionLogicContext(lineNum);
        else if (opStr == OPER_MODULO)
            retVal = new ModuloExpressionLogicContext(lineNum);
        else if (opStr == OPER_ASSIGNMENT)
            retVal = new AssignmentExpressionLogicContext(lineNum);
        else if (opStr == OPER_EQUALS)
            retVal = new EqualityExpressionLogicContext(lineNum);
        else if (opStr == OPER_LESS)
            retVal = new LessThanExpressionLogicContext(lineNum);
        else if (opStr == OPER_LESSEQL)
            retVal = new LessThanEqualExpressionLogicContext(lineNum);
        else if (opStr == OPER_GREATER)
            retVal = new GreaterThanExpressionLogicContext(lineNum);
        else if (opStr == OPER_GREATEREQL)
            retVal = new GreaterThanEqualExpressionLogicContext(lineNum);
        else
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

        if ((lvalue instanceof LogicContext) == false)
        {
            lvalue = new IntrinsicLogicContext(lineNum,
                             buildIntrinsicFromValue(parent, (String) lvalue));
        } // if

        if ((rvalue instanceof LogicContext) == false)
        {
            rvalue = new IntrinsicLogicContext(lineNum,
                             buildIntrinsicFromValue(parent, (String) rvalue));
        } // if

        retVal.addChild((LogicContext) lvalue);
        retVal.addChild((LogicContext) rvalue);

        tokens.insertElementAt(retVal, index - 1);

        return(retVal);
    } // parseBinaryOperator


    private Vector moveElementsByDelimiter(Vector src, int index, String delim)
                                                throws ParseException
    {
        Vector retVal = new Vector();
        Object obj;
        boolean getOut = false;

        do
        {
            obj = src.remove(index);
            if (obj.equals(delim))
                getOut = true;
            else
                retVal.addElement(obj);
        } while (getOut == false);

        return(retVal);
    } // moveElementsByDelimiter


    private ExpressionLogicContext parseGroupedExpression(Vector tokens,
                                                          int index,
                                                          String delimiter,
                                                          LogicContext parent)
                                                          throws ParseException
    {
        Vector v;
        ExpressionLogicContext retVal;

//        _D("parseGroupedExpression", "called. index == (" + index + "). [" +
//                                    tokens.elementAt(index).toString() + "].");

        tokens.removeElementAt(index);
        v = moveElementsByDelimiter(tokens, index, delimiter);
        retVal = buildExpression(v, parent);
        tokens.insertElementAt(retVal, index);
        return(retVal);
    } // parseGroupedExpression


    private ExpressionLogicContext parseParentheses(Vector tokens, int index,
                                                    LogicContext parent)
                                                     throws ParseException
    {
        ExpressionLogicContext retVal = null;

                // Is this a function call or just an expression grouping?
        if (
            (index == 0) ||
            (isOperator((String) tokens.elementAt(index - 1)))
           )
        {
            retVal = parseGroupedExpression(tokens, index, OPER_RPAREN, parent);
        } // if
        else
        {
            retVal = parseFunctionCall(tokens, index, parent);
        } // else

        return(retVal);
    } // parseParentheses


            // !!! this function is pretty ugly...

    private ExpressionLogicContext parseFunctionCall(Vector tokens, int index,
                                                     LogicContext parent)
                                                      throws ParseException
    {
        Vector v = new Vector();
        FunctionCallExprLogicContext retVal;
        String str;
        Object obj;

//        _D("parseFunctionCall", "called.");

            // get function name, build logic context...
        index--;
        str = (String) tokens.remove(index);

//        _D("parseFunctionCall", "name of function to call is [" + str + "].");

            // standard function call?
        if (searchAlphabeticArray(stdFuncTable, str) != -1)
        {
//            _D("parseFunctionCall", "This is a standard function.");
            str = "__$STDFUNC$__" + str;
        } // if

        retVal = new FunctionCallExprLogicContext(toker.lineno(), str);

        tokens.remove(index);    // remove '(' ...

        if (tokens.elementAt(index) == OPER_RPAREN)  // Check for void args...
            tokens.remove(index);
        else
        {
            do
            {
                v.clear();
                do
                {
                    if ((tokens.size()) == index)
                        ParseException._throw(TobyLanguage.NO_RPAREN);

                    obj = tokens.remove(index);

                    if ((obj != OPER_SEPARATOR) && (obj != OPER_RPAREN))
                        v.addElement(obj);
                } while ((obj != OPER_SEPARATOR) && (obj != OPER_RPAREN));

                if (v.size() == 0)
                    ParseException._throw(TobyLanguage.SYNTAX_ERROR);

                ExpressionLogicContext elc = buildExpression(v, parent);
                retVal.addChild(elc);
            } while (obj != OPER_RPAREN);
        } // else

        return(retVal);
    } // parseFunctionCall

} // TobyProcedure

// end of TobyProcedure.java ...
