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

#include "turtlespace/TurtleSpace.h"

TurtleSpace::TurtleSpace(TurtleSpaceRenderer *_renderer) :
        dueNorth(270.0), turtles(NULL), turtlesArraySize(0),
        turtlePool(new TobyStack()), fenceEnabled(false), turtle(NULL),
        renderer(_renderer), turtleSync(__platformBuildMutex()),
        renderingToOffscreen(false)
{
    assert(_renderer != NULL);
} // Constructor


TurtleSpace::~TurtleSpace(void)
{
    for (int i = 0; i < turtlesArraySize; i++)
    {
        if (turtles[i] != NULL)
            delete turtles[i];
    } // for

    if (turtles != NULL)
        free(turtles);

    while (turtlePool->isEmpty() == false)
        delete ((Turtle *) turtlePool->remove(0));

    delete turtlePool;
} // Destructor


static inline void clampFloat(float *x, float min, float max)
{
    if (*x < min)
        *x = min;
    else if (*x > max)
        *x = max;
} // clampFloat


static inline void clampColorValues(float *r, float *g, float *b, float *a)
{
    clampFloat(r, 0.0, 1.0);
    clampFloat(g, 0.0, 1.0);
    clampFloat(b, 0.0, 1.0);
    clampFloat(a, 0.0, 1.0);
} // clampColorValues


TurtleSpaceRenderer *TurtleSpace::getTurtleSpaceRenderer(void)
{
    return(renderer);
} // TurtleSpace::getTurtleSpaceRenderer


double TurtleSpace::getTurtleSpaceWidth(void)
{
    return(renderer->getTurtleSpaceWidth());
} // TurtleSpace::getTurtleSpaceWidth


double TurtleSpace::getTurtleSpaceHeight(void)
{
    return(renderer->getTurtleSpaceHeight());
} // TurtleSpace::getTurtleSpaceWidth


double TurtleSpace::getDueNorth(void)
{
    return(dueNorth);
} // TurtleSpace::getDueNorth


void TurtleSpace::setDueNorth(double angle)
{
    dueNorth = angle;
} // TurtleSpace::setDueNorth


Turtle *TurtleSpace::getTurtleByIndex(int turtleIndex)
{
    Turtle *retVal = NULL;

    turtleSync->request();

    if (turtles != NULL)
    {
        if ((turtleIndex >= 0) && (turtleIndex < turtlesArraySize))
            retVal = turtles[turtleIndex];
    } // if

    turtleSync->release();

    return(retVal);
} // TurtleSpace::getTurtleByIndex


int TurtleSpace::getTurtleCount(void)
{
    int retVal = 0;

    turtleSync->request();

    if (turtles != NULL)
    {
        for (int i = 0; i < turtlesArraySize; i++)
        {
            if (turtles[i] != NULL)
                retVal++;
        } // for
    } // if

    turtleSync->release();

    return(retVal);
} // TurtleSpace::getTurtleCount


Turtle *TurtleSpace::buildNewTurtle(void) throw (ExecException *)
{
    Turtle *retVal = NULL;

    turtleSync->request();
    retVal = (Turtle *) turtlePool->pop();
    turtleSync->release();

    if (retVal == NULL)
        retVal = new Turtle();

    defaultTurtle(retVal);

    return(retVal);
} // TurtleSpace::buildNewTurtle


int TurtleSpace::addTurtle(void) throw (ExecException *)
{
    Turtle *newTurtle = buildNewTurtle();

    turtleSync->request();

    if (turtles != NULL)
    {
        for (int i = 0; i < turtlesArraySize; i++)
        {
            if (turtles[i] == NULL)
            {
                turtles[i] = newTurtle;
                turtleSync->release();
                return(i);
            } // if
        } // for
    } // if

    // if we landed here, we need to increase the size of the array...

    turtlesArraySize++;
    turtles = (Turtle **)realloc(turtles, turtlesArraySize * sizeof(Turtle *));
    assert(turtles != NULL);
    turtles[turtlesArraySize - 1] = newTurtle;
    turtleSync->release();
    return(turtlesArraySize - 1);
} // TurtleSpace::addTurtle


bool TurtleSpace::removeTurtle(int turtleIndex) throw (ExecException *)
{
    bool retVal = false;
    Turtle *t = getTurtleByIndex(turtleIndex);

    if (turtle == t)     // set selected turtle to none if removing selected.
        turtle = NULL;

    // !!! Should we throw an ExecException if turtleIndex is bogus?

    if (t != NULL)   // ...if removing a valid, existing turtle...
    {
        renderer->blankTurtle(t);
        t->setVisible(false);
        turtleSync->request();
        turtlePool->push(t);
        turtles[turtleIndex] = NULL;
        turtleSync->release();
        retVal = true;
    } // if

    return(retVal);
} // TurtleSpace::removeTurtle


