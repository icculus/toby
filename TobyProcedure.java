/**
 *  This class contains individual TOBY procedures. All parsing of a given
 *   procedure is done here. Once all the TobyProcedures are created (and
 *   therefore all the program's code has been parsed for bogus syntax), the
 *   TobyInterpreter class will perform the actual execution of the procedures,
 *   starting with the TobyProcedure named "main", and interprets code line
 *   by line by requesting strings from this procedure. All sorts of other
 *   nifty information can be retrieved from these objects, too.
 *
 *    Copyright (c) Lighting and Sound Technologies, 1997.
 *     Written by Ryan C. Gordon.
 */

import java.io.*;
import java.util.Vector;


public class TobyProcedure
{
        // Constant error messages...
    public final static String PROCERR_EXPECTED_END  = "Expected ENDFUNCTION";

        // Instance members...
    private String   procName       = null;    // Declared name of proc.
    private String[] paramNames     = null;    // array of parameter names.
    private String[] paramTypes     = null;    // array of parameter types.
    private String   returnType     = null;    // Intrinsic type of returns.
    private Vector   sourceCode     = null;    // Actual code to run.
    private Vector   sourceLineNums = null;    // Line num in actual source.


        // Methods...

    public TobyProcedure(SaneTokenizer sToker) throws TobyParseException,
                                                      IOException
    /**
     *  Constructor; build procedure by setting and first-pass parsing
     *   code.
     *
     *  In programming language lingo, all functions consist of a 
     *   DECLARATION and a DEFINITION.
     *
     *  For example, in Java, the declaration is the header that tells
     *   what the function is called, what parameters it takes, and
     *   what it returns. Other attributes, like what it throws, if it is
     *   static, final, etc...are DECLARED at that point in the program.
     *
     *  The definition is the actual source code that makes up the body of
     *   the function. The algorithm is DEFINED at that point in the program.
     *
     *          public void moveTo(int x, int y)      // <-- declaration.
     *          {                                     // <-\
     *              moreCode(y, x + 10);              // <  --- definition.
     *          } // moveTo                           // <-/
     *
     *    params : sToker == This tokenizer should be just past the
     *                       KEYWORD_FUNC token, leaving us at the
     *                       function name.
     *   returns : Constructor; void.
     *    throws : TobyParseException; on panic.
     */
    {
        setDeclaration(sToker);
        setDefinition(sToker);
    } // Constructor


    public void finalize() throws Throwable
    {
        freeResources();
        super.finalize();
    } // finalize


    public String getProcName()
    /**
     * Gives access to procName by value, and not by reference.
     *
     *    params : void.
     *   returns : copy of procName.
     */
    {
        return(new String(procName));
    } // getProcName


    public String[] getSourceLine(int lineNum)
    {
        String[] arrayToCopy = (String[]) sourceCode.elementAt(lineNum);
        String[] retVal = new String[arrayToCopy.length];

        System.arraycopy(arrayToCopy, 0, retVal, 0, arrayToCopy.length);

        return(retVal);
    } // getSourceLine


    public int getActualSourceLine(int srcLine)
    {
        return(((Integer) sourceLineNums.elementAt(srcLine)).intValue());
    } // getActualSourceLine


    public int sourceLineCount()
    {
        return(sourceCode.size());
    } // sourceLineCount


    public int getParamCount()
    {
        return(paramNames.length);
    } // getParamCount


    public String getParamName(int paramIndex)
    {
        return(paramNames[paramIndex]);
    } // getParamName


    public String getParamType(int paramIndex)
    {
        return(paramTypes[paramIndex]);
    } // getParamType


    public String getReturnType()
    {
        return(returnType);
    } // getReturnType


    public void freeResources()
    /**
     *  This method frees up all possible resources. Expect this
     *   object to be unusable after this call. Everything gets
     *   set to (null), so they'll be garbage collectable.
     *    
     *      params : void.
     *     returns : void.
     */
    {
        int i;
        int n;
        String[] strTmp;

        procName = null;

        if (sourceCode != null)
        {
            for (i = sourceCode.size() - 1; i >= 0; i--)
            {
                strTmp = (String[]) sourceCode.elementAt(i);
                for (n = 0; n < strTmp.length; n++)
                    strTmp[n] = null;
            } // for
            sourceCode.removeAllElements();
            sourceCode.trimToSize();
            sourceCode = null;
            sourceLineNums.removeAllElements();
            sourceLineNums.trimToSize();
            sourceLineNums = null;
        } // if

        if (paramNames != null)
        {
            for (i = 0; i < paramNames.length; i++)
                paramNames[i] = paramTypes[i] = null;

            paramNames = null;
            paramTypes = null;
        } // if
    } // freeResources


