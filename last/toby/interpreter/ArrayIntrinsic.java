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
import last.toby.util.*;
import last.toby.exceptions.*;

    /**
     * !!! comment me!
     *
     *   @author Ryan C. Gordon. (icculus@icculus.org)
     */
public class ArrayIntrinsic extends Intrinsic
{
    protected Intrinsic[] elements = null;
    protected int index_offset = -1;


    // !!! being able to turn off range checking might be a nice, dangerous
    // !!!  speed hack.


    /*
     * Note that ArrayIntrinsics are constructed without any element storage.
     *  Before you can store Intrinsics in the array, you need to
     *  call redim() to define some space. This should probably be
     *  done as the first instructions in a FunctionLogicContext (or
     *  GlobalLogicContext, for globally-scoped arrays), in the same
     *  way that prepended instructions are used to set the initial values
     *  of simple Intrinsic types like NumberIntrinsic.
     */


        /*
         * (Re)size the array.
         *
         *   @param start_index index of initial array element. A pascal array
         *                      with a range of [5 .. 10] would specify 5 here.
         *   @param   end_index index of final array element. A pascal array
         *                      with a range of [5 .. 10] would specify 10.
         *   @param dummy_element Intrinsic to fill array with. The filling
         *                        is done with dummy_element.getCopy().
         *                        If this is too time/memory draining for you,
         *                        You can specify null, in which case the
         *                        array will have Java null references, which
         *                        will cause a crash unrelated to your
         *                        interpreted language if dereferenced. For
         *                        interpreting something like C, however,
         *                        this might be preferable behaviour on all
         *                        counts.
         */
    public void redim(int start_index, int end_index, Intrinsic dummy_element)
    {
        int new_size = end_index - start_index + 1;
        this.index_offset = start_index;
        Intrinsic[] new_array = new Intrinsic[new_size];
        int empty_space = new_size;
        int empty_start_index = 0;

        if (elements == null)
            elements = new_array;
        else
        {
            for (int i = 0; i < elements.length; i++)
            {
                if (i < new_size)
                    new_array[i] = elements[i];
                elements[i] = null;
            } // for

            empty_space = new_size - elements.length;
            empty_start_index = elements.length;
            elements = new_array;
        } // else

        if (empty_space > 0)
        {
            for (int i = empty_start_index; i < new_size; i++)
            {
                if (dummy_element == null)
                    elements[i] = null;
                else
                    elements[i] = dummy_element.getCopy();
            } // for
        } // if
    } // redim

    public Intrinsic getElement(int index) throws ExecException
    {
        index -= index_offset;

        if ((index < 0) || (index >= elements.length))
            ExecException._throw(TobyLanguage.OUT_OF_RANGE);

        return(elements[index]);
    } // getElement


        // chances are this isn't needed; usually you'll getElement() an
        //  Intrinsic, and then set values on that directly. This is for
        //  dropping a new Intrinsic item into an array slot.
    public void setElement(int index, Intrinsic new_value) throws ExecException
    {
        index -= index_offset;

        if ((index < 0) || (index >= elements.length))
            ExecException._throw(TobyLanguage.OUT_OF_RANGE);

        elements[index] = new_value;
    } // setElement


    public boolean isConstantValue()
    {
        return(false);
    } // isConstantValue


    public String toString()
    {
        return(name + "array://" + getIdentifier() + ":" + index_offset + "-" +
                (elements.length - index_offset) + ")");
    } // toString

    protected Intrinsic getCopyImpl()
    {
        ArrayIntrinsic retval = new ArrayIntrinsic();

        if (elements != null)
        {
            retval.elements = new Intrinsic[elements.length];
            for (int i = 0; i < elements.length; i++)
            {
                if (elements[i] == null)
                    retval.elements[i] = null;
                else
                    retval.elements[i] = elements[i].getCopy();
            } // for
        } // if

        retval.index_offset = index_offset;
        return(retval);
    } // getCopyImpl

    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        if (elements != null)
        {
            for (int i = 0; i < elements.length; i++)
            {
                if ((elements[i] != null) && (elements[i] instanceof Linkable))
                    ((Linkable) elements[i]).link(glob);
            } // for
        } // if
    } // linkImpl
} // ArrayIntrinsic

// end of ArrayIntrinsic.java ...

