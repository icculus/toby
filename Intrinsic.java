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

/**
 * This is an abstract class that represents an intrinsic data type in
 *  TOBY during interpretation.
 *
 *    Written by Ryan C. Gordon.
 */

public abstract class Intrinsic extends Throwable
{
        // instance variables...
    private String identifier;
    protected String value;


    public Intrinsic(String identifier, String initValue)
                                   throws TobyParseException
    {
        this.identifier = identifier;
        setValue(initValue);
    } // Constructor


    public Intrinsic(String identifier)
    {
        this.identifier = identifier;
    } // Constructor


    public String getIdentifier()
    {
        return(identifier);
    } // getIdentifier


    public boolean equals(Object obj)
    {
        boolean retVal;

        if (obj instanceof Intrinsic)
            retVal = value.equals(((Intrinsic) obj).getValue());
        else
            retVal = super.equals(obj);

        return(retVal);
    } // equals


    public String toString()
    {
        return(getType() + "[" + identifier + " == " + getValue() + "]");
    } // toString


    public String getValue()
    {
        return(value);
    } // getValue

    public abstract void setValue(String newValue) throws TobyParseException;
    public abstract String getType();
} // Intrinsic

// end of Intrinsic.java ...

