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

package last.toby.util;

import java.util.Vector;

    /**
     * A node tree is a hierarchy, starting at a single root node.
     *  Each node may have a variable number of children nodes, and
     *  a single parent node, except the root node, which has a
     *  parent node of (null).
     *
     * For speed's sake, we are very trusting with the NodeTree's data.
     *  All children are kept in a protected Vector until the lock()
     *  function is called, at which time this node is considered "locked"
     *  and all children are moved to an exactly-sized NodeTree[]. No
     *  more elements may be added to the array via addChild(), as it
     *  will intentionally generate an exception. However, the accesses
     *  no longer have the overhead of a method call followed by a Vector
     *  lookup. Just hit the array directly. If you must add more children,
     *  call the unlock() method, where the current data in the array is
     *  dumped back to a new Vector, and may be manipulated again.
     *
     * The parent node is public, too. It may be set, as a convenience, via
     *  the constructor. The void constructor just assumes a root node, and
     *  leaves the parent field null.
     *
     *  @author Ryan C. Gordon (icculus@lokigames.com)
     */
public abstract class NodeTree
{
    protected NodeTree[] children = null;
    protected NodeTree parent = null;
    protected Vector kidsVect = null;
    private boolean resourcesFreed = false;
    private boolean locked = false;

    public NodeTree(NodeTree parent)
    {
        this.parent = parent;
        kidsVect = new Vector();
    } // Constructor (overloaded; takes parent node.)


    public NodeTree()
    {
        this(null);
    } // Constructor (overloaded; takes void args.)


    public void finalize() throws Throwable
    {
        freeResources(true);
    } // finalize

        /**
         * If (debugging) is (true), then println debug info to stdout.
         *
         *   @param funcName name of method where debugging info lies.
         *   @param msg Message to print.
         */
    public static void _D(String funcName, String msg)
    {
        BugSquasher.println(funcName, msg);
    } // _D

    public void setParent(NodeTree newParent)
    {
        this.parent = newParent;
    } // setParent


    public NodeTree getParent()
    {
        return(this.parent);
    } // getParent


    public synchronized int getChildCount()
    {
        if (children != null)
            return(children.length);

        if (kidsVect != null)
            return(kidsVect.size());

        return(-1);  // (*shrug*)
    } // getChildCount


    public void verifyUnlocked()
    {
        if (resourcesFreed == true)
            throw(new IllegalArgumentException(TobyLanguage.USING_FREE_NODE));

        if (isLocked() == true)
            throw(new IllegalArgumentException(TobyLanguage.NODETREE_LOCKED));
    } // verifyUnlocked


    public void verifyLocked()
    {
        if (resourcesFreed == true)
            throw(new IllegalArgumentException(TobyLanguage.USING_FREE_NODE));

        if (isLocked() == false)
            throw(new IllegalArgumentException(TobyLanguage.NODETREE_ULOCKED));
    } // verifyLocked


    public final synchronized NodeTree addChild(NodeTree child)
    {
        verifyUnlocked();

        if (verifyNewChildType(child) == false)
            throw(new IllegalArgumentException(TobyLanguage.BAD_CHILD_NODE));
        else
        {
            child.parent = this;
            kidsVect.addElement(child);
        } // else

        return(child);
    } // addChild

    public final synchronized boolean isLocked()
    {
        return(locked);
    } // isLocked

    public final synchronized void lock()
    {
        verifyUnlocked();

        int max = kidsVect.size();
        children = new NodeTree[max];
        kidsVect.copyInto(children);
        kidsVect.removeAllElements();
        kidsVect.trimToSize();
        kidsVect = null;
        locked = true;
    } // lock

    public final synchronized void unlock()
    {
        verifyLocked();

        int max = children.length;
        kidsVect = new Vector(max);

        for (int i = 0; i < max; i++)
        {
            kidsVect.addElement(children[i]);
            children[i] = null;
        } // for

        children = null;
        locked = false;
    } // unlock

    public final void freeResources(boolean recurse)
    {
        if (resourcesFreed == false)
        {
            freeResourcesImpl();

            if (isLocked() == false)
                lock();

            for (int i = 0; i < children.length; i++)
            {
                if (recurse)
                    children[i].freeResources(true);
                children[i] = null;
            } // for

            children = null;
            resourcesFreed = true;
        } // if
    } // freeResources


    public boolean verifyNewChildType(NodeTree child)
    {
        return(true);   // override to check types.
    } // verifyNewChildType


    protected abstract void freeResourcesImpl();

} // NodeTree

// end of NodeTree.java ...

