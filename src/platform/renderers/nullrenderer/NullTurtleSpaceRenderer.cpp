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

#include "platform/renderers/nullrenderer/NullTurtleSpaceRenderer.h"


NullTurtleSpaceRenderer::NullTurtleSpaceRenderer(void)
    : w(0.0),
      h(0.0)
{
    _D(("NullTurtleSpaceRenderer::NullTurtleSpaceRenderer() called!\n"));
    fprintf(stderr, "WARNING: You are using NullTurtleSpaceRenderer!\n");
} // Constructor


NullTurtleSpaceRenderer::~NullTurtleSpaceRenderer(void)
{
    _D(("NullTurtleSpaceRenderer::~NullTurtleSpaceRenderer() called!\n"));
} // Destructor


bool NullTurtleSpaceRenderer::resize(int _w, int _h)
{
    _D(("NullTurtleSpaceRenderer::resize() called!\n"));
    w = (double) _w;
    h = (double) _h;
    return(true);
} // NullTurtleSpaceRenderer::resize


void NullTurtleSpaceRenderer::notifyGrabbed(void)
{
    _D(("NullTurtleSpaceRenderer::notifyGrabbed() called!\n"));
} // NullTurtleSpaceRenderer::notifyGrabbed


void NullTurtleSpaceRenderer::notifyUngrabbed(void)
{
    _D(("NullTurtleSpaceRenderer::notifyUngrabbed() called!\n"));
} // NullTurtleSpaceRenderer::notifyUngrabbed


double NullTurtleSpaceRenderer::getTurtleSpaceWidth(void)
{
    _D(("NullTurtleSpaceRenderer::getTurtleSpaceWidth() called!\n"));
    return(w);
} // NullTurtleSpaceRenderer::getTurtleSpaceWidth


double NullTurtleSpaceRenderer::getTurtleSpaceHeight(void)
{
    _D(("NullTurtleSpaceRenderer::getTurtleSpaceHeight() called!\n"));
    return(h);
} // NullTurtleSpaceRenderer::getTurtleSpaceHeight


double NullTurtleSpaceRenderer::getDesiredTurtleWidth(void)
{
    _D(("NullTurtleSpaceRenderer::getDesiredTurtleWidth() called!\n"));
    return(w * 0.02);
} // NullTurtleSpaceRenderer::getDesiredTurtleWidth


double NullTurtleSpaceRenderer::getDesiredTurtleHeight(void)
{
    _D(("NullTurtleSpaceRenderer::getDesiredTurtleHeight() called!\n"));
    return(h * 0.02);
} // NullTurtleSpaceRenderer::getDesiredTurtleHeight


void NullTurtleSpaceRenderer::renderString(double x, double y, double angle,
                                          float r, float b, float g, float a,
                                          const char *str)
{
    _D(("NullTurtleSpaceRenderer::renderString() called!\n"));
} // NullTurtleSpaceRenderer::renderString


void NullTurtleSpaceRenderer::renderLine(double _x1, double _y1,
                                        double _x2, double _y2,
                                        float r, float b,
                                        float g, float a)
{
    _D(("NullTurtleSpaceRenderer::renderLine() called!\n"));
} // NullTurtleSpaceRenderer::renderLine


void NullTurtleSpaceRenderer::renderTurtle(Turtle *t) throw (ExecException *)
{
    _D(("NullTurtleSpaceRenderer::renderTurtle() called!\n"));
} // NullTurtleSpaceRenderer::renderTurtle


void NullTurtleSpaceRenderer::blankTurtle(Turtle *t) throw (ExecException *)
{
    _D(("NullTurtleSpaceRenderer::blankTurtle() called!\n"));
} // NullTurtleSpaceRenderer::blankTurtle


void NullTurtleSpaceRenderer::cleanup(void) throw (ExecException *)
{
    _D(("NullTurtleSpaceRenderer::cleanup() called!\n"));
} // NullTurtleSpaceRenderer::cleanup


void NullTurtleSpaceRenderer::renderToOffscreen(void)
{
    _D(("NullTurtleSpaceRenderer::renderToOffscreen() called!\n"));
} // NullTurtleSpaceRenderer::renderToOffscreen


void NullTurtleSpaceRenderer::renderToScreen(void)
{
    _D(("NullTurtleSpaceRenderer::renderToScreen() called!\n"));
} // NullTurtleSpaceRenderer::renderToScreen


TurtleSpaceRenderer *__platformBuildStandaloneRenderer(char *winTitle,
                                                       int *argc, char ***argv)
{
    return(new NullTurtleSpaceRenderer());
} // __platformBuildStandaloneRenderer


bool __platformRendererDoEvents(void)
{
    _D(("NullTurtleSpaceRenderer's __platformRendererDoEvents() called!\n"));
    return(true);
} // __platformRendererDoEvents

// end of NullTurtleSpaceRenderer.cpp ...