    private void throwSyntaxError(int lineNum) throws TobyParseException
    /**
     * This is just here so the source code is cleaner. (A lot of
     *  the TobyParseExceptions are "syntax error" messages, and they
     *  take like three lines to fit the whole throw on the screen. Ugh.
     *
     *     params : p == procedure name.
     *              l == line number.
     *    returns : Technically, void. But...
     *     throws :  ...always throws TobyParseException.
     */
    {
        String procStr = ((procName == null) ? "???" : procName);
        TobyParseException.throwException(TobyInterpreter.PROCERR_SYNTAX_ERR,
                                          procStr, lineNum);
    } // throwSyntaxError


    public String[] vectorToStrArray(Vector v)
    {
        String[] retVal = new String[v.size()];
        v.copyInto(retVal);
        v.removeAllElements();
        v.trimToSize();
        return(retVal);
    } // vectorToStrArray



    private void setDeclaration(SaneTokenizer sToker)
                                 throws TobyParseException, IOException
    /**
     *  This method parses the TOBY function declaration into
     *   something this class can deal with.
     *
     *     params : sToker == Tokenizer that is pointing to new func's name.
     *    returns : void.
     *     throws : TobyParseException on choke.
     */
    {
        setProcName(sToker);
        setParameters(sToker);
        setReturnType(sToker);
        verifyEndLine(sToker);
    } // setDeclaration


    private void verifyEndLine(SaneTokenizer sToker) throws TobyParseException,
                                                            IOException
    /**
     *  Make sure next token is the end of the line in the tokenizer, and
     *   throw if not. Returns without a throw if it is.
     *  !!! check to see where else we can use this function...
     */
    {
        int lineNum = sToker.lineno();

        if (sToker.nextToken() != SaneTokenizer.TT_EOL)
            throwSyntaxError(lineNum);
    } // verifyEndLine


    private void setProcName(SaneTokenizer sToker) throws TobyParseException,
                                                          IOException
    /**
     * Assuming that (sToker) should be staring directly at the function
     *  name in the declaration, this method will make sure all is copacetic
     *  and set instance variable (procName) accordingly.
     *
     *    params : sToker == see above.
     *   returns : void.
     *    throws : TobyParseException on syntax errors.
     */
    {
        int lineNum = sToker.lineno();

        if (sToker.nextToken() != SaneTokenizer.TT_WORD)
            throwSyntaxError(lineNum);   // !!! "???" as constant?

        if (TobyInterpreter.isValidIdentifier(sToker.sval) == false)
        {
            TobyParseException.throwException(
                                           TobyInterpreter.PROCERR_BAD_IDENT,
                                           sToker.sval, lineNum);
        } // if

        procName = sToker.sval;
    } // setProcName


    private void setParameters(SaneTokenizer sToker) throws TobyParseException,
                                                            IOException
    /**
     *  This method is called right after setProcName(), and (sToker) should
     *   return a '(' as the next token. This will be the start of this
     *   TOBY function's parameter list. Syntax is as follows:
     *   
     *      function myFunc(varType varName1, [...], varType varNameN)
     *
     *  If the parameter list is void (has no parameters) then the syntax is:
     *
     *      function myFunc()
     *
     *  Have fun.
     *
     *    params : sToker == see above.
     *   returns : void.
     *    throws : TobyParseException when syntax is all crudded up.
     */
    {
        int lineNum = sToker.lineno();
        Vector paramVect;

        if (sToker.nextToken() != SaneTokenizer.TT_WORD)
            throwSyntaxError(lineNum);

        if (!sToker.sval.equals(TobyInterpreter.OPCODE_LPAREN))
            throwSyntaxError(lineNum);

        paramVect = new Vector();

        sToker.nextToken();
        if (!sToker.sval.equals(TobyInterpreter.OPCODE_RPAREN)) // no params?
        {
            sToker.pushBack();
            paramParseLoop(sToker, paramVect, lineNum);
        } // if

        vectToParamArrays(paramVect);
    } // setParameters


