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

import last.toby.exceptions.*;
import last.toby.util.*;
import java.util.Stack;

    /**
     *  !!! comment me!
     *
     *  @author Ryan C. Gordon.
     */
public class GlobalLogicContext extends LogicContext
{
    protected Object screen = null;
    protected String mainlineName = null;
    protected Stack localStack = new Stack();
    protected Stack globalStack = new Stack();
    protected FunctionLogicContext mainline = null;
    protected boolean caseSensitive = true;
    private long execStartTime = -1;
    private long execTotalTime = -1;
    private static Stack stackFrames = new Stack();
    private int currentStackFrame = 0;

    public GlobalLogicContext(String mainlineName)
    {
        super(0);
        this.mainlineName = mainlineName;

        // !!! this should probably be done by the parsers.
        loadStandardFunctions();
    } // Constructor


    public void setCaseSensitive(boolean caseSensitive)
    {
        this.caseSensitive = caseSensitive;
    } // setCaseSensitive


    public boolean getCaseSensitive()
    {
        return(this.caseSensitive);
    } // getCaseSensitive


    public long getTotalExecTime()
    {
        long retVal = this.execTotalTime;

        if (this.execTotalTime < 0)
        {
            if (this.execStartTime < 0)
                retVal = 0;
            else
                retVal = System.currentTimeMillis() - execStartTime;
        } // if

        return(retVal);
    } // getTotalExecTime


        // !!! this should probably be done by the parsers.
    protected void loadStandardFunctions()
    {
        addChild(new last.toby.interpreter.stdfunctions.UseTurtle());
        addChild(new last.toby.interpreter.stdfunctions.AddTurtle());
        addChild(new last.toby.interpreter.stdfunctions.RemoveTurtle());
        addChild(new last.toby.interpreter.stdfunctions.RemoveAllTurtles());
        addChild(new last.toby.interpreter.stdfunctions.GoForward());
        addChild(new last.toby.interpreter.stdfunctions.GoBackward());
        addChild(new last.toby.interpreter.stdfunctions.SetAngle());
        addChild(new last.toby.interpreter.stdfunctions.TurnRight());
        addChild(new last.toby.interpreter.stdfunctions.TurnLeft());
        addChild(new last.toby.interpreter.stdfunctions.SetPenUp());
        addChild(new last.toby.interpreter.stdfunctions.SetPenDown());
        addChild(new last.toby.interpreter.stdfunctions.SetPenColor());
        addChild(new last.toby.interpreter.stdfunctions.SetPenColorRGB());
        addChild(new last.toby.interpreter.stdfunctions.HideTurtle());
        addChild(new last.toby.interpreter.stdfunctions.ShowTurtle());
        addChild(new last.toby.interpreter.stdfunctions.HideAllTurtles());
        addChild(new last.toby.interpreter.stdfunctions.ShowAllTurtles());
        addChild(new last.toby.interpreter.stdfunctions.GetTurtleSpaceWidth());
        addChild(new last.toby.interpreter.stdfunctions.GetTurtleSpaceHeight());
        addChild(new last.toby.interpreter.stdfunctions.GetTurtleCount());
        addChild(new last.toby.interpreter.stdfunctions.GetTurtleX());
        addChild(new last.toby.interpreter.stdfunctions.GetTurtleY());
        addChild(new last.toby.interpreter.stdfunctions.SetTurtleX());
        addChild(new last.toby.interpreter.stdfunctions.SetTurtleY());
        addChild(new last.toby.interpreter.stdfunctions.SetTurtleXY());
        addChild(new last.toby.interpreter.stdfunctions.HomeTurtle());
        addChild(new last.toby.interpreter.stdfunctions.HomeAllTurtles());
        addChild(new last.toby.interpreter.stdfunctions.Random());
        addChild(new last.toby.interpreter.stdfunctions.Round());
        addChild(new last.toby.interpreter.stdfunctions.Pause());
        addChild(new last.toby.interpreter.stdfunctions.CleanupTurtleSpace());
        addChild(new last.toby.interpreter.stdfunctions.SetFence());
        addChild(new last.toby.interpreter.stdfunctions.EnableFence());
        addChild(new last.toby.interpreter.stdfunctions.DisableFence());
    } // loadStandardFunctions


    public Object getScreen()
    {
        return(this.screen);
    } // getScreen


    public void setScreen(Object newScreen)
    {
        this.screen = newScreen;
    } // setScreen


    public boolean verifyNewChildType(NodeTree child)
    {
        boolean retVal = false;
        if (child instanceof LogicContext)
            retVal = true;
        return(retVal);
    } // verifyNewChildType