void TurtleSpace::removeAllTurtles(void) throw (ExecException *)
{
    turtleSync->request();

    if (turtles != NULL)
    {
        for (int i = 0; i < turtlesArraySize; i++)
        {
            if (turtles[i] != NULL)
                removeTurtle(i);
        } // for
    } // if

    turtleSync->release();
} // TurtleSpace::removeAllTurtles


void TurtleSpace::grabTurtleSpace(void)
{
    renderer->notifyGrabbed();
} // TurtleSpace::grabTurtleSpace


void TurtleSpace::releaseTurtleSpace(void)
{
    renderer->notifyUngrabbed();
} // TurtleSpace::grabTurtleSpace


void TurtleSpace::verifyPointInsideFence(double x, double y)
                                                    throw (ExecException *)
{
    if (fenceEnabled == true)
    {
        if ( (x < 0.00) || (x > renderer->getTurtleSpaceWidth()) ||
             (y < 0.00) || (y > renderer->getTurtleSpaceHeight()) )
        {
            ExecException::_throw(TobyLanguage::TURTLE_FENCED);
        } // if
    } // if
} // TurtleSpace::verifyPointInsideFence


void TurtleSpace::enableFence(void) throw (ExecException *)
{
    setFence(true);
} // TurtleSpace::enableFence


void TurtleSpace::disableFence(void) throw (ExecException *)
{
    setFence(false);
} // TurtleSpace::disableFence


void TurtleSpace::setFence(bool buildFence) throw (ExecException *)
{
    fenceEnabled = buildFence;

    turtleSync->request();

    try
    {
        if ((buildFence == true) && (turtles != NULL))
        {
            Turtle *t;

            for (int i = 0; i < turtlesArraySize; i++)
            {
                t = turtles[i];
                if (t != NULL)
                    verifyPointInsideFence(t->getX(), t->getY());
            } // for
        } // if
    } // try

    catch (TobyObject *obj)
    {
        turtleSync->release();
        throw obj;
    } // catch

    turtleSync->release();
} // TurtleSpace::setFence


void TurtleSpace::useTurtle(int turtleIndex) throw (ExecException *)
{
    turtleSync->request();

    Turtle *newTurtle = getTurtleByIndex(turtleIndex);
    if (newTurtle != NULL)
        turtle = newTurtle;
    else
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::BAD_ARGUMENT);
    } // else

    turtleSync->release();
} // TurtleSpace::useTurtle


double TurtleSpace::getTurtleX(void) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    double retval = turtle->getX();
    turtleSync->release();
    return(retval);
} // TurtleSpace::getTurtleX


double TurtleSpace::getTurtleY(void) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    double retval = turtle->getY();
    turtleSync->release();
    return(retval);
} // TurtleSpace::getTurtleY


double TurtleSpace::getTurtleWidth(void) throw (ExecException *)
{
    turtleSync->request();
    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    double retval = turtle->getWidth();
    turtleSync->release();
    return(retval);
} // TurtleSpace::getTurtleWidth


double TurtleSpace::getTurtleHeight(void) throw (ExecException *)
{
    turtleSync->request();
    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    double retval = turtle->getHeight();
    turtleSync->release();
    return(retval);
} // TurtleSpace::getTurtleHeight


void TurtleSpace::setTurtleX(double x) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if (x == turtle->getX())
        turtleSync->release();
    else
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setX(x);

        if (isVis == true)
            renderer->renderTurtle(turtle);

        double y = turtle->getY();
        turtleSync->release();
        verifyPointInsideFence(x, y);
    } // else
} // TurtleSpace::setTurtleX


void TurtleSpace::setTurtleY(double y) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if (y == turtle->getY())
        turtleSync->release();
    else
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setY(y);

        if (isVis == true)
            renderer->renderTurtle(turtle);

        double x = turtle->getX();
        turtleSync->release();
        verifyPointInsideFence(x, y);
    } // else
} // TurtleSpace::setTurtleY


void TurtleSpace::setTurtleXY(double x, double y) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if ((y == turtle->getY()) && (x == turtle->getX()))
        turtleSync->release();
    else
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setXY(x, y);

        if (isVis == true)
            renderer->renderTurtle(turtle);

        turtleSync->release();
        verifyPointInsideFence(x, y);
    } // else
} // TurtleSpace::setTurtleXY