    private void paramParseLoop(SaneTokenizer sToker,
                                Vector paramVect,
                                int lineNum)
                                     throws TobyParseException, IOException
    /**
     * This method is called if the parameters declaration is not void.
     *
     *     params : sToker == you know.
     *              paramVect == Vector to fill. See vectToParamArrays()...
     *              lineNum == line in source code we're parsing.
     *    returns : void.
     *     throws : TobyParseException on syntax errors.
     */
    {
        int loopCount = 1;
        boolean getOut = false;

        do
        {
            if (sToker.nextToken() != SaneTokenizer.TT_WORD)
                throwSyntaxError(lineNum);

            getOut = handleParamElements(loopCount, paramVect,
                                         lineNum, sToker.sval);

            if (loopCount == 3)
                loopCount = 1;
            else
                loopCount++;
        } while (!getOut);
    } // paramParseLoop



    private boolean handleParamElements(int loopCount,
                                        Vector paramVect,
                                        int lineNum,
                                        String sval) throws TobyParseException
    /**
     * This handles individual tokens (in (sval)) from the parameter
     *  declaration.
     *
     * Variable types and variable names get added to (paramVect),
     *  other elements get junked, and syntax errors cause throws.
     *  Refer to the individual "case" statements to see what each value
     *  of (loopCount) signifies, and refer to getParameters() for the
     *  correct syntax of a TOBY function declaration.
     *
     *    params : see above. lineNum is line in source code this is on.
     *   returns : (false) if not end of declaration, (true) otherwise.
     *    throws : TobyParseException on syntactical smegma.
     */
    {
        boolean retVal = false;

        switch (loopCount)  // which part of param definition is this?
        {
            case 1:     // variable type.
                if (!TobyInterpreter.isIntrinsic(sval))
                {
                    TobyParseException.throwException(
                                        TobyInterpreter.PROCERR_NOT_A_VAR,
                                        procName, lineNum);
                } // if

                paramVect.addElement(sval);
                break;

            case 2:     // variable name.
                if (!TobyInterpreter.isValidIdentifier(sval))
                {
                    TobyParseException.throwException(
                                        TobyInterpreter.PROCERR_NOT_A_VAR,
                                        procName, lineNum);
                } // if

                paramVect.addElement(sval);
                break;

            case 3:     // either ")" ... "," ... or error.
                if (sval.equals(TobyInterpreter.OPCODE_RPAREN))
                    retVal = true;
                else if (!sval.equals(TobyInterpreter.OPCODE_SEPARATOR))
                    throwSyntaxError(lineNum);
                break;
        } // switch

        return(retVal);
    } // handleParamElements


    private void vectToParamArrays(Vector paramVect)
    /**
     *  In setParameters(), we fill a Vector with Strings, alternating
     *   variable types and variable names. Here, we split them into
     *   two arrays, and shrink the Vector to zero elements.
     *
     *      params : paramVect : Vector to split.
     *     returns : void.
     */
    {
        int i;
        int n;
        int paramCount = paramVect.size() / 2;

        paramNames = new String[paramCount];
        paramTypes = new String[paramCount];

        for (i = 0, n = 0; i < paramCount; i++, n += 2)
        {
            paramTypes[i] = (String) paramVect.elementAt(n);
            paramNames[i] = (String) paramVect.elementAt(n + 1);
        } // for

        paramVect.removeAllElements();
        paramVect.trimToSize();
    } // vectToParamArrays


    private void setReturnType(SaneTokenizer sToker) throws TobyParseException,
                                                            IOException
    /** !!! comment. Make throws below more specific error messages. */
    {
        int lineNum = sToker.lineno();

        if (sToker.nextToken() != SaneTokenizer.TT_WORD)
            throwSyntaxError(lineNum);

        if (sToker.sval.equals(TobyInterpreter.KEYWORD_RETURNS) == false)
            throwSyntaxError(lineNum);

        if (sToker.nextToken() != SaneTokenizer.TT_WORD)
            throwSyntaxError(lineNum);

        if (TobyInterpreter.isIntrinsic(sToker.sval))
            returnType = sToker.sval;
        else
            throwSyntaxError(lineNum);
    } // setReturnType


    public void removeBlankLines(SaneTokenizer sToker) throws IOException
    /**
     *   Take the blank lines out of a Stream.
     *
     *    next call to sToker.nextToken() will grab a non-EOL
     *    token.
     *
     *      params : sToker == SaneTokenizer to trim lines from...
     *     returns : void.
     */
    {
        do
        {
            sToker.nextToken();
        } while (sToker.ttype == SaneTokenizer.TT_EOL);

        sToker.pushBack();     // put last non-EOL token back in Stream.
    } // removeBlankLines



