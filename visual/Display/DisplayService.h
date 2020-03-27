//
// Created by delaunay on 25/3/20.
//

#ifndef DELAUNAY_DISPLAYSERVICE_H
#define DELAUNAY_DISPLAYSERVICE_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <GL/glut.h>
#include "defines.h"
#include "DisplayableColor.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayService
{
public:
    /**
     *
     */
    static void clear()  { glClear(GL_COLOR_BUFFER_BIT);} ;

    /**
     *
     */
    static void flush()  { glutSwapBuffers(); };

    /**
     *
     */
    static void startPoints() { glBegin(GL_POINTS); };

    /**
     *
     */
    static void startCircle() { glBegin(GL_LINE_LOOP); };

    /**
     *
     */
    static void startLine()   { glBegin(GL_LINE_STRIP); };

    /**
     *
     */
    static void finish() { glEnd(); };

    /**
     *
     * @param size
     */
    static void setPointSize(float size) { glPointSize(size);} ;

    /**
     *
     * @param size
     */
    static void setLineSize(float size) { glLineWidth(size);} ;

    /**
     * Set color functions.
     */
    static void setColor(const DisplayableColor &color) {glColor3f(color.getRed(), color.getGreen(),color.getBlue()); };

    /**
     * @fn          display
     * @brief       Display point
     *
     * @param x     x coordinate
     * @param y     y coordinate
     */
    static void display(TYPE x, TYPE y)
    {
        glVertex2f(x, y);
    }
};


#endif //DELAUNAY_DISPLAYSERVICE_H
