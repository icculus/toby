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
import last.toby.util.*;
import last.toby.exceptions.*;

    /**
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public abstract class LogicContext extends NodeTree implements Linkable
{
    public final static int DEFAULT_TRACE_DELAY = 200;
    public final static int SLEEP_DEPRIVED = 10000;

    private static int insomniaCount = 0;
    private static int currentExecLine = -1;
    private static boolean terminateFlag = false;
    private static boolean pauseFlag = false;
    private static boolean stepFlag = false;
    private static boolean traceFlag = false;
    private static long traceTime = 0;
    private static SourceWatcher[] sourceWatchers = null;
    private static VarWatcher[] varWatchers = null;

    private int sourceLine = -1;
    protected Vector symTable = null;   // !!! need a freeResourcesImpl!


    public LogicContext(int sourceLine)
    {
        this.sourceLine = sourceLine;
    } // Constructor


    public static void addSourceWatcher(SourceWatcher sw)
    {
        if (sourceWatchers == null)
        {
            sourceWatchers = new SourceWatcher[1];
            sourceWatchers[0] = sw;
        } // if
        else
        {
            for (int i = 0; i < sourceWatchers.length; i++)
            {
                if (sourceWatchers[i] == sw)
                    return;
            } // for

            SourceWatcher[] a = new SourceWatcher[sourceWatchers.length + 1];
            System.arraycopy(sourceWatchers, 0, a, 0, sourceWatchers.length);
            a[sourceWatchers.length] = sw;
            sourceWatchers = a;
        } // else
    } // addSourceWatcher


    public synchronized static void addVarWatcher(VarWatcher vw)
    {
        if (varWatchers == null)
        {
            varWatchers = new VarWatcher[1];
            varWatchers[0] = vw;
        } // if
        else
        {
            for (int i = 0; i < varWatchers.length; i++)
            {
                if (varWatchers[i] == vw)
                    return;
            } // for

            VarWatcher[] a = new VarWatcher[varWatchers.length + 1];
            System.arraycopy(varWatchers, 0, a, 0, varWatchers.length);
            a[varWatchers.length] = vw;
            varWatchers = a;
        } // else
    } // addVarWatcher


        // !!! a finer-grained lock would be nice.
    public synchronized static void removeVarWatcher(VarWatcher vw)
    {
        if (varWatchers == null)
            return;

        for (int i = 0; i < varWatchers.length; i++)
        {
            if (varWatchers[i] == vw)
            {
                for (int j = i; j < varWatchers.length - 1; j++)
                    varWatchers[j] = varWatchers[j + 1];

                VarWatcher[] a = new VarWatcher[varWatchers.length - 1];
                System.arraycopy(varWatchers, 0, a, 0, a.length);
                varWatchers = a;
            } // if
        } // for
    } // removeVarWatcher


    protected static void notifyBeginInterpretation()
    {
        if (sourceWatchers == null)
            return;

        int max = sourceWatchers.length;
        for (int i = 0; i < max; i++)
            sourceWatchers[i].beginInterpretation();
    } // notifyBeginInterpretation


    protected static void notifyEndInterpretation(boolean normalTermination)
    {
        if (sourceWatchers == null)
            return;

        int max = sourceWatchers.length;
        for (int i = 0; i < max; i++)
            sourceWatchers[i].endInterpretation(normalTermination);
    } // notifyEndInterpretation

    protected static void notifySourceNewLine(int newLineNum)
    {
        if (sourceWatchers == null)
            return;

        int max = sourceWatchers.length;
        for (int i = 0; i < max; i++)
            sourceWatchers[i].sourceNewLine(newLineNum);
    } // notifySourceNewLine

    protected static void notifySourceError(ExecException e)
    {
        if (sourceWatchers == null)
            return;

        int max = sourceWatchers.length;
        for (int i = 0; i < max; i++)
            sourceWatchers[i].sourceError(e);
    } // notifySourceError



    protected synchronized static void notifyNewStackFrame(int framePtr)
    {
        if (varWatchers == null)
            return;

        int max = varWatchers.length;
        for (int i = 0; i < max; i++)
            varWatchers[i].varNewStackFrame(framePtr);
    } // notifyNewStackFrame
    
    protected synchronized static void notifyDefineGlobals(Stack newGlobals)
    {
        if (varWatchers == null)
            return;

        int max = varWatchers.length;
        for (int i = 0; i < max; i++)
            varWatchers[i].varDefineGlobals(newGlobals);
    } // notifyDefineGlobals
    
    protected synchronized static void notifyDefineLocalStack(Stack newLocals)
    {
        if (varWatchers == null)
            return;

        int max = varWatchers.length;
        for (int i = 0; i < max; i++)
            varWatchers[i].varDefineLocalStack(newLocals);
    } // notifyDefineLocalStack



        /**
         *  Same as Thread.sleep(), but keep trying even if there's
         *   an interrupted exception thrown...
         *
         *      @param millis Number of milliseconds to sleep.
         */
    protected void nap(long millis)
    {
//        _D("nap", "napping (" + millis + ") milliseconds...");

        long endTime = System.currentTimeMillis() + millis;

        do
        {
            try
            {
                Thread.sleep(millis);
            } // try
            catch (InterruptedException ie)
            {
                millis = endTime - System.currentTimeMillis();
            } // catch
        } while (System.currentTimeMillis() < endTime);

        this.insomniaCount = 0;  // reset it 'cause we're sleeping.
    } // nap


    private static void checkForTerminateRequest() throws HaltException
    {
        if (terminateFlag == true)
        {
            terminateFlag = false;
            HaltException._throw("Halted at user's request");  // !!! toby language.
        } // if
    } // checkForTerminateRequest

        /**
            !!! more.
         * Blocks until delay time is over. In tracing mode, this is when
         *  the specified delay time elapses. In stepping mode, this is when
         *  the user instructs the interpreter to continue.
         *
         * If no tracing/stepping is enabled, this method returns almost
         *  immediately. Every (SLEEP_DEPRIVED) calls to this function
         *  when not stepping or tracing causes the calling thread to sleep()
         *  for ~10 milliseconds. This allows other events to run (such as
         *  input to the menubar), but does not significantly slow down
         *  interpretation. Even though the interpretation thread should not
         *  lag the queue, without this, it'll starve the processor, and still
         *  prevent other threads from running. This is the nature of an
         *  program that does little more than push data between arrays. :)
         */
    protected final void lineUpkeep() throws FlowException
    {
        if ((this.sourceLine <= 0) || (this.currentExecLine == this.sourceLine))
            return;

//        _D("lineUpkeep", "Now on line (" + this.sourceLine + ").");

        this.currentExecLine = this.sourceLine;
        notifySourceNewLine(this.sourceLine);

        checkForTerminateRequest();

        while (this.pauseFlag == true)
        {
            nap(10);

            checkForTerminateRequest();

            if (this.stepFlag == true)
            {
                this.stepFlag = false;
                return;
            } // if
        } // while

        if (this.traceFlag == true)
            nap(this.traceTime);
        else
        {
            this.insomniaCount++;
            if (this.insomniaCount >= this.SLEEP_DEPRIVED)
                nap(10);
        } // else
    } // lineUpkeep


    public int getSourceLine()
    {
        return(sourceLine);
    } // getSourceLine


    public LogicContext checkScopesForSymbol(String symbol)
    {
        LogicContext retVal = this;

        for (retVal = this;
             retVal != null;
             retVal = (LogicContext) retVal.getParent())
        {
            if (retVal.getSymbolIndex(symbol) != -1)
                return(retVal);
        } // for

        return(retVal);
    } // checkScopesForSymbol


    public Symbol getSymbolByName(String name)
    {
        Symbol retVal = null;
        int index = getSymbolIndex(name);
        if (index != -1)
            retVal = (Symbol) symTable.elementAt(index);
        return(retVal);
    } // getSymbolByName


    public int getSymbolIndex(String sym)
    {
        if (symTable == null)
            return(-1);

        int retVal = 0;

        for (int i = symTable.size() - 1; i >= 0; i--)
        {
            if ( ((Symbol) symTable.elementAt(i)).getName().equals(sym) )
            {
                NodeTree nt = getParent();
                if (nt != null)
                {
                    while ( ((nt instanceof FunctionLogicContext) == false) &&
                            ((nt instanceof GlobalLogicContext) == false) )
                        retVal += getSymbolCount();
                } // if

                return(retVal + i);
            } // if
        } // for

        return(-1);
    } // getSymbolIndex


    public int getSymbolCount()
    {
        int retVal = 0;
        if (symTable != null)
            retVal = symTable.size();

        return(retVal);
    } // getSymbolCount


    protected void addScopedSymbol(Object type, String identifier)
    {
        int pos = 0;
        if (symTable != null)
            pos = symTable.size();
        addScopedSymbol(type, identifier, pos);
    } // addScopedSymbol

    protected void addScopedSymbol(Object type, String identifier, int pos)
    {
        if (symTable == null)
            symTable = new Vector();

//        _D("addScopedSymbol", "[" + type.toString() + "], [" +
//                               identifier + "], (" + pos + ").");

            // !!! this should probably check the current symtable, if not the tree...
        symTable.insertElementAt(new Symbol(type, identifier), pos);
    } // addScopedSymbol


    public final void link(GlobalLogicContext glob) throws ParseException
    {
        lock();

        try
        {
            int max = children.length;
            for (int i = 0; i < max; i++)
            {
                if (children[i] instanceof Linkable)
                    ((Linkable) children[i]).link(glob);
            } // for

//            _D("link", "starting linkImpl for " + getClass().getName() + "...");
            linkImpl(glob);
        } // try
        catch (ParseException pe)
        {
            if (pe.exceptionLine == ParseException.LINENUM_UNDEFINED)
                pe.exceptionLine = this.sourceLine;

                // !!! I'm certain this sucks.
            if ((pe.procName == null) &&
                (this instanceof FunctionLogicContext))
            {
                pe.procName = ((FunctionLogicContext) this).getFuncName();
            } // if
            throw(pe);
        } // catch
    } // link


    public static void linkVectorElements(Vector v, GlobalLogicContext glob)
                                           throws ParseException
    {
        int max = v.size();
        for (int i = 0; i < max; i++)
        {
            Object obj = v.elementAt(i);
            if (obj instanceof Linkable)
                ((Linkable) obj).link(glob);
        } // for
    } // linkVectorElements


    protected Intrinsic defaultExecuteImpl() throws FlowException
    {
        int max = children.length;
        for (int i = 0; i < max; i++)
            ((LogicContext) children[i]).execute();

        return(NothingIntrinsic.nothing);
    } // defaultExecuteImpl


    public GlobalLogicContext getGlobalContext()
    {
            // !!! cache this?
        NodeTree retVal = this;

        while (true)
        {
            NodeTree myParent = retVal.getParent();
            if (myParent == null)
                return((GlobalLogicContext) retVal);
            retVal = myParent;
        } // while
    } // getGlobalContext


    public final Intrinsic execute() throws FlowException
    {
//        _D("execute", "starting executeImpl() for " +
//            getClass().getName() + "...");

        lineUpkeep();

        try
        {
            return(executeImpl());
        } // try
        catch (FlowException fe)
        {
            if (fe.exceptionLine == FlowException.LINENUM_UNDEFINED)
                fe.exceptionLine = this.sourceLine;
            throw(fe);  // pass it on.
        } // catch
    } // execute

    public static void resetState()
    {

        insomniaCount = 0;
        terminateFlag = false;
        currentExecLine = -1;
        pauseFlag = false;
        stepFlag = false;
        traceFlag = false;
        traceTime = 0;

        if (sourceWatchers != null)
        {
            int max = sourceWatchers.length;
            for (int i = 0; i < max; i++)
                sourceWatchers[i] = null;
            sourceWatchers = null;
        } // if
    } // resetState

    public static void endExecution()
    {
        terminateFlag = true;
    } // endExecution

    public static void pauseExecution()
    {
        pauseFlag = true;
    } // pauseExecution

    public static void resumeExecution()
    {
        pauseFlag = false;
    } // endExecution

    public static void triggerStep()
    {
        stepFlag = true;
    } // triggerStep

    public static void enableTracing(long delay)
    {
        traceFlag = true;
        traceTime = delay;
    } // enableTracing (overload; takes a specific delay)

    public static void setTracing(boolean onOff)
    {
        if (onOff)
            enableTracing();
        else
            disableTracing();
    } // setTracing

    public static void enableTracing()
    {
        enableTracing(DEFAULT_TRACE_DELAY);
    } // enableTracing (overload; default delay);

    public static void disableTracing()
    {
        traceFlag = false;
    } // disableTracing


    protected abstract Intrinsic executeImpl() throws FlowException;
    protected abstract void linkImpl(GlobalLogicContext glob) throws ParseException;
//    public abstract String toXML();
} // LogicContext

// end of LogicContext.java ...

