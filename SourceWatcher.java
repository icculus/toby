/**
 *  Implement this interface, then call TurtleSpace.addSourceWatcher(). You
 *   will then be notified via these methods when the TOBY source code
 *   line number under scrutiny changes.
 *
 *     Copyright (c) Lighting and Sound Technologies, 1997.
 *      Written by Ryan C. Gordon.
 */

public interface SourceWatcher
{
    public void beginInterpretation();
    public void endInterpretation();
    public void sourceUpdated(int newSourceLine);
    public void sourceError(int errLine);
} // SourceWatcher

// end of SourceWatcher.java ...


