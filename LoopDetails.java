/**
 * This is a abstract class to represent loops and other structured
 *  code blocks during TOBY interpretation.
 *
 *    Copyright (c) 1997-1999 Lighting and Sound Technologies.
 *     Written by Ryan C. Gordon.
 */


public abstract class LoopDetails
{
    public int lineNum;
    public String[] condition;

    public LoopDetails(int lineNum)
    {
        this.lineNum = lineNum;
    } // Constructor

} // LoopDetails

// end of LoopDetails.java ...