    private boolean isThisEnd(SaneTokenizer sToker) throws TobyParseException,
                                                           IOException
    /**
     * Ok, this following piece of logic is a little dense.
     *
     * If this method is called, then we are somewhere in the midst of
     *  parsing a TOBY function. This is promised not to be a blank line.
     *  Therefore, we either expect TOBY opcodes, or a KEYWORD_ENDFUNC token,
     *  signifying the end of the TOBY function. Hitting an EOF at this
     *  point is not karmically cool.
     *
     * First, we grab a token. If it's a KEYWORD_ENDFUNC, we make sure
     *  it's the only token on the line. If not, we throw. If so,
     *  we return (true) to signify the end of the TOBY procedure.
     *  If the token wasn't a KEYWORD_ENDFUNC in the first place, we
     *  pushBack(), so the rest of the algorithm can deal with it, and
     *  return (false).
     *
     * Oh, and if we hit a KEYWORD_BEGINFUNC, we throw, since we
     *  are already inside a function...
     *
     * If there was no token in the first place, due to an EOF,
     *  then we throw, expecting a KEYWORD_ENDFUNC, if nothing else.
     *
     *      params : see above.
     *     returns : see above.
     *      throws : TobyParseException on premature EOF or syntax problems.
     */
    {
        int lineNum = sToker.lineno();

        sToker.nextToken();
        if (sToker.ttype == SaneTokenizer.TT_WORD)
        {
            if (sToker.sval.equals(TobyInterpreter.KEYWORD_ENDFUNC))
            {
                if (sToker.nextToken() != SaneTokenizer.TT_EOL)
                    throwSyntaxError(lineNum);
                else
                    return(true);   // end of procedure.
            } // if

            else if (sToker.sval.equals(TobyInterpreter.KEYWORD_BEGINFUNC))
            {
                TobyParseException.throwException(PROCERR_EXPECTED_END,
                                                  procName);
            } // else if

            else
                sToker.pushBack();
        } // if

        else if (sToker.ttype == SaneTokenizer.TT_EOF)
            TobyParseException.throwException(PROCERR_EXPECTED_END, procName);

        return(false);   // if we didn't throw, must not be end of function.
    } // isThisEnd


    private String[] parseLine(SaneTokenizer sToker) throws TobyParseException,
                                                            IOException
    /**
     * This method Tokenizes a String, and checks other syntax issues.
     *
     *      params : sToker == see setDefinition()...
     *     returns : Array of tokens, sorted. null if this is the end
     *                procedure token.
     *      throws : TobyParseException == on panic.
     */
    {
        Vector codeLine;
        String[] retVal;
        String[] tokenCluster = null;
        int i;

        if (isThisEnd(sToker) == true)
            return(null);

            /*
             * Now, we read tokens into a Vector, until we hit the
             *  end of the line...
             */

        codeLine = new Vector();
        do
        {
            sToker.nextToken();
            if (sToker.ttype != SaneTokenizer.TT_EOL)
            {


/* !!! *****************************************************************
               if (sToker.sval.equals(OPCODE_ASSIGNMENT))
                {
                    if ((sToker.nextToken() == TT_WORD) &&
                        (sToker.sval.equals(OPCODE_ASSIGNMENT))
                        codeLine.addElement(OPCODE_EQUALS);
                    {
                        codeLine.addElement(OPCODE_ASSIGNMENT);
                        sToker.pushBack();
                    } // else
                } // if
                else
                    codeLine.addElement(sToker.sval);
       ******************************************************************/


                    codeLine.addElement(sToker.sval);
            } // if
        } while (sToker.ttype != SaneTokenizer.TT_EOL);

        retVal = vectorToStrArray(codeLine);
        return(retVal);
    } // parseLine



    private void setDefinition(SaneTokenizer sToker)
                                       throws TobyParseException, IOException
    /**
     *  Sets up source code-containing variables.
     *
     *    params : sToker == Tokenizer holding source code.
     *   returns : void.
     *    throws : TobyParseException for problematic stuff.
     */
    {
        String[] rc;
        boolean getOut = false;
        int readLine;

        sourceCode = new Vector();
        sourceLineNums = new Vector();

        do
        {
            removeBlankLines(sToker);
            readLine = sToker.lineno();
            rc = parseLine(sToker);
            if (rc != null)                     // null == end of function.
            {
                sourceCode.addElement(rc);
                sourceLineNums.addElement(new Integer(readLine));
            } // if
        } while ((rc != null) && (sToker.ttype != SaneTokenizer.TT_EOF));

        if (sToker.ttype == SaneTokenizer.TT_EOF)
            TobyParseException.throwException(PROCERR_EXPECTED_END, procName);
        else
            sourceCode.trimToSize();
    } // setDefinition

} // TobyProcedure

// end of TobyProcedure.java ...

