/*
 * Circle.h
 *
 *  Created on: Jul 28, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_CIRCLE_H_
#define INCLUDE_CIRCLE_H_

#include "Point.h"

//****************************************************************************
//                           	CIRCLE CLASS
//****************************************************************************
class Circle
{
	//------------------------------------------------------------------------
	// Attributes
	//------------------------------------------------------------------------
	Point<TYPE> p,q,r;			// Points in circle.
	Point<TYPE> centre;			// Circle centre.
	TYPE radius;				// Circle radius.

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	void computeCentre();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Circle( );
	Circle( Point<TYPE> *centre, TYPE radius);
	Circle( Point<TYPE> *p,	Point<TYPE> *q, Point<TYPE> *r);

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	inline double area() { return(PI*pow(this->radius,2)); };
	inline double length() { return(2*PI*this->radius); };
	bool inCircle( Point<TYPE> *s);

	// GET/SET functions.
	inline Point<TYPE>* getRefCentre() { return( &this->centre); };
	inline double getRadius() { return(this->radius); };
};

#endif /* INCLUDE_CIRCLE_H_ */
