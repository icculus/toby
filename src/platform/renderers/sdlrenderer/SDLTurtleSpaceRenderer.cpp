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

#include "platform/renderers/sdlrenderer/SDLTurtleSpaceRenderer.h"


SDLTurtleSpaceRenderer::SDLTurtleSpaceRenderer(Uint32 flags)
    : FrameBufferTurtleSpaceRenderer(),
      sdlflags(flags),
      swsurface(NULL)
{
} // Constructor


SDLTurtleSpaceRenderer::~SDLTurtleSpaceRenderer(void)
{
    deleteSurface();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
} // Destructor


bool SDLTurtleSpaceRenderer::screenResize(int _w, int _h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return(false);

    SDL_WM_SetCaption("Toby", "Toby");
    SDL_Surface *screen = SDL_SetVideoMode(_w, _h, 32, sdlflags);
    return(screen != NULL);
} // SDLTurtleSpaceRenderer::screenResize


void SDLTurtleSpaceRenderer::createSurface(int _w, int _h)
{
    swsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, _w, _h, 32,
                                     0x00FF0000, 0x0000FF00,
                                     0x000000FF, 0xFF000000);
} // SDLTurtleSpaceRenderer::createSurface


void SDLTurtleSpaceRenderer::deleteSurface(void)
{
    if (swsurface != NULL)
    {
        SDL_FreeSurface(swsurface);
        swsurface = NULL;
    } // if
} // SDLTurtleSpaceRenderer::deleteSurface


void SDLTurtleSpaceRenderer::putToScreen(toby_uint32 *pix,
                                         int x, int y,
                                         int w, int h)
{
    assert(pix == swsurface->pixels);
    SDL_Rect rect = {x, y, w, h};
    SDL_Surface *screen = SDL_GetVideoSurface();
    assert(screen != NULL);

    if (SDL_MUSTLOCK(screen))
        SDL_LockSurface(screen);

    SDL_BlitSurface(swsurface, &rect, screen, &rect);

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    SDL_UpdateRect(screen, x, y, w, h);  // !!! need this?
} // SDLTurtleSpaceRenderer::putToScreen


void SDLTurtleSpaceRenderer::putMultToScreen(toby_uint32 *pix, int rectCount,
                                             FrameBufferRect *rects)
{
    assert(pix == swsurface->pixels);
    SDL_Surface *screen = SDL_GetVideoSurface();
    assert(screen != NULL);

    if (SDL_MUSTLOCK(screen))
        SDL_LockSurface(screen);

    for (int i = 0; i < rectCount; i++)
    {
        SDL_Rect r = {rects[i].x, rects[i].y, rects[i].w, rects[i].h};
        SDL_BlitSurface(swsurface, &r, screen, &r);
        SDL_UpdateRect(screen, r.x, r.y, r.w, r.h);
    } // for

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

//    SDL_UpdateRects(screen, rectCount, (SDL_Rect *) rects);  // !!! need this?
} // SDLTurtleSpaceRenderer::putToScreen


toby_uint32 *SDLTurtleSpaceRenderer::getSurface(void)
{
    if (swsurface == NULL)
        return(NULL);

    return((toby_uint32 *) swsurface->pixels);
} // SDLTurtleSpaceRenderer::getSurface


TurtleSpaceRenderer *__platformBuildStandaloneRenderer(char *winTitle,
                                                       int *argc, char ***argv)
{
    SDLTurtleSpaceRenderer *retval = new SDLTurtleSpaceRenderer(0);
    SDL_WM_SetCaption(winTitle, winTitle);
    return(retval);
} // __platformBuildStandaloneRenderer


bool __platformRendererDoEvents(void)
{
    bool retval = true;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            retval = false;
    } // while

    return(retval);
} // __platformRendererDoEvents

// end of SDLTurtleSpaceRenderer.cpp ...

