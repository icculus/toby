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

package last.toby.interpreter;
import java.util.*;
import last.toby.exceptions.*;

    /**
     * An abstract base class for subroutines in a given language. This just
     *  defines the basic attributes that programming subroutines tend
     *  to have in common.
     *
     *  @author Ryan C. Gordon.
     */
public class FunctionLogicContext extends LogicContext
{
    protected String functionName = null;
    protected Vector locals = new Vector();
    protected GlobalLogicContext globalLogicContext = null;
    protected Vector parameters = new Vector();

    public FunctionLogicContext(int sourceLine, String funcName)
    {
        super(sourceLine);
        this.functionName = funcName;
    } // Constructor

    public void addParameter(Object type, String identifier, Intrinsic newParam)
    {
        newParam.setIdentifier(identifier);
        parameters.addElement(newParam);
        addScopedSymbol(type, identifier);
    } // addParameter

    public void addLocal(Object type, String identifier, Intrinsic newLocal)
    {
        newLocal.setIdentifier(identifier);
        locals.addElement(newLocal);
        addScopedSymbol(type, identifier, 0);
    } // addLocal


        /**
         * Gives access to procedure name.
         *
         *   @return The procedure's name.
         */
    public String getFuncName()
    {
        return(functionName);
    } // getFuncName


        /**
         * Get access to the number of parameters this routine expects.
         */
    public int getParamCount()
    {
        return(parameters.size());
    } // getParamCount


    public void freeResourcesImpl()
    {
        if (locals != null)
        {
            locals.clear();
            locals = null;
        } // if
    } // freeResources


    public void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        this.globalLogicContext = glob;

        linkVectorElements(locals, glob);
        linkVectorElements(parameters, glob);
    } // link


    protected void addLocalsToStack()
    {
        int max = locals.size();

        for (int i = 0; i < max; i++)
        {
            Intrinsic var = ((Intrinsic) locals.elementAt(i)).getCopy();
//            _D("addLocalsToStack", "adding " +var.getClass().getName()+ "...");
            globalLogicContext.pushLocal(var);
        } // while
    } // addLocalsToStack


    protected void removeLocalsFromStack()
    {
        globalLogicContext.popMultipleLocals(locals.size());
    } // removeLocalsFromStack


    public Intrinsic executeImpl() throws FlowException
    {
        Intrinsic retVal = NothingIntrinsic.nothing;

        addLocalsToStack();

        try
        {
            defaultExecuteImpl();
        }
        catch (ReturnException re)
        {
            retVal = (Intrinsic) re.getReturnValue();
        } // catch
        catch (FlowException fe)
        {
            if (fe.procName == null)
                fe.procName = this.functionName;
            throw(fe);  // pass it on.
        } // catch
        finally
        {
            removeLocalsFromStack();
        } // finally

        return(retVal);
    } // execute

} // FunctionLogicContext

// end of FunctionLogicContext.java ...

