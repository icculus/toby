/**
 *  This class manages TOBY procedures and their output.
 *
 *  TobyInterpreter is actually derived from TurtleSpace, so
 *   that if (heaven forbid) another scripting language besides
 *   TOBY, or a different implementation therein, wants to manipulate
 *   TurtleSpace, it can. Just override paint(), and its abstract
 *   methods.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.io.*;
import java.util.*;
import java.awt.event.*;

import javax.swing.*;

public final class TobyInterpreter extends TurtleSpace implements Runnable,
                                                          ComponentListener
{
        // Constants...
    private final static int GRAPHICS_SCREEN = 0;
    private final static int GRAPHICS_COPY   = 1;

    public final static char TOBYCHAR_QUOTE   = '"';

        // Constants representing TOBY operators...
    public final static int    OPERATOR_COUNT    = 14;
    public final static String OPCODE_LPAREN     = "(";
    public final static String OPCODE_RPAREN     = ")";
    public final static String OPCODE_SEPARATOR  = ",";
    public final static String OPCODE_ASSIGNMENT = "=";
    public final static String OPCODE_EQUALS     = "==";
    public final static String OPCODE_MULTIPLY   = "*";
    public final static String OPCODE_DIVIDE     = "/";
    public final static String OPCODE_MODULO     = "%";
    public final static String OPCODE_ADD        = "+";
    public final static String OPCODE_SUBTRACT   = "-";
    public final static String OPCODE_GREATER    = ">";
    public final static String OPCODE_LESS       = "<";
    public final static String OPCODE_GREATEREQL = ">=";
    public final static String OPCODE_LESSEQL    = "<=";

       // Constants representing intrinsic data types...
    public final static int    INTRINSIC_COUNT   = 2;
    public final static String INTRINSIC_NUMBER  = "number";
    public final static String INTRINSIC_BOOLEAN = "boolean";

       // Constants representing general TOBY keywords...
    public final static int    KEYWORD_COUNT       = 14;
    public final static String KEYWORD_IF          = "if";
    public final static String KEYWORD_ELSEIF      = "elseif";
    public final static String KEYWORD_ELSE        = "else";
    public final static String KEYWORD_ENDIF       = "endif";
    public final static String KEYWORD_BEGINFOR    = "for";
    public final static String KEYWORD_TO          = "to";
    public final static String KEYWORD_ENDFOR      = "endfor";
    public final static String KEYWORD_RETURN      = "return";
    public final static String KEYWORD_BEGINFUNC   = "function";
    public final static String KEYWORD_ENDFUNC     = "endfunction";
    public final static String KEYWORD_TRUE        = "true";
    public final static String KEYWORD_FALSE       = "false";
    public final static String KEYWORD_RETURNS     = "returns";
    public final static String KEYWORD_NOTHING     = "nothing";

       // Constants representing standard TOBY function names...
    public final static int    STDFUNC_COUNT       = 16;
    public final static String PROCNAME_MAINLINE   = "main"; // not in table.
    public final static String PROCNAME_HIDETURTLE = "hideturtle";
    public final static String PROCNAME_SHOWTURTLE = "showturtle";
    public final static String PROCNAME_FORWARD    = "goforward";
    public final static String PROCNAME_BACKWARD   = "gobackward";
    public final static String PROCNAME_RIGHT      = "turnright";
    public final static String PROCNAME_LEFT       = "turnleft";
    public final static String PROCNAME_SETPENCOL  = "setpencolor";
    public final static String PROCNAME_GETPENCOL  = "getpencolor";
    public final static String PROCNAME_PENUP      = "setpenup";
    public final static String PROCNAME_PENDOWN    = "setpendown";
    // !!!
    //public final static String PROCNAME_ISPENUP    = "ispenup";
    //public final static String PROCNAME_ISPENDOWN  = "ispendown";
    //public final static String PROCNAME_RANDOM     = "random";
    //public final static String PROCNAME_TSPACEHIGH = "getturtlespaceheight";
    //public final static String PROCNAME_TSPACEWIDE = "getturtlespacewidth";
    // !!!
    public final static String PROCNAME_SETANGLE   = "setangle";
    public final static String PROCNAME_GETANGLE   = "getangle";
    public final static String PROCNAME_PAUSE      = "pause";
    public final static String PROCNAME_GETTURTLEX = "getturtlex";
    public final static String PROCNAME_GETTURTLEY = "getturtley";
    public final static String PROCNAME_SETTURTXY  = "setturtlexy";

        // Constants representing error messages...
    public final static String PROCERR_SYNTAX_ERR   = "Syntax error";
    public final static String PROCERR_DOUBLE_DEF   = "Duplicate Definition";
    public final static String PROCERR_ORPHAN_CODE  = "Code outside function";
    public final static String PROCERR_INTERNAL     = "Internal error";
    public final static String PROCERR_BAD_TYPE     = "Bad variable type";
    public final static String PROCERR_NOT_A_VAR    = "Expected variable";
    public final static String PROCERR_NO_MAINLINE  = "Expected mainline";
    public final static String PROCERR_NO_RPAREN    = "Expected \")\"";
    public final static String PROCERR_NO_LPAREN    = "Expected \"(\"";
    public final static String PROCERR_NOT_A_FUNC   = "Undefined function";
    public final static String PROCERR_BADNUM_ARGS  = "Wrong arguments";
    public final static String PROCERR_BAD_IDENT    = "Invalid identifier";
    public final static String PROCERR_NO_ENDFOR    = "FOR without ENDFOR";
    public final static String PROCERR_NO_FOR       = "ENDFOR without FOR";
    public final static String PROCERR_ORPHAN_ELIF  = "ELSEIF without IF";
    public final static String PROCERR_ORPHAN_ELSE  = "ELSE without IF";
    public final static String PROCERR_ORPHAN_ENDIF = "ENDIF without IF";
    public final static String PROCERR_TYPE_MMATCH  = "Type mismatch";

        // Static variables...
    protected static String[] operatorTable  = null;
    protected static String[] intrinsicTable = null;
    protected static String[] keywordTable = null;
    protected static String[] stdFuncTable = null;


        // Instance variables...
    private boolean codeRunning = false;
    private boolean terminateCode = false;
    private Vector procList;
    private Stack globals;
    private Stack stack;
    private Graphics[] g;
    private Image copyImage = null;
    private NothingIntrinsic nothing = null;

        // Methods...

    public TobyInterpreter()
    {
        super();

        setDoubleBuffered(false);

        addComponentListener(this);

        g = new Graphics[2];
        procList = new Vector();
        globals = new Stack();
        stack = new Stack();
        nothing = new NothingIntrinsic();

        buildTables();
    } // Constructor


    private static synchronized void buildTables()
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


    public boolean isCodeRunning()
    {
        return(codeRunning);
    } // isCodeRunning


    public void haltInterpreter()
    {
        terminateCode = true;
    } // haltInterpreter


    public boolean haltRequested()
    {
        return(terminateCode);
    } // haltRequested


    public synchronized void cleanup()
    {
        Graphics gr;
        Graphics g;
        int height = getHeight();
        int width = getWidth();


        if ((width > 0) && (height > 0))
        {
            g = getGraphics();
            if (copyImage == null)
                copyImage = createImage(width, height);

            gr = copyImage.getGraphics();
            gr.setColor(getBackground());
            gr.fillRect(0, 0, width, height);
            turtle.homeTurtle(this, g, copyImage);
            turtle.setVisible(true, g, copyImage);
            gr.dispose();
            g.dispose();
            repaint();
        } // if
    } // cleanup


    public static void displayParseException(TobyParseException tpe)
    {
        StringBuffer sb = new StringBuffer();

        if (tpe.procName != null)
        {
            sb.append("Error in function \"");
            sb.append(tpe.procName);
            sb.append("\" ");

            if (tpe.errLine != TobyParseException.LINENUM_UNDEFINED)
            {
                sb.append("(source line ");
                sb.append(tpe.errLine);
                sb.append(") ");
            } // if

            sb.append(": ");
        } // if

        sb.append(tpe.getMessage());

        JOptionPane.showMessageDialog(null, sb.toString(), "Error",
                                      JOptionPane.ERROR_MESSAGE);
    } // displayParseException


    private static void buildOperatorTable()
    /**
     *  Creates the operator table, an array of Strings, in order of
     *   precedence, of TOBY operators, such as "+", "-", and "*"...
     *
     *     params : void.
     *    returns : void.
     */
    {
        operatorTable = new String[OPERATOR_COUNT];

        operatorTable[13] = OPCODE_LPAREN;
        operatorTable[12] = OPCODE_RPAREN;
        operatorTable[11] = OPCODE_MULTIPLY;
        operatorTable[10] = OPCODE_DIVIDE;
        operatorTable[9]  = OPCODE_MODULO;
        operatorTable[8]  = OPCODE_ADD;
        operatorTable[7]  = OPCODE_SUBTRACT;
        operatorTable[6]  = OPCODE_ASSIGNMENT;
        operatorTable[5]  = OPCODE_EQUALS;
        operatorTable[4]  = OPCODE_GREATEREQL;
        operatorTable[3]  = OPCODE_LESSEQL;
        operatorTable[2]  = OPCODE_GREATER;
        operatorTable[1]  = OPCODE_LESS;
        operatorTable[0]  = OPCODE_SEPARATOR;
     } // buildOperatorTable



    private static void buildIntrinsicTable()
    /**
     *  Creates the intrinsic table, an array of Strings of
     *   keywords representing various intrinsic data types, like
     *   numbers and strings, etc...
     *
     *     params : void.
     *    returns : void.
     */
    {
        intrinsicTable = new String[INTRINSIC_COUNT];

        intrinsicTable[0] = INTRINSIC_NUMBER;
        intrinsicTable[1] = INTRINSIC_BOOLEAN;
    } // buildIntrinsicTable



    private static void buildKeywordTable()
    /**
     *  Creates the keyword table, an array of Strings of
     *   keywords representing miscellaneous reserved words.
     *
     *     params : void.
     *    returns : void.
     */
    {
        keywordTable = new String[KEYWORD_COUNT];

        keywordTable[0]  = KEYWORD_BEGINFUNC;
        keywordTable[1]  = KEYWORD_ENDFUNC;
        keywordTable[2]  = KEYWORD_IF;
        keywordTable[3]  = KEYWORD_ELSEIF;
        keywordTable[4]  = KEYWORD_ELSE;
        keywordTable[5]  = KEYWORD_ENDIF;
        keywordTable[6]  = KEYWORD_BEGINFOR;
        keywordTable[7]  = KEYWORD_TO;
        keywordTable[8]  = KEYWORD_ENDFOR;
        keywordTable[9]  = KEYWORD_RETURN;
        keywordTable[10] = KEYWORD_TRUE;
        keywordTable[11] = KEYWORD_FALSE;
        keywordTable[12] = KEYWORD_RETURNS;
        keywordTable[13] = KEYWORD_NOTHING;
    } // buildKeywordTable


    private static void buildStdFuncTable()
    /**
     *  Creates the standard functions table, an array of Strings
     *   representing built-in TOBY function names.
     *
     *     params : void.
     *    returns : void.
     */
    {
        stdFuncTable = new String[STDFUNC_COUNT];

        stdFuncTable[0]  = PROCNAME_HIDETURTLE;
        stdFuncTable[1]  = PROCNAME_SHOWTURTLE;
        stdFuncTable[2]  = PROCNAME_FORWARD;
        stdFuncTable[3]  = PROCNAME_BACKWARD;
        stdFuncTable[4]  = PROCNAME_RIGHT;
        stdFuncTable[5]  = PROCNAME_LEFT;
        stdFuncTable[6]  = PROCNAME_SETPENCOL;
        stdFuncTable[7]  = PROCNAME_GETPENCOL;
        stdFuncTable[8]  = PROCNAME_PENUP;
        stdFuncTable[9]  = PROCNAME_PENDOWN;
        stdFuncTable[10] = PROCNAME_GETANGLE;
        stdFuncTable[11] = PROCNAME_SETANGLE;
        stdFuncTable[12] = PROCNAME_PAUSE;
        stdFuncTable[13] = PROCNAME_GETTURTLEX;
        stdFuncTable[14] = PROCNAME_GETTURTLEY;
        stdFuncTable[15] = PROCNAME_SETTURTXY;
    } // buildStdFuncTable


    public static int searchTable(String[] table, String forThis)
    /**
     * Find out if (forThis) is in (table).
     *
     *     params : see above.
     *    returns : index in table, if there. (-1) if (forThis) isn't there.
     */
    {
        int i;

        for (i = 0; i < table.length; i++)
        {
            if (table[i].equals(forThis))
                return(i);
        } // for

        return(-1);
    } // searchTable



    public static boolean isReservedWord(String str)
    {
        boolean retVal = false;

        if (searchTable(operatorTable, str) != -1)
            retVal = true;
        else if (searchTable(intrinsicTable, str) != -1)
            retVal = true;
        else if (searchTable(keywordTable, str) != -1)
            retVal = true;
        else if (searchTable(stdFuncTable, str) != -1)
            retVal = true;

        return(retVal);
    } // isReservedWord


    public static boolean isIntrinsic(String str)
    {
        return((searchTable(intrinsicTable, str) != -1) ? true : false);
    } // isIntrinsic


    private void deleteProcedures()
    /**
     *  Empty the procedure listing, and free all the resources
     *   associated with it.
     *
     *      params : void.
     *     returns : void.
     */
    {
        int i;

        for (i = procList.size() - 1; i >= 0; i--)
            ((TobyProcedure) procList.elementAt(i)).freeResources();

        procList.removeAllElements();
        procList.trimToSize();
        globals.removeAllElements();
        globals.trimToSize();
        System.gc();
    } // deleteProcedures



    private TobyProcedure findProcedure(String procName)
    /**
     * This method searches through the Procedure Vector for a TOBY
     *  function with a name matching procName.
     *
     *    params : procName == the name of the function to find.
     *   returns : Reference to procedure if found, null if not found.
     */
    {
        int i;
        TobyProcedure retVal;

        for (i = procList.size() - 1; i >= 0; i--)
        {
            retVal = (TobyProcedure) procList.elementAt(i);
            if (retVal.getProcName().equals(procName))
                return(retVal);
        } // for

        return(null);   // if code falls here, procedure doesn't exist.
    } // findProcedure



    public void setCode(String newCode) throws TobyParseException
    /**
     *  Sets new TOBY code, and repaints, based on code.
     *
     *     params : newCode == Array of lines of code.
     *    returns : void.
     */
    {
        Thread codeThread;
        StringReader strReader;
        SaneTokenizer sToker;
        
        cleanup();

        if (newCode != null)
        {
            strReader = new StringReader(newCode);
            sToker = new TobyTokenizer(strReader);

            try
            {
                buildGlobals(sToker);
                buildProcedures(sToker);
            } // try
            catch (TobyParseException tpe)
            {
                deleteProcedures();
                throw(tpe);           // send error on down the line...
            } // catch

            strReader.close();

            codeThread = new Thread(this);
            codeThread.start();
        } // else
    } // setCode



    private void buildGlobals(SaneTokenizer sToker) throws TobyParseException
    {
        // !!! write this.
    } // buildGlobals



    private void buildProcedures(SaneTokenizer sToker)
                                    throws TobyParseException
    /**
     *  This method will create a bunch of TobyProcedure objects,
     *   based on the source code passed to it.
     *
     *        params : sToker == Tokenizer of source code.
     *       returns : void.
     *        throws : TobyParseException on crap-out.
     */
    {
        do
        {
            try
            {
                if (sToker.nextToken() == SaneTokenizer.TT_WORD)
                    checkIfProc(sToker);
            } // try
            catch (IOException e)
            {
                TobyParseException.throwException(PROCERR_INTERNAL, null);
            } // catch
        } while (sToker.ttype != SaneTokenizer.TT_EOF);

        if (findProcedure(PROCNAME_MAINLINE) == null)
            TobyParseException.throwException(PROCERR_NO_MAINLINE, null);

        procList.trimToSize();    // if code falls to here, we win.  :)
    } // buildProcedures



    private void checkIfProc(SaneTokenizer sToker) throws TobyParseException,
                                                          IOException
    /**
     *  Checks if current token in sToker is the start of a TOBY function.
     *   If so, a new TobyProcedure object is added to procList. If not,
     *   a TobyParseException is thrown, since if this isn't the start of
     *   a function, than we have code outside a procedure, hence "orphaned"
     *   code.
     *
     *     params : sToker == Tokenizer holding the token.
     *    returns : void.
     *     throws : TobyParseException, for various reasons.
     */
    {
        String pName;
        TobyProcedure proc;
        int startLine;

        if (sToker.sval.equals(KEYWORD_BEGINFUNC))
        {
            startLine = sToker.lineno();
            proc = new TobyProcedure(sToker);
            pName = proc.getProcName();
            if (findProcedure(pName) != null)
            {
                TobyParseException.throwException(PROCERR_DOUBLE_DEF, pName,
                                                  startLine);
            } // if
            else
                procList.addElement(proc);   // it's all good: add it.
        } // if

        else
            TobyParseException.throwException(PROCERR_ORPHAN_CODE, null);
    } // checkIfProc


    private synchronized void initGraphics()
    {
        int i;
        Dimension d = getSize();

        g[GRAPHICS_COPY] = copyImage.getGraphics();
        g[GRAPHICS_SCREEN] = getGraphics();

        for (i = 0; i < g.length; i++)
        {
            g[i].setColor(getBackground());
            g[i].fillRect(0, 0, d.width, d.height);
            g[i].setColor(getForeground());
        } // for
    } // initGraphics


    private void deInitGraphics()
    {
        g[GRAPHICS_COPY].dispose();
        g[GRAPHICS_SCREEN].dispose();
        System.gc();         // Cleanup from all that parsing...
    } // deInitGraphics


    public void paint(Graphics g)
    /**
     *  Note that while Turtle isn't actually derived from Component, it
     *   needs paint()'s Graphics parameter, so it may modify TurtleSpace's
     *   canvas. This method works as such: we'll bitblt copyImage to the
     *    screen, if it exists, or just draw a blank if all else fails.
     *
     *      params : g == the "device context;" see Component::paint().
     *     returns : void.
     */
    {
        int width;
        int height;

        if (copyImage == null)      // creates copyImage ...
            cleanup();

        if (copyImage != null)
        {
            g.drawImage(copyImage, 0, 0, this);
            turtle.paint(g, copyImage);
        } // if
        else        // give up and draw default...
        {
            g.setColor(getBackground());
            g.drawRect(0, 0, getWidth(), getHeight());
        } // else
    } // paint



        /*
         * All the execution of TOBY code is done below, after it has been
         *  parsed into several TobyProcedure objects. This is a recursive
         *  nightmare, but this is really the only effective way I could
         *  figure on to design this interpreter.
         *
         * Please start your examination with run(), as the interpretation 
         *  always starts there, called only by setCode()...
         *
         * The first TobyProcedure to be interpreted is always the mainline
         *  the user has defined, although runCode() is called recursively
         *  for execution of other user-defined routines.
         */

    private double convStrToDouble(String conv) throws TobyParseException
    {
        double retVal = 0.0;

        try
        {
            retVal = Double.valueOf(conv).doubleValue();
        } // try
        catch (NumberFormatException nfe)   // not a number?
        { //!!! change error?
            TobyParseException.throwException(nfe.getMessage(), null);
        } // catch

        return(retVal);
    } // convStrToDouble


    private Intrinsic buildVar(String type, String name)
                                           throws TobyParseException
    /**
     *  Check to see if (type) is a valid name for an intrinsic type, and
     *   build that type of intrinsic, naming it (name)...
     *  !!! comment params/returns/throws...
     */
    {
        Intrinsic retVal = null;

        if (type.equals(INTRINSIC_NUMBER))
            retVal = new NumberIntrinsic(name);
        else if (type.equals(INTRINSIC_BOOLEAN))
            retVal = new BooleanIntrinsic(name);
        else
            TobyParseException.throwException(PROCERR_BAD_TYPE, null, 0);

        return(retVal);
    } // buildVar


    private Intrinsic createTobyVar(String type, String name, Stack addToStack)
                                           throws TobyParseException
    /**
     * Build a new intrinsic Toby variable, initialize it, and return it.
     *  Checks are made to make sure the variable type is valid, and
     *  the name isn't already in use in the current scope.
     *
     * VarWatchers registered with this TurtleSpace are notified.
     *
     *    params : type == string name of var type. ("number", "boolean", etc)
     *             name == identifier for new variable.
     *             addToStack == scope of variable (global? Local?)
     *   returns : new Intrinsic object.
     *    throws : TobyParseException on duplicate vars and other errors.
     */
    {
        Intrinsic retVal;

        if (isValidIdentifier(name) == false)
            TobyParseException.throwException(PROCERR_BAD_IDENT, null, 0);
            
        if ((addToStack != null) &&
            (scanIntrinsicStack(addToStack, name) != null))
        {
            TobyParseException.throwException(PROCERR_DOUBLE_DEF, null, 0);
        } // if

        retVal = buildVar(type, name);

        if (addToStack != null)
            addToStack.push(retVal);

        notifyVarCreated(retVal);
        return(retVal);
    } // createTobyVar


    private void setTobyVar(Intrinsic var, String newValue)
                               throws TobyParseException
    /**
     * Make sure that (newValue) is acceptable to set as (var)'s value, and
     *  do so. Notify all registered VarWatchers.
     */
    {
        var.setValue(newValue);
        notifyVarUpdated(var);
    } // setTobyVar


    private Intrinsic scanIntrinsicStack(Stack scanStack, String ident)
    /*
     * Toby keeps a java.util.Stack that resembles a processor's stack.
     *  It stores references to TobyProcedures and Intrinsics (which is
     *  much like a real program's stack: return addresses and local
     *  variables...). This function allows you to scan a stack from
     *  top to bottom (newest to oldest) looking for an intrinsic named
     *  (ident). If a TobyProcedure is encountered, we consider any
     *  objects below it to be "out of scope". We allow a stack to be
     *  specified, since Toby stores global variables in a different stack
     *  than the locals. Locals, should always be scanned first to
     *  correct;y resolve name clashes, naturally.
     *
     *     @param scanStack Stack to scan.
     *     @param ident Name of intrinsic to find.
     *    @return Intrinsic if it is found, (null) otherwise.
     */
    {
        int i;
        boolean getOut = false;
        Object obj;

        for (i = stack.size() - 1; (i >= 0) && (!getOut); i--)
        {
            obj = stack.elementAt(i);

            if (obj instanceof TobyProcedure)
                getOut = true;

            else if (obj instanceof Intrinsic)
            {
                    // below is the ugliest line of code in this whole program.
                if (((Intrinsic) obj).getIdentifier().equals(ident))
                    return((Intrinsic) obj);
            } // else if
        } // for

        return(null);   // not found if code falls here...
    } // scanIntrinsicStack


    public static boolean isValidIdentifier(String ident)
    /**
     *  Checks if (ident) can be used to name functions, variables, etc...
     *
     *      params : ident == name to check for validity.
     *     returns : (true) if valid, (false) if INvalid.
     */
    {
        boolean retVal = true;

        if (ident.length() == 0)
            retVal = false;
        else if (Character.isLetter(ident.charAt(0)) == false)
            retVal = false;
        else if (isReservedWord(ident))
            retVal = false;

        return(retVal);
    } // isValidIdentifier


    private Intrinsic varDefined(String ident)
    /**
     * Checks stack and (possibly) globals for a var named (identifier)...
     *
     *    params : ident        == variable name to look for.
     *   returns : Intrinsic for said variable, or (null) if not found.
     */
    {
        Intrinsic retVal = null;

        if (isValidIdentifier(ident))
        {
            retVal = scanIntrinsicStack(stack, ident);
            if (retVal == null)
                retVal = scanIntrinsicStack(globals, ident);
        } // if

        return(retVal);
    } // varDefined



    private int findHighestOperator(String[] src)
    /**
     *  This method searches a line of code for the operator with the
     *   highest precedence. This assures that mathematical equasions
     *   come up with accurate results. Please refer to buildOperatorTable()
     *   for a list of understood operators, and their precedence.
     *
     *      params : src == array of tokens of one line of TOBY code.
     *     returns : index of highest precedence operator. -2 if no tokens
     *               remain, -1 if no operators found.
     */
    {
        int i;
        int highToken = -2;
        int highestPrecedence = -1;   // lowest precedence.
        int thisPrecedence;

        for (i = 0; i < src.length; i++)
        {
            if (src[i] != null)
            {
                thisPrecedence = searchTable(operatorTable, src[i]);
                if (thisPrecedence > highestPrecedence)
                {
                    highestPrecedence = thisPrecedence;
                    highToken = i;
                } // if
            } // if
        } // for

        return(highToken);
    } // findHighestOperator



    private int findLeftValue(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Find the first non-null value to the left of the specified index.
     *   A token set to (null) means it has already been used. If the
     *   non-null token in question is a defined variable, we swap out
     *   the identifier in (src) with the value currently assigned to said
     *   variable.
     *
     *     params : src == TOBY source line.
     *              tokIndex == index of token we need a value left of.
     *    returns : see above.
     *     throws : TobyParseException if no such token exists.
     */
    {
        Intrinsic var;
        int i;

        for (i = tokIndex; i >= 0; i--)
        {
            if (src[i] != null)
            {
                var = varDefined(src[i]);
                if (var != null)
                    src[i] = var.getValue();
                return(i);
            } // if
        } // for

        TobyParseException.throwException(PROCERR_SYNTAX_ERR, null);

        return(0);  // never hits this point; this just keeps javac happy.
    } // findLeftValue



    private int findRightValue(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  The same as findLeftValue(), but it goes the opposite direction.
     *
     *    params, returns, throws : see findLeftValue()...
     */
    {
        Intrinsic var;
        int i;

        for (i = tokIndex; i < src.length; i++)
        {
            if (src[i] != null)
            {
                var = varDefined(src[i]);
                if (var != null)
                    src[i] = var.getValue();
                return(i);
            } // if
        } // for

        TobyParseException.throwException(PROCERR_SYNTAX_ERR, null);

        return(0);  // never hits this point; this just keeps javac happy.
    } // findRightValue



    private String[] moveGroupingToArray(String[] src,
                                         int tokIndex,
                                         String endToken)
    /**
     *
     * !!! comment !!!
     *
     *
     */
    {
        int i;
        int n;
        int groupingCount = 1;
        String startToken = src[tokIndex];
        String[] retVal = null;

        for (i = tokIndex + 1; (i < src.length) && (groupingCount > 0); i++)
        {
            if (src[i].equals(endToken))
                groupingCount--;
            else if (src[i].equals(startToken))
                groupingCount++;
        } // for

        if (groupingCount == 0)
        {
            i--;
            src[tokIndex] = null;      // blank left delimieter...
            src[i]        = null;      // blank right delimieter...

            tokIndex++;
            retVal = new String[i - tokIndex];
            for (n = tokIndex; n < i; n++)
            {
                retVal[n - tokIndex] = src[n];
                src[n] = null;
            } // for
        } // if

        return(retVal);
    } // moveGroupingToArray



    private void dealWithMultiply(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Deal with a mulplication operator.
     *
     *     params : src      == code line.
     *              tokIndex == index of multiplication operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        int lvalue;
        int rvalue;
        double rc;

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        rc = convStrToDouble(src[lvalue]) * convStrToDouble(src[rvalue]);

        src[lvalue] = null;
        src[rvalue] = null;
        src[tokIndex] = Double.toString(rc);
    } // dealWithMultiply



    private void dealWithSubtract(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Deal with a subtraction operator.
     *
     *     params : src      == code line.
     *              tokIndex == index of subtraction operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        int lvalue;
        int rvalue;
        double rc;

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        rc = convStrToDouble(src[lvalue]) - convStrToDouble(src[rvalue]);

        src[lvalue] = null;
        src[rvalue] = null;
        src[tokIndex] = Double.toString(rc);
    } // dealWithSubtract



    private void dealWithDivide(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Deal with a division operator.
     *
     *     params : src      == code line.
     *              tokIndex == index of division operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        int lvalue;
        int rvalue;
        double rc;

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        rc = convStrToDouble(src[lvalue]) / convStrToDouble(src[rvalue]);

        src[lvalue] = null;
        src[rvalue] = null;
        src[tokIndex] = Double.toString(rc);
    } // dealWithDivide



    private void dealWithAdd(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Deal with an addition operator.
     *
     *     params : src      == code line.
     *              tokIndex == index of addition operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        int lvalue;
        int rvalue;
        double rc;

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        rc = convStrToDouble(src[lvalue]) + convStrToDouble(src[rvalue]);

        src[lvalue] = null;
        src[rvalue] = null;
        src[tokIndex] = Double.toString(rc);
    } // dealWithAdd



    private void dealWithModulo(String[] src, int tokIndex)
                                  throws TobyParseException
    /**
     *  Deal with a modulo operator.
     *
     *     params : src      == code line.
     *              tokIndex == index of modulo operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        int lvalue;
        int rvalue;
        double rc;

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        rc = convStrToDouble(src[lvalue]) % convStrToDouble(src[rvalue]);

        src[lvalue] = null;
        src[rvalue] = null;
        src[tokIndex] = Double.toString(rc);
    } // dealWithModulo



    private void dealWithAssignment(String[] src, int tokIndex)
                                     throws TobyParseException
    /**
     *  Deal with an assignment operator.
     *
     *  Currently, assignments can only be the first operator of a
     *   line. This isn't like C, C++, and Java, and the restriction
     *   may be lifted at a later time.
     *
     *     params : src      == code line.
     *              tokIndex == index of multiplication operator in src.
     *     throws : TobyParseException on errors.
     */
    {
        Intrinsic var;
        int assignValue;

        if (tokIndex != 1)  // first operator on line?
            TobyParseException.throwException(PROCERR_SYNTAX_ERR, null);

        var = varDefined(src[tokIndex - 1]);
        if (var == null)
            TobyParseException.throwException(PROCERR_NOT_A_VAR, null);

        src[tokIndex]     = null;     // blank assignment operator.
        src[tokIndex - 1] = null;     // blank variable name.

        // !!! need to calculate()?
        assignValue = findRightValue(src, tokIndex + 1);
        setTobyVar(var, src[assignValue]);
        src[assignValue] = null;      // yeah, blank rvalue, too.
    } // dealWithAssignment



    private void dealWithParens(String[] src, int tokIndex)
                                       throws TobyParseException
    /**
     *   !!! comment !!!
     *
     */
    {
        TobyProcedure procCall;
        String[] innerToks;
        Intrinsic retCall;

        innerToks = moveGroupingToArray(src, tokIndex, OPCODE_RPAREN);
        if (innerToks == null)
            TobyParseException.throwException(PROCERR_NO_RPAREN, null);

        if ((tokIndex == 0) ||                   // first token or operator?
            (searchTable(operatorTable, src[tokIndex - 1]) != -1))
        {
            calculate(innerToks);
            src[tokIndex] = innerToks[findRightValue(innerToks, 0)];
        } // if

        else    // potential function call...
        {
            retCall = dealWithFunctionCall(src[tokIndex - 1],innerToks);
            src[tokIndex - 1] = retCall.getValue();
        } // else
    } // dealWithParens



    private Intrinsic dealWithFunctionCall(String funcName, String[] argToks)
                                      throws TobyParseException
    /** !!! comment !!! */
    {
        Intrinsic retCall;    // Returned value.

        retCall = dealWithStdFunction(funcName, argToks);
        if (retCall == null)        // no such std. function exists?
        {
            retCall = dealWithUserFunction(funcName, argToks);
            if (retCall == null)    // no such user-defined function exists?
                TobyParseException.throwException(PROCERR_NOT_A_FUNC, null);
        } // if

        return(retCall);
    } // dealWithFunctionCall



    private Intrinsic dealWithUserFunction(String funcName,
                                                String[] argToks)
                                                 throws TobyParseException
    /**
     *  Check if funcName is a user-defined function, and if so, interpret it.
     *
     *     params : funcName == potential user-defined function name.
     *              argToks  == Tokens that make up arguments in this call.
     *    returns : Intrinsic instance of return value, which may be a
     *               NothingIntrinsic, or (null) if function not found.
     *     throws : TobyParseException if some function down the line throws.
     */
    {
        TobyProcedure proc;
        Intrinsic retCall = null;
        Vector args;

        proc = findProcedure(funcName);
        if (proc != null)
        {
            args = buildArguments(argToks, proc.getParamCount());
            stack.push(proc);
            moveArgsToStack(proc, args);
            retCall = runCode(proc);
        } // if

        return(retCall);
    } // dealWithUserFunctions



    private Intrinsic dealWithStdFunction(String funcName,
                                          String[] argToks)
                                              throws TobyParseException
    /**
     *  This method checks if (funcName) is a standard TOBY function name,
     *   and if so, executes that function, using the arguments passed in
     *   (argToks). This may result in a return value, a change in the
     *   Turtle's state, or both.
     *
     *      params : funcName == function to call, if it exists.
     *     returns : Intrinsic containing function's return value
     *                (which may be NothingIntrinsic if function returns void.),
     *                or (null) if the function didn't exist in the
     *                first place.
     *      throws : TobyParseException on code pukeage.
     */
    {
        Intrinsic retVal = nothing;    // NothingIntrinsic return is assumed.
        Vector args;
        String tmpStr;
        int tmpInt;

        if (funcName.equals(PROCNAME_FORWARD))
        {
            args = buildArguments(argToks, 1);
            turtle.forwardTurtle(convStrToDouble((String) args.elementAt(0)),
                                 g, g[GRAPHICS_SCREEN], copyImage);
        } // if

        else if (funcName.equals(PROCNAME_BACKWARD))
        {
            args = buildArguments(argToks, 1);
            tmpInt = (int) convStrToDouble((String) args.elementAt(0));
            turtle.forwardTurtle(-convStrToDouble((String) args.elementAt(0)),
                                 g, g[GRAPHICS_SCREEN], copyImage);
        } // if

        else if (funcName.equals(PROCNAME_RIGHT))
        {
            args = buildArguments(argToks, 1);
            turtle.rotate(convStrToDouble((String) args.elementAt(0)),
                          g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_LEFT))
        {
            args = buildArguments(argToks, 1);
            turtle.rotate(-convStrToDouble((String) args.elementAt(0)),
                          g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_PENUP))
        {
            args = buildArguments(argToks, 0);
            turtle.setPenUp(true);
        } // else if

        else if (funcName.equals(PROCNAME_PENDOWN))
        {
            args = buildArguments(argToks, 0);
            turtle.setPenUp(false);
        } // else if

        else if (funcName.equals(PROCNAME_HIDETURTLE))
        {
            args = buildArguments(argToks, 0);
            turtle.setVisible(false, g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_SHOWTURTLE))
        {
            args = buildArguments(argToks, 0);
            turtle.setVisible(true, g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_PAUSE))
        {
            long currentTime = System.currentTimeMillis();
            long stopTime;

            args = buildArguments(argToks, 1);
            tmpInt = (int) convStrToDouble((String) args.elementAt(0));
            stopTime = currentTime + tmpInt;

            do
            {
                Thread.yield();
                currentTime = System.currentTimeMillis();
            } while (currentTime < stopTime);
        } // else if

        else if (funcName.equals(PROCNAME_GETTURTLEX))
        {
            args = buildArguments(argToks, 0);
            retVal = new NumberIntrinsic(null, Double.toString(turtle.getX()));
        } // else if

        else if (funcName.equals(PROCNAME_GETTURTLEY))
        {
            args = buildArguments(argToks, 0);
            retVal = new NumberIntrinsic(null, Double.toString(turtle.getY()));
        } // else if

        else if (funcName.equals(PROCNAME_SETTURTXY))
        {
            double x;
            double y;

            args = buildArguments(argToks, 2);
            x = convStrToDouble((String) args.elementAt(0));
            y = convStrToDouble((String) args.elementAt(1));

            turtle.setXY(x, y, g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_GETANGLE))
        {
            args = buildArguments(argToks, 0);
            retVal = new NumberIntrinsic(null,
                                         Double.toString(turtle.getAngle()));
        } // else if

        else if (funcName.equals(PROCNAME_SETANGLE))
        {
            args = buildArguments(argToks, 1);
            turtle.setAngle(convStrToDouble((String) args.elementAt(0)),
                            g[GRAPHICS_SCREEN], copyImage);
        } // else if

        else if (funcName.equals(PROCNAME_GETPENCOL))
        {
            args = buildArguments(argToks, 0);
            retVal = new NumberIntrinsic(null, turtle.getPenColorNumStr());
        } // else if

        else if (funcName.equals(PROCNAME_SETPENCOL))
        {
            args = buildArguments(argToks, 1);
            turtle.setPenColor(convStrToDouble((String) args.elementAt(0)));
        } // else if
        else
            retVal = null;  // function not found.

        return(retVal);
    } // dealWithStdFunction



    private Vector buildArguments(String[] argToks, int totalArgs)
                                   throws TobyParseException
    /** !!! comment !!! */
    {
        Vector retVal = new Vector();
        int i;
        int count = 0;
        String[] tmp;

        for (i = 0; i < argToks.length; i++)
        {
            if (argToks[i].equals(OPCODE_SEPARATOR))    // last tok in arg?
            {
                tmp = new String[count];
                System.arraycopy(argToks, i - count, tmp, 0, count);
                calculate(tmp);
                retVal.addElement(tmp[findRightValue(tmp, 0)]);
                count = 0;
            } // if

            else if (i == argToks.length - 1)
            {
                count++;
                tmp = new String[count];
                System.arraycopy(argToks, i - (count - 1), tmp, 0, count);
                calculate(tmp);
                retVal.addElement(tmp[findRightValue(tmp, 0)]);
                count = 0;
            } // else if

            else
                count++;
        } // for

        if (retVal.size() != totalArgs)
        {
            retVal.removeAllElements();
            retVal.trimToSize();
            TobyParseException.throwException(PROCERR_BADNUM_ARGS, null);
        } // if

        return(retVal);
    } // buildArguments


    private int findEndBlock(TobyProcedure proc,
                             String startToken,
                             int srcLine,
                             String endToken)
                               throws TobyParseException
    /**
     *  This method is used to find the correct END block for any
     *   flow control modifier. For example, we might need to find the
     *   ENDIF statement after hitting an IF statement. IFs may be nested,
     *   so if we hit another IF before hitting an ENDIF, we need to skip
     *   the first ENDIF, since it belongs to the nested IF.
     *
     *      params : proc       == TobyProcedure with the code.
     *               startToken == code signifying beginning of block.
     *               srcLine    == line index in (proc) of start of block.
     *               endToken   == code signifying end of block.
     *     returns : line index in (proc) where end block is. (-1) if none.
     *      throws : TobyParseException on panic and syntax error.
     */
    {
        int i;
        int tokenCount = 1;
        int totals = proc.sourceLineCount();
        String[] codeLine;

        for (i = srcLine + 1; (i < totals) && (tokenCount > 0); i++)
        {
            codeLine = proc.getSourceLine(i);
            if (codeLine[0].equals(startToken))
                tokenCount++;
            else if (codeLine[0].equals(endToken))
                tokenCount--;
        } // for

        if (tokenCount > 0)
            return(-1);

        return(i);
    } // findEndBlock



    private int dealWithForLoop(TobyProcedure proc,
                                String[] src,
                                int srcLine)
                                       throws TobyParseException
    {
        Intrinsic var;
        ForLoopDetails forDetails = new ForLoopDetails(0);
        String[] initValue;
        int position;

        var = varDefined(src[1]);
        if (var == null)
            TobyParseException.throwException(PROCERR_NOT_A_VAR, null);

        if ((var instanceof NumberIntrinsic) == false)
            TobyParseException.throwException(PROCERR_BAD_TYPE, null);

        forDetails.ascending = true;   // !!! fix this.
        forDetails.var = var;
        forDetails.lineNum = srcLine + 1;

        src[0] = null;   // lose KEYWORD_BEGINFOR...

        if (src[2].equals(OPCODE_ASSIGNMENT) == false)
            TobyParseException.throwException(PROCERR_SYNTAX_ERR, null);

        initValue = moveGroupingToArray(src, 0, KEYWORD_TO);
        dealWithAssignment(initValue, 1);

            // locate the conditional equasion (for i = x to [conditional])...
        position = findRightValue(src, 4);
        forDetails.condition = new String[src.length - position];
        initValue = new String[forDetails.condition.length];

            // save a copy of the conditional, and make a copy to calculate().
            //  we have to save a copy since the value may change with each
            //  iteration.
        System.arraycopy(src, position, initValue, 0, initValue.length);
        System.arraycopy(src, position, forDetails.condition, 0,
                         forDetails.condition.length);

        calculate(initValue);   // !!! separate into another function...
        if (convStrToDouble(var.getValue()) <   // fix for descending !!!
            convStrToDouble(initValue[findRightValue(initValue, 0)]))
        {
            stack.push(forDetails);
            return(srcLine + 1);
        } // if
        
            // Already past condition? Skip out of FOR loop...
        position = findEndBlock(proc, KEYWORD_BEGINFOR,
                                srcLine, KEYWORD_ENDFOR);

        if (position == -1)
            TobyParseException.throwException(PROCERR_NO_ENDFOR, null);

        return(position);
    } // dealWithForLoop


    private int dealWithEndForLoop(TobyProcedure proc,
                                   String[] src,
                                   int srcLine)
                                       throws TobyParseException
    /** !!! comment !!! */
    {
        Object obj;
        ForLoopDetails forDetails;
        double tmpDbl;
        String[] tmpStrs;
        boolean breakLoop = false;

        obj = stack.peek();
        if (!(obj instanceof ForLoopDetails))
            TobyParseException.throwException(PROCERR_NO_FOR, null);

        forDetails = (ForLoopDetails) obj;

        tmpStrs = new String[forDetails.condition.length];
        System.arraycopy(forDetails.condition, 0, tmpStrs, 0, tmpStrs.length);
        calculate(tmpStrs);

        if (forDetails.ascending)  // increment or decrement?
            tmpDbl = convStrToDouble((String) forDetails.var.value) + 1.0;
        else
            tmpDbl = convStrToDouble((String) forDetails.var.value) - 1.0;

        forDetails.var.value = Double.toString(tmpDbl);        // replace.

        if (forDetails.ascending)
        {
            if (tmpDbl > convStrToDouble(tmpStrs[findRightValue(tmpStrs, 0)]))
                breakLoop = true;
        } // if
        else
        {
            if (tmpDbl < convStrToDouble(tmpStrs[findRightValue(tmpStrs, 0)]))
                breakLoop = true;
        } // else

        if (breakLoop)     // remove ForLoopDetails object...
            stack.pop();

        return((breakLoop) ? (srcLine + 1) : (forDetails.lineNum));
    } // dealWithEndForLoop


    private Intrinsic intrinsicByTypeStr(String typeStr, String initVal)
                                          throws TobyParseException
    /*
     * Create an Intrinsic object by a string of it type. For example,
     *  if (typeStr) is "number" than a NumberIntrinsic is created.
     *
     *      @param typeStr String representation of intrinsic type.
     *      @param initval String representation of initial value of new var.
     *     @return subclass of Intrinsic that matches specified type.
     *      @throw TobyParseException if specified type doesn't exist.
     */
    {
        Intrinsic retVal = null;

        if (typeStr.equals(INTRINSIC_NUMBER))
            retVal = new NumberIntrinsic(null, initVal);
        else if (typeStr.equals(INTRINSIC_BOOLEAN))
            retVal = new BooleanIntrinsic(null, initVal);
        else
            TobyParseException.throwException(PROCERR_BAD_TYPE, null);

        return(retVal);
    } // intrinsicByTypeStr


    private void dealWithReturn(TobyProcedure proc, String[] src)
                                 throws TobyParseException, Intrinsic
    /*
     * The main Toby procedure executing loop
     *
     *
     *
     *
     *
     */
    {
        String retType = proc.getReturnType();

        if (src.length == 1)                    // basic return? !!! ?!
        {
            if (retType == null)
                throw(nothing);
        } // if
        else
        {
             src[0] = null;   // blank KEYWORD_RETURN...
             calculate(src);
             throw(intrinsicByTypeStr(retType, src[findLeftValue(src, 2)]));
        } // else
    } // dealWithReturn


    private int dealWithConditional(TobyProcedure proc,
                                     String[] src,
                                     int srcLine)
                                        throws TobyParseException
    /**
     *  Handler for IF statements.
     *
     *     params : proc    == Procedure this code appears in.
     *              src     == code line.
     *              srcLine == line index of code from (proc).
     *    returns : new line index in (proc) to branch to.
     *     throws : TobyParseException on errors.
     */
    {
        int boolIndex;
        String varType;

            // scoot condition over to the left, so calculate() doesn't
            //  think parentheses are a function call...
        for (boolIndex = 1; boolIndex < src.length; boolIndex++)
            src[boolIndex - 1] = src[boolIndex];

        src[src.length - 1] = null;  // blank extra token copy.

        calculate(src);
        boolIndex = findRightValue(src, 0);

        varType = intrinsicTable[determineType(src[boolIndex])];

        if (!varType.equals(INTRINSIC_BOOLEAN))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        return(followConditionalBranch(proc, srcLine,
                                       src[boolIndex].equals(KEYWORD_TRUE)));
    } // dealWithConditional



    private int followConditionalBranch(TobyProcedure proc,
                                         int srcLine,
                                         boolean tf) throws TobyParseException
    {
        String[] src;
        boolean getOut = false;
        int retVal = 0;

        if (tf == true)
        {
            stack.push(new IfLoopDetails(srcLine, false));
            retVal = srcLine + 1;
        } // if
        else
        {
            for (; (!getOut) && (srcLine < proc.sourceLineCount()); srcLine++)
            {
                src = proc.getSourceLine(srcLine);

                if (src[0].equals(KEYWORD_ENDIF))
                {
                    getOut = true;
                    retVal = srcLine + 1;
                } // if

                else if (src[0].equals(KEYWORD_ELSEIF))
                {
                    getOut = true;
                    retVal = dealWithConditional(proc, src, srcLine);
                } // else if

                else if (src[0].equals(KEYWORD_ELSE))
                {
                    getOut = true;
                    stack.push(new IfLoopDetails(srcLine, true));
                    retVal = srcLine + 1;
                } // else if
            } // for
        } // else

        return(retVal);
    } // followConditionalBranch


    private int dealWithEndIf(TobyProcedure proc, String[] src,
                              int srcLine, String procErr)
                                         throws TobyParseException
    /**
     *  Deals with potential breaks from conditional blocks. This is called
     *   when an ELSE, ELSEIF, or ENDIF is hit. This code does not evaluate
     *   conditionals themselves, but sees if they represent the end of a
     *   conditional we've previously followed. Refer to dealWithConditional()
     *   for that handler.
     *
     *     params : proc    == TobyProcedure with ELSE/ELSEIF.
     *              src     == line of code.
     *              srcLine == line index of ELSE/ELSEIF.
     *              procErr == Error message if not inside an IF statement.
     */
    {
        Object obj = stack.peek();
        int retVal = 0;

        if (obj instanceof IfLoopDetails)
        {
            IfLoopDetails ifd = (IfLoopDetails) obj;
            if ((ifd.isElse) && (!src[0].equals(KEYWORD_ENDIF)))
                TobyParseException.throwException(procErr, null);

            stack.pop();  // lose IfLoopDetails...

            if (src[0].equals(KEYWORD_ENDIF))
                retVal = srcLine + 1;
            else
            {
                retVal = findEndBlock(proc, KEYWORD_IF, ifd.lineNum,
                                      KEYWORD_ENDIF);
            } // else
        } // if
        else
            TobyParseException.throwException(procErr, null);

        return(retVal);
    } // dealWithEndIf


    private int dealWithFlowControl(TobyProcedure proc,
                                    String[] src,
                                    int srcLine)
                                       throws TobyParseException, Intrinsic
    /**
     *  This method checks for and handles logic structures such as
     *   conditionals, loops, returns, etc...in the TOBY code.
     *
     *      params : proc    == procedure being interpreted.
     *               src     == line of code with potential flow control.
     *               srcLine == line in proc that src came from.
     *     returns : line number in proc where control should resume.
     *      throws : TobyParseException on syntax and other errors...
     *               Intrinsic if a function return keyword is hit.
     */
    {
        int retVal = srcLine;
        String tok = src[0];

        if (tok.equals(KEYWORD_BEGINFOR))
            retVal = dealWithForLoop(proc, src, srcLine);

        else if (tok.equals(KEYWORD_ENDFOR))
            retVal = dealWithEndForLoop(proc, src, srcLine);

        else if (tok.equals(KEYWORD_IF))
            retVal = dealWithConditional(proc, src, srcLine);

        else if (tok.equals(KEYWORD_ENDIF))
            retVal = dealWithEndIf(proc, src, srcLine, PROCERR_ORPHAN_ENDIF);

        else if (tok.equals(KEYWORD_ELSE))
            retVal = dealWithEndIf(proc, src, srcLine, PROCERR_ORPHAN_ELSE);

        else if (tok.equals(KEYWORD_ELSEIF))
            retVal = dealWithEndIf(proc, src, srcLine, PROCERR_ORPHAN_ELIF);

        else if (tok.equals(KEYWORD_RETURN))
            dealWithReturn(proc, src);


        // !!! add more flow control keywords... !!!


        return(retVal);
    } // dealWithFlowControl


    public boolean bothNumbers(String str1, String str2)
                                          throws TobyParseException
    /**
     *  Returns (true) if (str1) and (str2) are both Number intrinsics.
     *  Returns (false) otherwise.
     */
    {
        int whatType = determineType(str1);
        boolean retVal = true;

        if (intrinsicTable[whatType].equals(INTRINSIC_NUMBER) == false)
            retVal = false;
        else if (whatType != determineType(str2))
            retVal = false;

        return(retVal);
    } // bothNumbers


    private void dealWithGreaterThan(String[] src, int tokIndex)
                                           throws TobyParseException
    /**
     * Handle "greater than" operator. This method checks to see if two
     *  Toby objects are numeric, and if so, sees if the first > the second.
     *  Depending, either KEYWORD_TRUE or KEYWORD_FALSE is placed
     *  into (src)[(tokIndex)].
     *
     *      params : src == array of tokens remaining on a given line of code.
     *               tokIndex == index of OPERATOR_GREATER in (src).
     *     returns : void.
     *      throws : TobyParseException on syntax errors, etc...
     */
    {
        int lvalue;    // index of comparable value to left of operator.
        int rvalue;    // index of comparable value to right of operator.

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        if (!bothNumbers(src[lvalue], src[rvalue]))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        if (convStrToDouble(src[lvalue]) > convStrToDouble(src[rvalue]))
            src[tokIndex] = KEYWORD_TRUE;
        else
            src[tokIndex] = KEYWORD_FALSE;

        src[lvalue] = src[rvalue] = null;
    } // dealWithGreaterThan


    private void dealWithLessThan(String[] src, int tokIndex)
                                           throws TobyParseException
    /**
     * Same as dealWithGreaterThan(), but for "<" ...
     */
    {
        int lvalue;    // index of comparable value to left of operator.
        int rvalue;    // index of comparable value to right of operator.

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        if (!bothNumbers(src[lvalue], src[rvalue]))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        if (convStrToDouble(src[lvalue]) < convStrToDouble(src[rvalue]))
            src[tokIndex] = KEYWORD_TRUE;
        else
            src[tokIndex] = KEYWORD_FALSE;

        src[lvalue] = src[rvalue] = null;
    } // dealWithLessThan


    private void dealWithGreaterThanOrEqualTo(String[] src, int tokIndex)
                                               throws TobyParseException
    /**
     * Same as dealWithGreaterThan(), but for ">=" ...
     */
    {
        int lvalue;    // index of comparable value to left of operator.
        int rvalue;    // index of comparable value to right of operator.

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        if (!bothNumbers(src[lvalue], src[rvalue]))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        if (convStrToDouble(src[lvalue]) >= convStrToDouble(src[rvalue]))
            src[tokIndex] = KEYWORD_TRUE;
        else
            src[tokIndex] = KEYWORD_FALSE;

        src[lvalue] = src[rvalue] = null;
    } // dealWithGreaterThanOrEqualTo


    private void dealWithLessThanOrEqualTo(String[] src, int tokIndex)
                                               throws TobyParseException
    /**
     * Same as dealWithGreaterThan(), but for "<=" ...
     */
    {
        int lvalue;    // index of comparable value to left of operator.
        int rvalue;    // index of comparable value to right of operator.

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        if (!bothNumbers(src[lvalue], src[rvalue]))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        if (convStrToDouble(src[lvalue]) <= convStrToDouble(src[rvalue]))
            src[tokIndex] = KEYWORD_TRUE;
        else
            src[tokIndex] = KEYWORD_FALSE;

        src[lvalue] = src[rvalue] = null;
    } // dealWithLessThanOrEqualTo


    private void dealWithEquals(String[] src, int tokIndex)
                                           throws TobyParseException
    /**
     * Handle equality operator. This method checks to see if two
     *  Toby objects are of the same type, and if so, compares them for
     *  equality. Depending, either KEYWORD_TRUE or KEYWORD_FALSE is
     *  placed into (src)[(tokIndex)].
     *
     *      params : src == array of tokens remaining on a given line of code.
     *               tokIndex == index of OPERATOR_EQUALS in (src).
     *     returns : void.
     *      throws : TobyParseException on syntax errors, etc...
     */
    {
        int lvalue;    // index of comparable value to left of equality op.
        int rvalue;    // index of comparable value to right of equality op.

        lvalue = findLeftValue(src, tokIndex - 1);
        rvalue = findRightValue(src, tokIndex + 1);

        if (determineType(src[lvalue]) != determineType(src[rvalue]))
            TobyParseException.throwException(PROCERR_TYPE_MMATCH, null);

        if (src[lvalue].equals(src[rvalue]))
            src[tokIndex] = KEYWORD_TRUE;
        else
            src[tokIndex] = KEYWORD_FALSE;

        src[lvalue] = src[rvalue] = null;
    } // dealWithEquals


    private int determineType(String typeVal) throws TobyParseException
    /**
     * Determine the type of Intrinsic needed to contain a value stored
     *  in (typeVal). Returns the index in (intrinsicTable) of determined
     *  type, throws otherwise.
     *
     *
     *   !!! this is inefficient. Change!
     */
    {
        String typStr = null;

        try
        {
            typStr = new NumberIntrinsic(null, typeVal).getType();
        } // try
        catch (TobyParseException tpe)
        {
            try
            {
                typStr = new BooleanIntrinsic(null, typeVal).getType();
            } // try
            catch (TobyParseException tpe2)
            {
                TobyParseException.throwException(PROCERR_SYNTAX_ERR, null);
            } // catch
        } // catch

        return(searchTable(intrinsicTable, typStr));
    } // determineType


    private void calculate(String[] src) throws TobyParseException
    /**
     * This is the really recursive part. This function checks a String
     *  array, and find the operator with the highest precedence, and
     *  deals with it. Once an operation is calculated, the operator and
     *  operands that are finished being examined are set to (null). Other
     *  functions also set array elements to (null) to help facilitate
     *  this method.
     *
     * This function may be recursively called to deal with groupings
     *  such as parenthesis and array operators. Also, other TOBY procedures
     *  may be called as a result of this function, which will call this
     *  function again. It's hard to trace, but it's the best way I've
     *  found for doing this.
     *
     * This method returns when all the tokens in (src) have been set to
     *  (null). It will also return if there is only one token remaining,
     *  but it will throw if that token is an operator.
     *
     *     params : src == array of stuff to calculate.
     *    returns : void.
     *     throws : TobyParseException on panic.
     */
    {
        int tokIndex;

        do
        {
            tokIndex = findHighestOperator(src);

            if (tokIndex >= 0)                       // any operators left?
            {
                if (src[tokIndex].equals(OPCODE_LPAREN))
                    dealWithParens(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_MULTIPLY))
                    dealWithMultiply(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_DIVIDE))
                    dealWithDivide(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_MODULO))
                    dealWithModulo(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_ADD))
                    dealWithAdd(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_SUBTRACT))
                    dealWithSubtract(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_EQUALS))
                    dealWithEquals(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_ASSIGNMENT))
                    dealWithAssignment(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_GREATER))
                    dealWithGreaterThan(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_LESS))
                    dealWithLessThan(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_GREATEREQL))
                    dealWithGreaterThanOrEqualTo(src, tokIndex);

                else if (src[tokIndex].equals(OPCODE_LESSEQL))
                    dealWithLessThanOrEqualTo(src, tokIndex);

                    /*
                     * if these below are found, there's a problem, since
                     *  they should get set to null elsewhere before
                     *  being found by calculate()...
                     */

                else if (src[tokIndex].equals(OPCODE_RPAREN))
                {
                    TobyParseException.throwException(PROCERR_NO_LPAREN,
                                                      null);
                } // else if
            } // if

            else if (tokIndex == -1)     // tokens, but no operators.
            {
                // !!! will this need to throw? Use stderr for debugging...
                // !!! if no throw, change definition of findHighestPrecedence
                // !!! and remove this else if...
                System.err.println("Tokens, but no operators!");
                System.err.print  ("     ");
                for (int x = 0; x < src.length; x++)
                    System.err.print("\"" + src[x] + "\" ");
                System.err.println();
            } // else if
        } while (tokIndex >= 0);
    } // calculate


    private void printSrc(String msg, String[] src)
    // !!! debugging code:  lose this before shipping!
    {
        int i;

        System.err.println(msg);
        System.err.print("Tokens : ");

        for (i = 0; i < src.length; i++)
            System.err.print("[" + src[i] + "] ");

        System.err.println();
    } // printSrc


    private void cleanupStack()
    /**
     * Pops all elements up to and including the last TOBY procedure call
     *  from the stack. Notifies registered VarWatchers of variable
     *  destruction.
     *
     *     params : void.
     *    returns : void.
     */
    {
        Object obj;

        do
        {
            obj = stack.pop();

            if (obj instanceof Intrinsic)
                notifyVarDestroyed((Intrinsic) obj);

        } while (!(obj instanceof TobyProcedure));
    } // cleanupStack



    private void moveArgsToStack(TobyProcedure proc, Vector args)
                                        throws TobyParseException
    /*
     * This checks and moves the values present in a procedure's args
     *  to the stack so they may be used in the procedure.
     *
     *     params : proc == procedure that arguments apply to.
     *              args == args to be added to stack.
     *    returns : void.
     *     throws : TobyParseException on error.
     */
    {
        int i;
        int argCount = args.size();
        Intrinsic var;

        for (i = 0; i < argCount; i++)
        {
            var = createTobyVar(proc.getParamType(i),
                                proc.getParamName(i),
                                stack);
            setTobyVar(var, (String) args.elementAt(i));
        } // for

        args.removeAllElements();
        args.trimToSize();
    } // moveArgsToStack



    private int addLocalsToStack(TobyProcedure proc) throws TobyParseException
    /**
     * Add any variables defined in the scope of the procedure to the stack.
     *
     *     params : proc == TOBY function with potential locals.
     *    returns : line number of first non-local-defining line.
     *     throws : TobyParseException on syntactic error.
     */
    {
        String[] src;
        int i;
        int n;
        boolean getOut = false;
        int max = proc.sourceLineCount();

        for (i = 0; (i < max) && (!getOut); i++)
        {
            src = proc.getSourceLine(i);

            if (searchTable(intrinsicTable, src[0]) != -1)
            {
                if (src.length != 2)   // !!! lift this restriction?
                {
                    TobyParseException.throwException(PROCERR_SYNTAX_ERR,
                                                      null, 0);
                } // if

                    // it's cool; create the var.
                createTobyVar(src[0], src[1], stack); 
            } // if
            else
            {
                i--;
                getOut = true;
            } // else

            for (n = 0; n < src.length; n++)   // remove references...
                src[n] = null;                 // !!! need this?!
        } // for

        return(i);
    } // addLocalsToStack



    private boolean tokensRemain(String[] src)
    /**
     * Check to see if any tokens remain on a line.
     *
     *    params : src == array of potential tokens.
     *   returns : boolean true if tokens remain, false otherwise.
     */
    {
        boolean retVal;

        try
        {
            findRightValue(src, 0);
            retVal = true;           // no throw? Then tokens remain.
        } // try
        catch (TobyParseException tpe)
        {
            retVal = false;
        } // catch

        return(retVal);
    } // tokensRemain


    private void updateSourceTrace(TobyProcedure proc, int srcLine)
    /**
     *  This method notifies any registered components that the
     *   interpreter is now working on a new line of source code.
     *
     *     params : proc == TobyProcedure being parsed.
     *              srcLine == line of code in (proc) being parsed.
     *    returns : void.
     */
    {
        notifySourceLineUpdated(proc.getActualSourceLine(srcLine));
    } // updateSourceTrace