void TurtleSpace::setTurtleWidth(double newWidth) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if (newWidth != turtle->getWidth())
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setWidth(newWidth);

        if (isVis == true)
            renderer->renderTurtle(turtle);
    } // if

    turtleSync->release();
} // TurtleSpace::setTurtleWidth


void TurtleSpace::setTurtleHeight(double newHeight) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if (newHeight != turtle->getHeight())
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setHeight(newHeight);

        if (isVis == true)
            renderer->renderTurtle(turtle);
    } // if

    turtleSync->release();
} // TurtleSpace::setTurtleHeight


void TurtleSpace::setTurtleVisibility(Turtle *t, bool isVis)
                                                    throw (ExecException *)
{
    if (isVis != t->getVisible())
    {
        t->setVisible(isVis);

        if (isVis == true)
            renderer->renderTurtle(t);
        else
            renderer->blankTurtle(t);
    } // if
} // TurtleSpace::setTurtleVisibility


void TurtleSpace::setTurtleVisibility(bool isVis) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    setTurtleVisibility(turtle, isVis);
    turtleSync->release();
} // TurtleSpace::setTurtleVisible
        

void TurtleSpace::showTurtle(void) throw (ExecException *)
{
    setTurtleVisibility(true);
} // TurtleSpace::showTurtle


void TurtleSpace::showAllTurtles(void) throw (ExecException *)
{
    turtleSync->request();

    for (int i = 0; i < turtlesArraySize; i++)
    {
        if (turtles[i] != NULL)
            setTurtleVisibility(turtles[i], true);
    } // for

    turtleSync->release();
} // TurtleSpace::showAllTurtles


void TurtleSpace::hideTurtle(void) throw (ExecException *)
{
    setTurtleVisibility(false);
} // TurtleSpace::hideTurtle


void TurtleSpace::hideAllTurtles(void) throw (ExecException *)
{
    turtleSync->request();

    for (int i = 0; i < turtlesArraySize; i++)
    {
        if (turtles[i] != NULL)
            setTurtleVisibility(turtles[i], false);
    } // for

    turtleSync->release();
} // TurtleSpace::hideAllTurtles


void TurtleSpace::advanceTurtle(double distance) throw (ExecException *)
{
    if (distance == 0.0)
        return;

    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    double angle = turtle->getAngle();
    double turtleX = turtle->getX();
    double turtleY = turtle->getY();
    double x, y;

    TobyGeometry::calculateLine(angle, distance, turtleX, turtleY, &x, &y);

    bool isVis = turtle->getVisible();
    if (isVis == true)
        renderer->blankTurtle(turtle);

    if (turtle->isPenDown())   // draw the line covering path turtle took?
    {
        float r, g, b, a;
        turtle->getPenColor(&r, &g, &b, &a);

// !!! needs to clamp lines to dimensions of TurtleSpace.
        renderer->renderLine(turtleX, turtleY, x, y, r, b, g, a);
    } // if

    turtle->setXY(x, y);

    if (isVis == true)
        renderer->renderTurtle(turtle);

    turtleSync->release();

    verifyPointInsideFence(x, y);
} // TurtleSpace::advanceTurtle


void TurtleSpace::setTurtleAngle(double angle) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    if (angle != turtle->getAngle())
    {
        bool isVis = turtle->getVisible();
        if (isVis == true)
            renderer->blankTurtle(turtle);

        turtle->setAngle(angle);

        if (isVis == true)
            renderer->renderTurtle(turtle);
    } // if

    turtleSync->release();
} // TurtleSpace::setTurtleAngle


void TurtleSpace::rotateTurtle(double degree) throw (ExecException *)
{
    if (degree == 0.0)
        return;

    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    setTurtleAngle(turtle->getAngle() + degree);

    turtleSync->release();
} // TurtleSpace::rotateTurtle


void TurtleSpace::homeTurtle(Turtle *t) throw (ExecException *)
{
    double x = renderer->getTurtleSpaceWidth() / 2;
    double y = renderer->getTurtleSpaceHeight() / 2;

    turtleSync->request();

    if ((x != t->getX()) || (y != t->getY()))
    {
        bool isVis = t->getVisible();
        if (isVis == true)
            renderer->blankTurtle(t);

        t->setXY(x, y);

        if (isVis == true)
            renderer->renderTurtle(t);
    } // if

    turtleSync->release();
} // TurtleSpace::homeTurtle


void TurtleSpace::homeTurtle(void) throw (ExecException *)
{
        // !!! This code is always the same... can we get this in a macro
        // !!!  or inline function?
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    homeTurtle(turtle);

    turtleSync->release();
} // TurtleSpace::homeTurtle


