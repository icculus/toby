/**
 *  TurtleSpace class; where all the good little turtles play...
 *
 *  This is an abstract class that handles the graphical display of
 *   TurtleSpace. An language interpreting class (i.e. - TobyInterpreter)
 *   should extend this and implement the abstracts at the bottom of the
 *   class. This is in case you'd like to "plug in" a LOGO interpreter, or
 *   any other language you can dream up.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.io.Serializable;

public abstract class TurtleSpace extends Canvas implements Serializable
{
    public final static int DEFAULT_TRACE_DELAY = 200;
    public final static int STEP_TRACE_DELAY = -1;
    public final static double TURTLE_RATIO = 0.02;  // 2 percent of screen.

    protected Turtle turtle;                   // our little virtual pet.
    private Vector varWatchers;
    private Vector sourceWatchers;

    private boolean nextStep = false;
    private boolean traceEnabled = false;
    private int traceDelay = DEFAULT_TRACE_DELAY;

    public TurtleSpace()
    {
        double tSize = getToolkit().getScreenSize().width;

        sourceWatchers = new Vector();
        varWatchers = new Vector();

        setForeground(Color.white);
        setBackground(Color.black);

        turtle = new TriangleTurtle();   // change for a new Turtle class.

        tSize *= TURTLE_RATIO;
        turtle.setTurtleSize(tSize);
    } // Constructor


    public void finalize() throws Throwable
    {
        varWatchers.removeAllElements();
        varWatchers.trimToSize();
        varWatchers = null;
        sourceWatchers.removeAllElements();
        sourceWatchers.trimToSize();
        sourceWatchers = null;
        turtle = null;
        super.finalize();
    } // finalize


    public final void addVarWatcher(VarWatcher vw)
    {
        varWatchers.addElement(vw);
    } // addVarWatcher


    public final void removeVarWatcher(VarWatcher vw)
    {
        varWatchers.removeElement(vw);
    } // removeVarWatcher


    public final void addSourceWatcher(SourceWatcher sw)
    {
        sourceWatchers.addElement(sw);
    } // addSourceWatcher


    public final void removeSourceWatcher(SourceWatcher sw)
    {
        sourceWatchers.removeElement(sw);
    } // removeSourceWatcher


    protected final void notifyVarCreated(Intrinsic var)
    {
        int i;

        for (i = 0; i < varWatchers.size(); i++)
            ((VarWatcher) varWatchers.elementAt(i)).varCreated(var);
    } // notifyVarCreate


    protected final void notifyVarDestroyed(Intrinsic var)
    {
        int i;

        for (i = 0; i < varWatchers.size(); i++)
            ((VarWatcher) varWatchers.elementAt(i)).varDestroyed(var);
    } // notifyVarCreate


    protected final void notifyVarUpdated(Intrinsic var)
    {
        int i;

        for (i = 0; i < varWatchers.size(); i++)
            ((VarWatcher) varWatchers.elementAt(i)).varUpdated(var);
    } // updateVarWatchers


    protected final void notifySourceLineUpdated(int line)
    {
        int i;

        for (i = 0; i < sourceWatchers.size(); i++)
            ((SourceWatcher) sourceWatchers.elementAt(i)).sourceUpdated(line);
    } // updateSourceWatchers


    protected final void notifyBeginInterpretation()
    {
        int i;

        for (i = 0; i < sourceWatchers.size(); i++)
            ((SourceWatcher)sourceWatchers.elementAt(i)).beginInterpretation();
    } // notifyBeginInterpretation


    protected final void notifyEndInterpretation()
    {
        int i;

        for (i = 0; i < sourceWatchers.size(); i++)
            ((SourceWatcher) sourceWatchers.elementAt(i)).endInterpretation();

        enableTracing(false);
    } // notifyEndInterpretation


    protected final void notifySourceError(int errLine)
    {
        int i;

        for (i = 0; i < sourceWatchers.size(); i++)
            ((SourceWatcher) sourceWatchers.elementAt(i)).sourceError(errLine);
    } // notifySourceError


    public final void enableTracing(boolean shouldEnable)
    /**
     *  !!! comment.
     */
    {
        traceEnabled = shouldEnable;
        traceDelay = DEFAULT_TRACE_DELAY;
    } // enableTracing (overloaded; default delay time.)


    public final void enableTracing(boolean shouldEnable, int delayTime)
    /**
     *  !!! comment.
     */
    {
        traceEnabled = shouldEnable;
        traceDelay = delayTime;
    } // enableTracing (overloaded; accepts delay time...)


    public final boolean isTraceEnabled()
    {
        return(traceEnabled);
    } // isTraceEnabled


    public final void setCodeWithStep(String newCode) throws TobyParseException
    {
        enableTracing(true, STEP_TRACE_DELAY);
        nextStep = false;
        setCode(newCode);
    } // setCodeWithStep


    public final void stepInterpreter()
    {
        if ((traceEnabled == false) || (traceDelay != STEP_TRACE_DELAY))
            enableTracing(true, STEP_TRACE_DELAY);
        nextStep = true;
    } // stepInterpreter


    public final void delayInterpretation()
    {
        if (traceEnabled)
        {
            if (traceDelay == STEP_TRACE_DELAY)
            {
                while ((!nextStep) && (!haltRequested()))
                    Thread.yield();
                nextStep = false;
            } // if
            else
            {
                try
                {
                    Thread.sleep(traceDelay);
                } // try
                catch (InterruptedException intrExcp)
                {
                    // don't care...
                } // catch
            } // else
        } // if
    } // delayInterpretation


    public abstract void setCode(String newCode) throws TobyParseException;
    public abstract boolean isCodeRunning();
    public abstract void haltInterpreter();
    public abstract void cleanup();
    public abstract boolean haltRequested();

} // TurtleSpace

// end of TurtleSpace.java ...


