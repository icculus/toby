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

package last.toby.turtlespace;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;

    /**
     *  TurtleSpace class; where all the good little turtles play...
     *
     *  This is an abstract class that handles the graphical display of
     *   TurtleSpace. An language interpreting class (i.e. - TobyInterpreter)
     *   should extend this and implement the abstracts at the bottom of the
     *   class. This is in case you'd like to "plug in" a LOGO interpreter, or
     *   any other language you can dream up.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class TurtleSpace extends JComponent
{
    public final static int DEFAULT_TRACE_DELAY = 200;
    public final static int STEP_TRACE_DELAY = -1;
    public final static double TURTLE_RATIO = 0.02;  // 2 percent of screen.
    public final static int SLEEP_DEPRIVED = 100;

    protected Turtle turtle;
//    private Vector varWatchers;
//    private Vector sourceWatchers;

    private boolean nextStep = false;
    private boolean traceEnabled = false;
    private int traceDelay = DEFAULT_TRACE_DELAY;
    private boolean codeRunning = false;
    private int insomniaCount = 0;

        /**
         * Construct a TurtleSpace component.
         */
    public TurtleSpace()
    {
        Graphics g;
        double tSize;

//        sourceWatchers = new Vector();
//        varWatchers = new Vector();

        setForeground(Color.white);
        setBackground(Color.black);

        turtle = new TriangleTurtle();   // change for a new Turtle class.
        tSize = ((double) getToolkit().getScreenSize().width) * TURTLE_RATIO;

        turtle.setTurtleSize(tSize);
    } // Constructor


        /**
         *  Cleanup after ourselves; empties out all the Vectors and objects
         *   that TurtleSpaces collects up.
         */
    public void finalize() throws Throwable
    {
//        varWatchers.removeAllElements();
//        varWatchers.trimToSize();
//        varWatchers = null;
//        sourceWatchers.removeAllElements();
//        sourceWatchers.trimToSize();
//        sourceWatchers = null;
        turtle = null;
        super.finalize();
    } // finalize


        /**
         * Query whether code is being interpreted or not.
         *
         *      @return (true) if interpretation is executing, (false) if not.
         */
    public boolean isCodeRunning()
    {
        return(codeRunning);
    } // isCodeRunning


        /**
         *  Add a VarWatcher to this TurtleSpace, so it will be notified
         *   when a variable is created or changed...
         *
         *    @param vw The VarWatcher to add.
         */
//    public final void addVarWatcher(VarWatcher vw)
//    {
//        varWatchers.addElement(vw);
//    } // addVarWatcher


        /**
         *  Remove a VarWatcher from this TurtleSpace
         *
         *    @param vw The VarWatcher to remove.
         */
//    public final void removeVarWatcher(VarWatcher vw)
//    {
//        varWatchers.removeElement(vw);
//    } // removeVarWatcher


        /**
         *  Add a SourceWatcher to this TurtleSpace, so it will be notified
         *   when a new line of code is being executed.
         *
         *    @param sw The SourceWatcher to add.
         */
//    public final void addSourceWatcher(SourceWatcher sw)
//    {
//        sourceWatchers.addElement(sw);
//    } // addSourceWatcher


        /**
         *  Remove a SourceWatcher from this TurtleSpace
         *
         *    @param sw The SourceWatcher to remove.
         */
//    public final void removeSourceWatcher(SourceWatcher sw)
//    {
//        sourceWatchers.removeElement(sw);
//    } // removeSourceWatcher


        /**
         *  Alert all VarWatchers that a new variable has been created.
         *
         *    @param var A reference to the Intrinsic that has been created.
         */
//    protected final void notifyVarCreated(Intrinsic var)
//    {
// /       int i;
//        int max = varWatchers.size();
//
//        for (i = 0; i < max; i++)
//            ((VarWatcher) varWatchers.elementAt(i)).varCreated(var);
//    } // notifyVarCreate


        /**
         *  Alert all VarWatchers that a variable has been destroyed.
         *
         *    @param var A reference to the Intrinsic that has been destroyed.
         *               Note that this does not been the Intrinsic object has
         *               been finalized or collected, but rather the Intrinsic
         *               has left the scope of interpretation.
         */
//    protected final void notifyVarDestroyed(Intrinsic var)
//    {
//        int i;
//        int max = varWatchers.size();
//
//        for (i = 0; i < max; i++)
//            ((VarWatcher) varWatchers.elementAt(i)).varDestroyed(var);
//    } // notifyVarCreate


        /**
         *  Alert all VarWatchers that a variable has a new value.
         *
         *    @param var A reference to the Intrinsic that has been modified.
         */
//    protected final void notifyVarUpdated(Intrinsic var)
//    {
//        int i;
//        int max = varWatchers.size();
//
//        for (i = 0; i < max; i++)
//            ((VarWatcher) varWatchers.elementAt(i)).varUpdated(var);
//   } // updateVarWatchers


        /**
         *  Alert all SourceWatchers that a new line of code has been reached.
         *
         *    @param line the line of code that we are now on.
         */
//    protected final void notifySourceLineUpdated(int line)
//    {
//        int i;
//        int max = sourceWatchers.size();
//
//        for (i = 0; i < max; i++)
//            ((SourceWatcher) sourceWatchers.elementAt(i)).sourceUpdated(line);
//    } // updateSourceWatchers
//

        /**
         *  Alert all SourceWatchers and VarWatchers that interpretation
         *   of the program has begun (or restarted).
         *
         *    @param var A reference to the Intrinsic that has been created.
         */
