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

import java.util.Stack;

    /**
     *  Implement this interface, then call TurtleSpace.addVarWatcher(). You
     *   will then be notified via these methods when their have been changes
     *   to variables: creation, modification, or destruction.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public interface VarWatcher
{
    public void varDefineLocalStack(Stack _localStack);
    public void varDefineGlobals(Stack _globalStack);
    public void varNewStackFrame(int pos);
    public void varUpdated(VarReferenceIntrinsic var);
} // VarWatcher

// end of VarWatcher.java ...