    public FunctionLogicContext findFunction(String funcName)
                                            throws ParseException
    {
        //_D("GlobalLogicContext.findFunction",
        //     "Looking for [" + funcName + "]...");
        //_D("GlobalLogicContext.findFunction",
        //     "(case sensitive == {" + this.caseSensitive + "}).");

        if (this.caseSensitive == false)
            funcName = funcName.toLowerCase();

            // !!! code duplication...find a better way.

        if (isLocked() == false)
        {
            int i;
            int max = kidsVect.size();

            for (i = 0; i < max; i++)
            {
                Object func;
                func = kidsVect.elementAt(i);
                if (func instanceof FunctionLogicContext)
                {
                    String fName = ((FunctionLogicContext) func).getFuncName();

                    //_D("GlobalLogicContext.findFunction",
                    //    "Looking at [" + fName + "] ...");

                    if (this.caseSensitive == false)
                        fName = fName.toLowerCase();

                    if (fName.equals(funcName))
                        return((FunctionLogicContext) func);
                } // if
            } // for

            return(null);
        } // if

        else   // locked version (for interpretation time) is faster...
        {
            int i;
            int max = children.length;

            for (i = 0; i < max; i++)
            {
                Object func = children[i];

                if (func instanceof FunctionLogicContext)
                {
                    String fName = ((FunctionLogicContext) func).getFuncName();

                    //_D("GlobalLogicContext.findFunction",
                    //    "Looking at [" + fName + "] ...");

                    if (this.caseSensitive == false)
                        fName = fName.toLowerCase();

                    if (fName.equals(funcName))
                        return((FunctionLogicContext) func);
                } // if
            } // for

            return(null);
        } // else
    } // findFunction


    protected void freeResourcesImpl()
    {
        mainlineName = null;
        localStack.clear();
        localStack.trimToSize();
        globalStack.clear();
        globalStack.trimToSize();
        localStack = globalStack = null;
    } // freeResourcesImpl

    public void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        if (this != glob)
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

        this.mainline = findFunction(mainlineName);
        if (this.mainline == null)
            ParseException._throw(TobyLanguage.NO_MAINLINE);
    } // link

    public int pushLocal(Intrinsic local)
    {
        localStack.push(local);
        return(localStack.size() - 1);
    } // pushLocal

    public int addGlobal(Object type, String identifier, Intrinsic global)
    {
        addScopedSymbol(type, identifier);
        global.setIdentifier(identifier);
        globalStack.push(global);

//        _D("addGlobal", "Added [" + identifier + "] at index (" +
//                            (globalStack.size() - 1) + ").");

        return(globalStack.size() - 1);
    } // addGlobal

    public Intrinsic localFromTopOfStack(int offset)
    {
        int pos = localStack.size() - offset - 1;
        return((Intrinsic) localStack.elementAt(pos));
    } // localFromTopOfStack

    public Intrinsic globalFromTopOfStack(int offset)
    {
        return((Intrinsic) globalStack.elementAt(offset));
    } // globalFromTopOfStack

    protected void clearLocalStack()
    {
        this.localStack.clear();
        this.stackFrames.clear();
        this.currentStackFrame = 0;
    } // clearLocalStack

    protected void pushStackFrame()
    {
            // !!! this seems like it's inefficient.

        Integer num = new Integer(currentStackFrame);
        this.currentStackFrame = this.localStack.size();
        this.stackFrames.push(num);
        notifyNewStackFrame(this.currentStackFrame);
    } // pushStackFrame

    protected void popStackFrame()
    {
            // !!! this seems like it's inefficient.
        Integer num = (Integer) stackFrames.pop();
        this.currentStackFrame = (num == null) ? 0 : num.intValue();
        notifyNewStackFrame(this.currentStackFrame);
    } // popStackFrame

    public void popMultipleLocals(int count)
    {
        localStack.setSize(localStack.size() - count);
    } // popMultipleLocals

    public void popLocal()
    {
        localStack.pop();
    } // popLocal

    public Intrinsic executeImpl() throws FlowException
    {
        Intrinsic retVal = null;

        notifyDefineGlobals(this.globalStack);
        notifyDefineLocalStack(this.localStack);
        clearLocalStack();

        notifyBeginInterpretation();

        this.execTotalTime = -1;
        this.execStartTime = System.currentTimeMillis();

        try
        {
                // execute code outside of functions. Usually, this is
                //  just to assign initial values to global variables.
                // Be more fancy at your own risk.
            for (int i = 0; i < children.length; i++)
            {
                LogicContext lc = (LogicContext) children[i];
                if ((lc instanceof FunctionLogicContext) == false)
                    lc.execute();
            } // for

                // Run the actual program...
            retVal = this.mainline.execute();
        } // try
        catch (HaltException e)
        {
            // ignore it.
        } // catch
        catch (ExecException e)
        {
            notifySourceError(e);
            throw(e);
        } // catch
        finally
        {
                // cleanup...
            this.execTotalTime = System.currentTimeMillis() - this.execStartTime;
            notifyEndInterpretation((retVal == null) ? true : false);
        } // finally

        return(retVal);
    } // execute

} // GlobalLogicContext

// end of GlobalLogicContext.java ...