/*
    protected final void notifyBeginInterpretation()
    {
        int i;
        int maxSource = sourceWatchers.size();
        int maxVar = varWatchers.size();

        codeRunning = true;

        for (i = 0; i < maxSource; i++)
            ((SourceWatcher)sourceWatchers.elementAt(i)).beginInterpretation();

        for (i = 0; i < maxVar; i++)
            ((VarWatcher) varWatchers.elementAt(i)).varBeginInterpretation();
    } // notifyBeginInterpretation
*/

        /**
         *  Alert all SourceWatchers and VarWatchers that
         *   interpretation of the program has ended.
         *
         *    @param normalTermination True if normal program termination,
         *                             false if program ended due to user
         *                             request or error.
         */
/*
    protected final void notifyEndInterpretation(boolean normalTermination)
    {
        int i;
        int maxSource = sourceWatchers.size();
        int maxVar = varWatchers.size();

        codeRunning = false;

        for (i = 0; i < maxSource; i++)
        {
            ((SourceWatcher) sourceWatchers.elementAt(i)).
                endInterpretation(normalTermination);
        } // for

        for (i = 0; i < maxVar; i++)
        {
            ((VarWatcher) varWatchers.elementAt(i)).
                varEndInterpretation(normalTermination);
        } // for

        enableTracing(false);
    } // notifyEndInterpretation

*/
        /**
         *  Alert all SourceWatchers that there was an error on
         *   line (errLine) of the program.
         *
         *    @param errLine line where error occured.
         */
/*
    protected final void notifySourceError(int errLine)
    {
        int i;
        int max = sourceWatchers.size();

        for (i = 0; i < max; i++)
            ((SourceWatcher) sourceWatchers.elementAt(i)).sourceError(errLine);
    } // notifySourceError
*/
        /**
         *  Set tracing of the program that runs in this TurtleSpace.
         *   The default delay between interpretation of lines is used.
         *
         *     @param shouldEnable (true) to enable tracing,
         *            (false) otherwise.
         */
/*
    public final void enableTracing(boolean shouldEnable)
    {
        traceEnabled = shouldEnable;
        traceDelay = DEFAULT_TRACE_DELAY;
    } // enableTracing (overloaded; default delay time.)
*/


        /**
         *  Set tracing of the program that runs in this TurtleSpace.
         *   The delay between interpretation of lines can be specified.
         *
         *     @param shouldEnable (true) to enable tracing, (false) otherwise.
         *     @param delayTime time, in milliseconds, to delay between lines.
         */
/*
    public final void enableTracing(boolean shouldEnable, int delayTime)
    {
        traceEnabled = shouldEnable;
        traceDelay = delayTime;
    } // enableTracing (overloaded; accepts delay time...)
*/

        /**
         * Determine if tracing has been enabled for this TurtleSpace.
         *
         *    @return (true) if enabled, (false) otherwise.
         */
//    public final boolean isTraceEnabled()
//    {
//        return(traceEnabled);
//    } // isTraceEnabled


        /**
         *  Load a new program into this TurtleSpace, and turn on
         *   program stepping (no lines of code are interpreted until
         *   we are explicitly instructed.)
         *
         *     @param shouldEnable (true) to enable tracing,
         *                         (false) otherwise.
         */
//    public final void setCodeWithStep(String newCode)
//    {
//        enableTracing(true, STEP_TRACE_DELAY);
//        nextStep = false;
//        setCode(newCode);
//    } // setCodeWithStep


        /**
         * Make the interpreter step to the next line of code. If
         *  stepping was not enabled prior to this call, it will
         *  be enabled by this method.
         */
//    public final void stepInterpreter()
//    {
//        if ((traceEnabled == false) || (traceDelay != STEP_TRACE_DELAY))
//            enableTracing(true, STEP_TRACE_DELAY);
//        nextStep = true;
//    } // stepInterpreter


        /**
         *  Same as Thread.sleep(), but you don't care if there's
         *   an interrupted exception thrown...
         *
         *      @param millis Number of milliseconds to sleep.
         */
    protected void nap(long millis)
    {
        try
        {
            Thread.sleep(millis);
        } // try
        catch (InterruptedException ie)
        {
            // don't care.
        } // catch
    } // nap


        /**
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
         *  lag the cue, without this, it'll starve the processor, and still
         *  prevent other threads from running. This is the nature of an
         *  program that does little more than push data between arrays. :)
         *
         * Subclasses of TurtleSpace should call this method between
         *  every line of code they interpret.
         */
/*
    protected final void delayInterpretation()
    {
        long endTime = System.currentTimeMillis();

        if (traceEnabled)
        {
            if (traceDelay == STEP_TRACE_DELAY)
            {
                while ((!nextStep) && (!haltRequested()))
                    nap(10);
                nextStep = false;
            } // if
            else
                nap(traceDelay);
        } // if
        else
        {
            insomniaCount++;
            if (insomniaCount >= SLEEP_DEPRIVED)
            {
                nap(10);
                insomniaCount = 0;
            } // if
        } // else
    } // delayInterpretation
*/

    /*
     * The following should be overriden by a subclass that
     *  interprets a program...paintComponent() should probably
     *  also be overridden.
     */


        /**
         * Set a new program, and start its execution.
         *
         *     @param newCode String of program. Can include whitespace.
         *     @throw TobyParseException on interpretation errors.
         */
//    public abstract void setCode(String newCode) throws TobyParseException;


        /**
         * Request the interpreter to halt interpretation.
         */
//    public abstract void haltInterpreter();


        /**
         * Query if an explicit termination of interpretation has been
         *  requested.
         *
         *   @return (true) if halt requested, (false) otherwise.
         */
//    public abstract boolean haltRequested();


        /*
         * Cleanup TurtleSpace. This causes any drawing to be cleaned out.
         */
//    public abstract void cleanup();

} // TurtleSpace

// end of TurtleSpace.java ...

