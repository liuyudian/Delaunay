//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDRESULT_H
#define DELAUNAY_COMMANDRESULT_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "Dcel.h"
#include "defines.h"
#include "DisplayableFactory.h"
#include "Point.h"
#include "StoreService.h"

using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResult
{
protected:
    bool isSuccess;
    StoreService *storeService;

public:
    CommandResult(bool isSuccessIn, StoreService *storeServiceIn) : isSuccess(isSuccessIn), storeService(storeServiceIn) {};
    virtual void updateStatus() {};
    virtual void createDisplayables(vector<Displayable*> &vDisplayable) {};

    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    bool wasSuccess() const { return isSuccess; }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultNull : public CommandResult
{
public:
    explicit CommandResultNull(bool isSuccess) : CommandResult(isSuccess, nullptr) {};
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultRead : public CommandResult
{
    Dcel *dcel;
public:
    CommandResultRead(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResult(isSuccess, service), dcel(dcelIn) {};

    void updateStatus() override
    {
        storeService->getStatus()->set(false, isSuccess, false, false, false, false);
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (isSuccess)
        {
            // Add figure display.
            vector<Point<TYPE>> vPoints;
            for (size_t i=0; i< Config::getNPoints(); i++)
            {
                vPoints.push_back(*dcel->getRefPoint(i));
            }
            vDisplayable.push_back(DisplayableFactory::createPointsSet(vPoints));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
// TODO This could be the same as CommandResultRead
class CommandResultTriangulation : public CommandResult
{
    Dcel *dcel;
public:
    CommandResultTriangulation(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResult(isSuccess, service), dcel(dcelIn) {};

    void updateStatus() override
    {
        storeService->getStatus()->set(false, true, true, false, false, false);
    };

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (isSuccess)
        {
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultDelaunay : public CommandResultTriangulation
{
    Dcel *dcel;
public:
    CommandResultDelaunay(bool isSuccess, StoreService *service, Dcel *dcelIn) : CommandResultTriangulation(isSuccess, service, dcelIn) {};

    void updateStatus() override
    {
        storeService->getStatus()->set(false, true, true, true, false, false);
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultPolygon : public CommandResult
{
    Polygon *polygon;
public:
    CommandResultPolygon(bool isSuccess, StoreService *service, Polygon *polygonIn) : CommandResult(isSuccess, service), polygon(polygonIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (isSuccess)
        {
            // Add polygon points
            vector<Point<TYPE>> vPoints;
            polygon->getPoints(vPoints);
            vDisplayable.push_back(DisplayableFactory::createPolygon(vPoints));
        }
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResulVoronoi : public CommandResult
{
    Dcel *dcel;
    Voronoi *voronoi;
public:
    CommandResulVoronoi(bool isSuccess, StoreService *service, Dcel *dcelIn, Voronoi *voronoiIn) :  CommandResult(isSuccess, service),
                                                                                                    dcel(dcelIn),
                                                                                                    voronoi(voronoiIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (isSuccess)
        {
            // Add delaunay and voronoi
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getVoronoi()->getRefDcel()));
        }
    };

    void updateStatus() override
    {
        storeService->getStatus()->set(false, true, true, true, true, false);
    };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResultGabriel : public CommandResult
{
    Dcel *dcel;
    Gabriel *gabriel;
public:
    CommandResultGabriel(bool isSuccess, StoreService *service, Dcel *dcelIn, Gabriel *gabrielIn) : CommandResult(isSuccess, service),
                                                                                                    dcel(dcelIn),
                                                                                                    gabriel(gabrielIn) {};

    void createDisplayables(vector<Displayable*> &vDisplayable) override
    {
        if (isSuccess)
        {
            Point<TYPE> *vertex1;	    // First vertex.
            Point<TYPE> *vertex2;	    // Second vertex.
            Dcel	*dcelRef;

            // Get reference to gabriel dcel.
            dcelRef = gabriel->getDcel();

            // Draw Gabriel edges.
            vector<Line> vLines;
            // https://github.com/juannavascalvente/Delaunay/issues/60
            for (size_t edgeIndex=0; edgeIndex<gabriel->getSize() ;edgeIndex++)
            {
                // Check if current edge mamtches Gabriel restriction.s
                if (gabriel->isSet(edgeIndex))
                {
                    // Get origin vertex of edge.
                    vertex1 = dcelRef->getRefPoint(dcelRef->getOrigin(edgeIndex)-1);

                    // Get destination vertex of edge.
                    vertex2 = dcelRef->getRefPoint(dcelRef->getOrigin(dcelRef->getTwin(edgeIndex)-1)-1);

                    Line line(*vertex1, *vertex2);
                    vLines.push_back(line);
                }
            }

            // Add Delaunay and gabriel graph
            vDisplayable.push_back(DisplayableFactory::createDcel(storeService->getDcel()));
            vDisplayable.push_back(DisplayableFactory::createPolyLine(vLines));
        }
    };

    void updateStatus() override
    {
        storeService->getStatus()->set(false, true, true, true, true, true);
    };
};


#endif //DELAUNAY_COMMANDRESULT_H
