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
 * This class maintains details about an instance of an IF block
 *  during the interpretation of TOBY code...
 *
 *     Written by Ryan C. Gordon.
 */

public class IfLoopDetails extends LoopDetails
{
    public boolean isElse;

    public IfLoopDetails(int srcLine, boolean isElse)
    {
        super(srcLine);
        this.isElse = isElse;
    } // Constructor
} // IfLoopDetails

// end of IfLoopDetails.java ...


