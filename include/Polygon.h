/*
 * Polygon.h
 *
 *  Created on: Jul 19, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_POLYGON_H_
#define INCLUDE_POLYGON_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Line.h"
#include "Point.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Polygon
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Constructor/Destructor
    *******************************************************************************************************************/
	Polygon() = default;
	explicit Polygon(int n) : vPoints(n) {};
	virtual ~Polygon() = default;

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
	inline int getNElements() { return(vPoints.size()); };
    inline void getPoints(vector<Point<TYPE>> &vPointsOut) { vPointsOut = vPoints; };
	inline Point<TYPE> at(int index) { return(vPoints.at(index)); };
	inline void	add(Point<TYPE> &p) { vPoints.push_back(p); };
	inline void reset() { vPoints.clear(); };

    /***************************************************************************
    * Name: 	centroid
    * IN:		NONE
    * OUT:		center		polygon centroid.
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	computes the centroid of the polygon.
    ***************************************************************************/
	void 	centroid(Point<TYPE> &center);

    /***************************************************************************
    * Name: 	getIntersections
    * IN:		line		line to check
    * OUT:		set			set of edges that intersect line.
    * RETURN:	true 		if intersect.
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	gets the set of edges(two maximum) that intersects the
    * 				polygon.
    ***************************************************************************/
	bool	getIntersections(Line &line, vector<int> &intersection);

    /***************************************************************************
    * Name: 	isInternal
    * IN:		p			point to check
    * OUT:		NONE
    * RETURN:	true 		if point is interior to polygon.
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	Checks if the input point is interior to the polygon
    ***************************************************************************/
	bool	isInternal(Point<TYPE> &p);

    /***************************************************************************
    * Name: 	print
    * IN:		out			output stream
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	print the set of points.
    ***************************************************************************/
	void 	print(std::ostream& out);

    /***************************************************************************
    * Name: 	toStr
    * IN:		NONE
    * OUT:		NONE
    * RETURN:	string		set of points as text.
    * GLOBAL:	NONE
    * Description: 	convert to string the set of points
    ***************************************************************************/
	string 	toStr();
};

#endif /* INCLUDE_POLYLINE_H_ */
