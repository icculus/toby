/*
 * TOBY -- An abstract interpreter engine and system for learning.
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


/*
 * This is a program entry point for a standalone Toby interpreter.
 *  There is no user interface, it just takes a source file on the command
 *  line and fires up the interpreter and renderer.
 *
 *   Written by Ryan C. Gordon (icculus@linuxgames.com)
 */

#include <sys/time.h>
#include <unistd.h>
#include "turtlespace/TurtleSpace.h"
#include "turtlespace/TurtleSpaceRenderer.h"
#include "util/TobyThread.h"

int main(int argc, char **argv)
{
    bool no_timing = false;
    struct timeval start_time;
    struct timeval end_time;
    TurtleSpaceRenderer *renderer;

    renderer = __platformBuildStandaloneRenderer(TobyLanguage::NAME,
                                                  &argc, &argv);

    if ((renderer == NULL) || (renderer->resize(640, 480) == false))
    {
        printf("failed to create TurtleSpaceRenderer.\n");
        return(0);
    } // if

    TurtleSpace tspace(renderer);

    if (gettimeofday(&start_time, NULL) == -1)
    {
        perror("WARNING! gettimeofday() failed");
        no_timing = true;
    } // if

    try
    {
        tspace.grabTurtleSpace();
        tspace.removeAllTurtles();
        tspace.addTurtle();
        tspace.useTurtle(0);
        tspace.defaultAllTurtles();
        tspace.cleanup();
        tspace.enableFence();

#if 1

        tspace.setPenColor(0.0, 1.0, 0.0, 0.0);

        for (int j = 0; j < 360; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                tspace.advanceTurtle(165);
                tspace.rotateTurtle(90);
            } // for
            tspace.rotateTurtle(1);
        } // for

#else

        float r = 0.0;
        float g = 1.0;
        float b = 0.0;
        float a = 0.0;

        double xmax = tspace.getTurtleSpaceWidth();
        double ymax = tspace.getTurtleSpaceHeight();
    
        float xincr = (1.0 / (float) xmax) * 2.0;
        float yincr = (1.0 / (float) ymax) * 2.0;

        tspace.rotateTurtle(90);

        for (double y = 0.0; y < ymax; y += 1.0)
        {
            r = 0.0;
            for (double x = 0.0; x < xmax; x += 1.0)
            {
                tspace.setPenColor(r, b, g, a);
                tspace.setTurtleXY(x, y);
                tspace.advanceTurtle(1);
                r += xincr;
                if ((xincr > 0.0) && (x >= (xmax / 2)))
                    xincr = -xincr;
            } // for
            xincr = -xincr;

            g -= yincr;
            b += yincr;

            if ((yincr > 0.0) && (y >= (ymax / 2)))
                yincr = -yincr;
        } // for

#endif


        tspace.releaseTurtleSpace();
    } // try

    catch (TobyException *e)
    {
        printf("Exception!\n[%s]\n", e->getMessage());
        delete e;
    } // catch

    if (gettimeofday(&end_time, NULL) == -1)
    {
        perror("WARNING! gettimeofday() failed");
        no_timing = true;
    } // if

    if (no_timing == false)
    {
        unsigned long ms;
        ms = (unsigned long) (((end_time.tv_sec - start_time.tv_sec) * 1000) +
                              ((end_time.tv_usec - start_time.tv_usec) / 1000));
        printf("total time == (%ld) milliseconds.\n", ms);
    } // if

    while (__platformRendererDoEvents())
    {
        __platformThreadYieldCurrent();
    } // while

    delete renderer;
    return(0);
} // main

