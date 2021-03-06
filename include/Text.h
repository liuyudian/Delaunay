//
// Created by delaunay on 28/3/20.
//

#ifndef DELAUNAY_TEXT_H
#define DELAUNAY_TEXT_H


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Text
{
    /*******************************************************************************************************************
    * Private members
    *******************************************************************************************************************/
    float fx;
    float fy;
    string strText;
public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Text(float fxIn, float fyIn, string &strTextIn) : fx(fxIn), fy(fyIn), strText(strTextIn) {};

    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    float getX() const { return fx; }
    float getY() const { return fy; }
    const string &getText() const { return strText; }
};


#endif //DELAUNAY_TEXT_H