/*    private void resizeCopyImageOnTheFly()
     *
     * Make a new copyImage, and copy the current one to the dead center of
     *  it for on-the-fly continuing...relocate turtle, too.
     *
    {
        int width = getWidth();
        int height = getHeight();
        Image tmp = createImage(width, height);
        Graphics tmpG = tmp.getGraphics();
        int offsetX = (width - copyImage.getWidth(this)) / 2;
        int offsetY = (height - copyImage.getHeight(this)) / 2;

        tmpG.setColor(getBackground());
        tmpG.drawRect(0, 0, width, height);
        tmpG.drawImage(copyImage, offsetX, offsetY, this);
        g[GRAPHICS_COPY].dispose();
        g[GRAPHICS_COPY] = tmpG;

        turtle.setXY(turtle.getX() + offsetX, turtle.getY() + offsetY);

        copyImage.flush();
        copyImage = tmp;
    } // resizeCopyImageOnTheFly
*/

    private Intrinsic runCode(TobyProcedure proc)
                                            throws TobyParseException
    /**
     *  Entry point for parsing TOBY functions. run() calls this to
     *   parse the mainline, and may be called recursively during that
     *   interpretation. Assume that runCode() is being executed in its own
     *   thread, so the 1/10 second rule doesn't apply, and the queue won't
     *   lag.
     *
     *     params : proc == TOBY function to parse.
     *              args == arguments to the function. null if none.
     *    returns : String representing possible TOBY return value. null if
     *              returns void.
     *     throws : TobyParseException on pukes.
     */
    {
        String[] src;
        int srcLine = 0;
        int lineChange;
        Intrinsic retVal = null;


            /*
             * This first "try" grabs TobyParseExceptions and, if the
             *  procName associated with the exception is (null), then
             *  this TobyProcedure's name is plugged in. This means that
             *  the last TobyProcedure called, regardless of what threw
             *  the exception, will have its name inserted.
             */
        try
        {
            srcLine = addLocalsToStack(proc);

            while (srcLine < proc.sourceLineCount())
            {
                Thread.yield();   // just to be nice.  :)

                if (terminateCode)
                {
                    TobyParseException tpe = new TobyParseException(null);
                    tpe.terminated = true;
                    throw(tpe);
                } // if

                src = proc.getSourceLine(srcLine);

                updateSourceTrace(proc, srcLine);   // update trace display.
                delayInterpretation();              // for tracing/stepping...

                try
                {
                    lineChange = dealWithFlowControl(proc, src, srcLine);

                    if (lineChange != srcLine)
                        srcLine = lineChange;
                    else
                    {
                        calculate(src);   // recursive.

                        if (tokensRemain(src))
                        {
                            TobyParseException.throwException(
                                                        PROCERR_SYNTAX_ERR,
                                                        proc.getProcName());
                        } // if

                        srcLine++;
                    } // else
                } // try
                catch (Intrinsic retCall)
                {
                    retVal = retCall;
                    srcLine = proc.sourceLineCount() + 1;  // breaks loop.
                } // catch
            } // while

        } // try
        catch (TobyParseException tpe)
        {
            if (tpe.procName == null)
            {
                tpe.procName = proc.getProcName();
                tpe.errLine = proc.getActualSourceLine(srcLine);
            } // if
            throw(tpe);     // rethrow it, with a procedure name attached.
        } // catch

        if (retVal == null)
            retVal = nothing;

        cleanupStack();
        return(retVal);
    } // runCode


        // Runnable implementation ... 

    public void run()
    {
        TobyProcedure mainLine = findProcedure(PROCNAME_MAINLINE);

        if (mainLine != null)
        {
            initGraphics();
            codeRunning = true;
            terminateCode = false;

            turtle.setPenUp(false);
            turtle.setPenColor(turtle.defaultPenColor());
            turtle.homeTurtle(this, g[GRAPHICS_SCREEN], copyImage);
            turtle.setVisible(true, g[GRAPHICS_SCREEN], copyImage);
            notifyBeginInterpretation();

            stack.removeAllElements();   // clear the stack.
            stack.push(mainLine);

            try
            {
                runCode(mainLine);    // !!! bitch if main returns value?
            } // try
            catch (TobyParseException tpe)
            {
                if (tpe.terminated == false)
                {
                    notifySourceError(tpe.errLine);
                    displayParseException(tpe);
                } // if
            } // catch

            deleteProcedures();   // get rid of code, now that it has ran...
            codeRunning = false;
            terminateCode = false;
            notifyEndInterpretation();
            deInitGraphics();
        } // if
    } // run


        // ComponentListener implementation...

    public void componentResized(ComponentEvent e)
    {
        if (codeRunning == false)
        {
            if (copyImage != null)
                copyImage.flush();
            cleanup();
        } // if
    } // componentResized

    public void componentMoved(ComponentEvent e) {}
    public void componentShown(ComponentEvent e) {}
    public void componentHidden(ComponentEvent e) {}

} // TobyInterpreter


// end of TobyInterpreter.java ...