void TurtleSpace::homeAllTurtles(void) throw (ExecException *)
{
    turtleSync->request();

    for (int i = 0; i < turtlesArraySize; i++)
    {
        if (turtles[i] != NULL)
            homeTurtle(turtles[i]);
    } // for

    turtleSync->release();
} // TurtleSpace::homeAllTurtles


void TurtleSpace::defaultTurtle(Turtle *t) throw (ExecException *)
{
    assert(t != NULL);

    float r = getDefaultPenRed();
    float g = getDefaultPenGreen();
    float b = getDefaultPenBlue();
    float a = getDefaultPenAlpha();

    clampColorValues(&r, &g, &b, &a); // better safe than sorry.

    bool isVis = t->getVisible();

    if (t->getAngle() != dueNorth)
    {
        if (isVis == true)
        {
            renderer->blankTurtle(t);
            isVis = false;
        } // if
        t->setAngle(dueNorth);
    } // if

    t->setPenDown(true);
    t->setPenColor(r, g, b, a);
    t->setSize(20.0);

    double x = renderer->getTurtleSpaceWidth() / 2;
    double y = renderer->getTurtleSpaceHeight() / 2;

    if ((t->getX() != x) || (t->getY() != y))
    {
        if (isVis == true)
        {
            renderer->blankTurtle(t);
            isVis = false;
        } // if
        t->setXY(x, y);
    } // if

    t->setVisible(true);
    if (isVis == false)
        renderer->renderTurtle(t);
} // TurtleSpace::defaultTurtle


void TurtleSpace::defaultTurtle(void) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    defaultTurtle(turtle);

    turtleSync->release();
} // TurtleSpace::defaultTurtle


void TurtleSpace::defaultAllTurtles(void) throw (ExecException *)
{
    turtleSync->request();

    if (turtles != NULL)
    {
        for (int i = 0; i < turtlesArraySize; i++)
            defaultTurtle(turtles[i]);
    } // if

    turtleSync->release();
} // TurtleSpace::defaultAllTurtles


void TurtleSpace::setPenColor(float r, float g, float b, float a)
                                                        throw (ExecException *)
{
    clampColorValues(&r, &g, &b, &a);

    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    turtle->setPenColor(r, g, b, a);

    turtleSync->release();
} // TurtleSpace::setPenColor


void TurtleSpace::setPen(bool drawing) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    turtle->setPenDown(drawing);

    turtleSync->release();
} // TurtleSpace::setPen


void TurtleSpace::setPenUp(void) throw (ExecException *)
{
    setPen(false);
} // TurtleSpace::setPenUp


void TurtleSpace::setPenDown(void) throw (ExecException *)
{
    setPen(true);
} // TurtleSpace::setPenDown


void TurtleSpace::drawString(const char *str) throw (ExecException *)
{
    turtleSync->request();

    if (turtle == NULL)
    {
        turtleSync->release();
        ExecException::_throw(TobyLanguage::NOCURTURTLE);
    } // if

    float r, g, b, a;
    double x = turtle->getX();
    double y = turtle->getY();
    double angle = turtle->getAngle();
    turtle->getPenColor(&r, &g, &b, &a);

    turtleSync->release();

    renderer->renderString(x, y, angle, r, g, b, a, str);
} // TurtleSpace::renderString


void TurtleSpace::cleanup(void) throw (ExecException *)
{
    renderer->cleanup();

    turtleSync->request();

    for (int i = 0; i < turtlesArraySize; i++)
    {
        if (turtles[i]->getVisible())
            renderer->renderTurtle(turtles[i]);
    } // for

    turtleSync->release();
} // TurtleSpace::cleanup


void TurtleSpace::renderToOffscreen(void)
{

    // !!! race condition?

    if (!renderingToOffscreen)
    {
        renderer->renderToOffscreen();
        renderingToOffscreen = true;
    } // if
} // TurtleSpace::renderToOffscreen


void TurtleSpace::renderToScreen(void)
{

    // !!! race condition?

    if (renderingToOffscreen)
    {
        renderer->renderToScreen();
        renderingToOffscreen = false;
    } // if
} // TurtleSpace::renderToScreen


bool TurtleSpace::isRenderingToOffscreen(void)
{
    return(renderingToOffscreen);
} // TurtleSpace::isRenderingToOffscreen


bool TurtleSpace::isRenderingToScreen(void)
{
    return(!renderingToOffscreen);
} // TurtleSpace::isRenderingToScreen

// end of TurtleSpace.cpp ...

