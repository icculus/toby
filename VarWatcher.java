/**
 *  Implement this interface, then call TurtleSpace.addVarWatcher(). You
 *   will then be notified via these methods when their have been changes
 *   to variables: creation, modification, or destruction.
 *
 */

public interface VarWatcher
{
    public void varBeginInterpretation();
    public void varCreated(Intrinsic intrinsic);
    public void varDestroyed(Intrinsic intrinsic);
    public void varInScope(Intrinsic intrinsic);
    public void varOutOfScope(Intrinsic intrinsic);
    public void varUpdated(Intrinsic intrinsic);
} // VarWatcher

// end of VarWatcher.java ...


