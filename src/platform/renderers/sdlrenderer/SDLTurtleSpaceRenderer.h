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

#ifndef _INCLUDE_SDLTURTLESPACERENDERER_H_
#define _INCLUDE_SDLTURTLESPACERENDERER_H_

#include "SDL.h"
#include "platform/renderers/fbrenderer/FrameBufferTurtleSpaceRenderer.h"

/*
 * An implementation of TurtleSpaceRenderer that does its drawing through
 *  FrameBufferTurtleSpaceRenderer as a superclass, and uses SDL for getting
 *  the bits to the screen. SDL stands for Simple Directmedia Layer, and
 *  is a fast, easy-to-use, cross-platform library for graphics, sound,
 *  input, etc.
 *
 *    SDL can be found at http://www.libsdl.org/
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class SDLTurtleSpaceRenderer : public FrameBufferTurtleSpaceRenderer
{
public:
    SDLTurtleSpaceRenderer(Uint32 sdlflags);
    virtual ~SDLTurtleSpaceRenderer(void);

    virtual bool screenResize(int _w, int _h);

protected:
    virtual void createSurface(int _w, int _h);
    virtual void deleteSurface(void);
    virtual toby_uint32 *getSurface(void);
    virtual void putToScreen(toby_uint32 *pix, int x, int y, int w, int h);
    virtual void putMultToScreen(toby_uint32 *pix, int rectCount,
                                    FrameBufferRect *rects);

    virtual bool doEvents(void);

private:
    Uint32 sdlflags;
    SDL_Surface *screen;
    SDL_Surface *swsurface;
}; // class SDLTurtleSpaceRenderer

#endif // !defined _INCLUDE_SDLTURTLESPACERENDERER_H_

// end of SDLTurtleSpaceRenderer.h ...

