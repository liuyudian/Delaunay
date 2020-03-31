/*
 * Process.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Process.h"
#include "Command.h"
#include "CommandFactory.h"
#include "DcelFigureBuilder.h"
#include "DcelGenerator.h"
#include "DcelReader.h"
#include "DcelWriter.h"
#include "DelaunayIO.h"
#include "DisplayableFactory.h"
#include "GabrielIO.h"
#include "LineFactory.h"
#include "MenuOption.h"
#include "VoronoiIO.h"

#include <GL/glut.h>


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
Process *Process::instance = nullptr;
typedef Point<TYPE> PointT;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Process::Process(int argc, char **argv, bool printData, StoreService *storeServiceIn)
{
	string 	fileName;			// Configuration file name.

	// Initialize configuration.
	fileName = argv[2];
	Config::readConfig(fileName);

	// Check flag to print data to screen.
	this->log = new Logging("log.txt", printData);

    storeService = storeServiceIn;
    dispManager = new DisplayManager(argc, argv);

	this->m = Menu(storeServiceIn->getStatus());

	// Function to execute by GLUT.
	glutDisplayFunc(executeWrapper);
}


Process::~Process()
{
	// Deallocate resources
	delete this->log;
	delete this->dispManager;
	delete storeService;
}


void Process::start()
{
	// GLUT main function.
	glutMainLoop();
}


void Process::setInstance(Process *process)
{
	instance = process;
}


/***************************************************************************
* Name: executeWrapper
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: wrapper to call "execute" function.
***************************************************************************/
void Process::executeWrapper()
{
	instance->execute();
}


/***************************************************************************
* Name: 	readData
* IN:		option				option selected in menu.
* OUT:		NONE
* RETURN:	true				if successfully read.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	read data from file or generate data set depending on input
* 				option. The data set can be generated:
* 				1.- Randomly uniform
* 				2.- Generated randomly in clusters
* 				3.- Read from flat file.
* 				4.- Read from DCEL.
* 				5.- Read Delaunay (DCEL + graph file).
* 				6.- Read voronoi data.
***************************************************************************/
bool Process::readData(int option)
{
	bool isSuccess; 		// Return value.

//	// Check option to generate/read set.
//	switch (option)
//	{
//		// Generate random set.
//		case RANDOMLY:
//		{
//            isSuccess = DcelGenerator::generateRandom(Config::getNPoints(), this->dcel);
//            status->set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Generate clusters set.
//		case CLUSTER:
//		{
//            isSuccess = DcelGenerator::generateClusters(Config::getNPoints(), Config::getNClusters(), Config::getRadius(), this->dcel);
//			status->set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Read set from flat file.
//		case READ_POINTS_FLAT_FILE:
//		case READ_POINTS_DCEL_FILE:
//		{
//			// Read points from flat file.
//			if(option == READ_POINTS_FLAT_FILE)
//			{
//                isSuccess = DcelReader::readPoints(Config::getInFlatFilename(), true, this->dcel);
//			}
//			// Read points from DCEL file.
//			else
//			{
//                isSuccess = DcelReader::readPoints(Config::getInDCELFilename(), false, this->dcel);
//			}
//			status->set(false, isSuccess, false, false, false, false);
//			break;
//		}
//		// Read dcel file.
//		case READ_DCEL:
//		{
//			// PENDING CHECK IF A DCEL IS CONSISTENT?
//			success = DcelReader::read(Config::getInDCELFilename(), false, this->dcel);
//			this->delaunay.setDCEL(&this->dcel);
//			status->set(false, true, true, false, false, false);
//			break;
//		}
//		// Read Delaunay incremental algorithm files.
//		case READ_DELAUNAY:
//		{
//			// PENDING CHECK IF A DCEL AND GRAPG ARE CONSISTENT?
//			this->delaunay.setDCEL(&this->dcel);
//			success = DelaunayIO::read(Config::getInDCELFilename(),
//                                       Config::getInGraphFilename(), this->delaunay);
//			status->set(false, success, success, success, false, false);
//			this->delaunay.setAlgorithm(INCREMENTAL);
//			break;
//		}
//		// Read Voronoi file.
//		case READ_VORONOI:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			//success = this->voronoi.read(Config::getInVoronoiFilename());
//			//status->set(false, true, !success, !success, true, false);
//			cout << "NOT IMPLEMENTED YET" << endl;
//			break;
//		}
//		// Read Gabriel file.
//		default:
//		{
//			// PENDING: What to allow in menu if only voronoi is read.
//			success = GabrielIO::readBinary(Config::getOutGabrielFilename(), this->gabriel);
//			status->setGabrielCreated(true);
//			break;
//		}
//        default:
//        {
//            // PENDING: What to allow in menu if only voronoi is read.
//            isSuccess = false;
//            break;
//        }
//	}
//
//	// Add figure display.
//    vector<Point<TYPE>> vPoints;
//    for (size_t i=0; i< Config::getNPoints(); i++)
//    {
//        vPoints.push_back(*this->dcel.getRefPoint(i));
//    }
//    dispManager->add(DisplayableFactory::createPointsSet(vPoints));

	return isSuccess;
}

/***************************************************************************
* Name: 	resetData
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	resets the data of the process class.
***************************************************************************/
void Process::resetData()
{
	// Check if Delaunay must be reset.
    Status *status = storeService->getStatus();
	if (status->isDelaunayCreated())
	{
        Delaunay *delaunay = storeService->getDelaunay();
		delaunay->reset();
	}
	// Check if voronoi must be reset.
	if (status->isVoronoiCreated())
	{
        storeService->getVoronoi()->reset();
	}
}


/***************************************************************************
* Name: 	findTwoClosest
* IN:		NONE
* OUT:		index1				index of the first point
* 			index2				index of the second point
* RETURN:	true				if found.
* 			false				i.o.c.
* GLOBAL:	NONE
* Description: 	finds the two closest points in the set of points. If the
* 				Delaunay triangulation exists the uses the incremental
* 				algorithm to locate the points. Otherwise uses a brute force
* 				algorithm.
***************************************************************************/
bool Process::findTwoClosest(int &index1, int &index2)
{
	bool found;		// Return value.

	// Check if Delaunay triangulation computed.
	// PENDING ALSO EXECUTES THIS IF THE DELAUNAY WAS BUILD FROM STAR? IF
	// SO THERE SHOULD BE NOT GRAPH AND IT IS NOT POSSIBLE TO USE THE GRAPH.
    Status *status = storeService->getStatus();
	if (status->isDelaunayCreated())
	{
        Delaunay *delaunay = storeService->getDelaunay();
		found = delaunay->findTwoClosest(index1, index2);
	}
	else
	{
        StarTriangulation *triangulation = storeService->getStarTriang();
		found = triangulation->findTwoClosest(index1, index2);
	}

	return(found);
}


void Process::createDcelPointsInfo(const Dcel &dcelIn, vector<Text> &info)
{
    char	text_Info[50];

    // Draw all points of the set.
    for (size_t i=0; i < dcelIn.getNVertex() ; i++)
    {
        // Get and draw i-point.
        Point<TYPE> *point = dcelIn.getRefPoint(i);
        string strText = std::to_string(i+1);

        Text text(point->getX(), point->getY(), strText);
        info.push_back(text);
    }
}


void Process::createDcelEdgeInfo(const Dcel &dcelIn, vector<Text> &info)
{
    // Loop all edges.
    for (size_t edgeIndex=0; edgeIndex<dcelIn.getNEdges() ;edgeIndex++)
    {
        // Check if twin edge already visited.
        if ((edgeIndex+1) < dcelIn.getTwin(edgeIndex))
        {
            // Check edge is real.
            if (!dcelIn.hasNegativeVertex((int) edgeIndex+1))
            {
                // Get edge extreme points.
                Point<TYPE> origin, dest;	// Extreme points of edges.
                dcelIn.getEdgePoints(edgeIndex, origin, dest);

                // Compute middle point of edge.
                Point<TYPE> middle;         // Middle point of the edge.
                Point<TYPE>::middlePoint(&origin, &dest, &middle);

                // Print information.
                string strText = std::to_string(edgeIndex+1) + " - " + std::to_string(dcelIn.getTwin(edgeIndex));

                Text text(middle.getX(), middle.getY(), strText);
                info.push_back(text);
            }
        }
    }
}


void Process::createDcelFacesInfo(const Dcel &dcelIn, vector<Text> &info)
{
    // Loop all faces (skip external face).
    for (size_t faceId=0; faceId<dcelIn.getNFaces() ;faceId++)
    {
        // If any vertex is imaginary then face is not drawn.
        if (!dcelIn.imaginaryFace(faceId))
        {
            Polygon polygon;

            // Get edge in current face.
            size_t firstEdgeIndex = dcelIn.getFaceEdge(faceId)-1;
            size_t edgeIndex = firstEdgeIndex;
            do
            {
                // Add origin point to polygon.
                Point<TYPE> origin;			// Edge origin point.
                origin = *dcelIn.getRefPoint(dcelIn.getOrigin(edgeIndex)-1);
                polygon.add(origin);

                // Next edge in face.
                edgeIndex = dcelIn.getNext(edgeIndex)-1;
            } while(edgeIndex != firstEdgeIndex);

            // Compute face centroid.
            Point<TYPE> center;			// Middle point of the edge.
            polygon.centroid(center);
            polygon.reset();

            // Print information.
            string strText = std::to_string(faceId);

            Text text(center.getX(), center.getY(), strText);
            info.push_back(text);
        }
    }
}


/***************************************************************************
* Name: execute
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	main loop that processes events from menu and executes the
* 				selected option and updates menu and internal status.
***************************************************************************/
void Process::execute()
{
    Command *cmd=nullptr;           // Command to execute
    CommandResult *result;

	static bool firstTime=true;
	int		option=0;			// Option to be executed.
	bool	error=false;		// Error executing any function.
	bool	quit=false;			// Quit application flag.
	int		index1, index2;		// Index of the two closest points.

	// Get option to be executed.
	option = m.getMenuOption();

	// Execute option.
	switch(option)
	{
		// Read parameters from configuration file.
		case PARAMETERS:
		case STAR_TRIANGULATION:
		case DELAUNAY:
		case CONVEX_HULL:
        case VORONOI:
        case GABRIEL:
        case TRIANGULATION_PATH:
        case VORONOI_PATH:
        case CLOSEST_POINT:
        case FIND_FACE:
		{
            // Create command
            cmd = CommandFactory::create(option, storeService);

            // Run command
            cmd->run();

            // Process results
            result = cmd->getResult();
            if (result->wasSuccess())
            {
                // Update menu status
                result->updateStatus();

                // Get displaybale elements
                vector<Displayable*> vDisplayable(0);
                result->createDisplayables(vDisplayable);
                dispManager->add(vDisplayable);

                dispManager->process();
            }

            // Update menu entries.
            m.updateMenu();

			break;
		}
		// New set of points (generated or read).
		case RANDOMLY:
		case CLUSTER:
        {
            this->resetData();

            // Read configuration file.
            cmd = CommandFactory::create(option, storeService);
            cmd->run();
            result = cmd->getResult();
            if (result->wasSuccess())
            {
                // Update menu status
                result->updateStatus();

                // Get displaybale elements
                vector<Displayable*> vDisplayable;
                result->createDisplayables(vDisplayable);
                dispManager->add(vDisplayable);

                dispManager->process();
            }

            // Update menu entries.
            m.updateMenu();

            break;
        }
		case READ_POINTS_FLAT_FILE:
		case READ_POINTS_DCEL_FILE:
		case READ_DCEL:
		case READ_DELAUNAY:
		case READ_VORONOI:
		case READ_GABRIEL:
		{
			this->resetData();

			// Check option to generate/read set.
			if (this->readData(option))
		    {
//				if (status->isVoronoiCreated())
//				{
//					// Clear screen.
//					this->drawer->drawFigures(VORONOI_DRAW);
//				}
//				if (status->isDelaunayCreated() ||
//					status->isTriangulationCreated())
//				{
//					// Draw triangulation.
//					this->drawer->drawFigures(TRIANGULATION_DRAW);
//				}
//				else
//				{
//					// Draw set of points.
//                    dispManager->process();
//					//this->drawer->drawFigures(SET_DRAW);
//				}

                dispManager->process();

				// Update menu entries.
				m.updateMenu();
		    }
			break;
		}
		// Find the two closest point in the set.
		case TWO_CLOSEST:
		{
			Set<PointT> points(2);	// List of points.

			// Compute the two closest point in the set of points.
			if (this->findTwoClosest(index1, index2))
			{
                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                vector<Point<TYPE>> vPoints;
                vPoints.push_back(*storeService->getDcel()->getRefPoint(index1));
                vPoints.push_back(*storeService->getDcel()->getRefPoint(index2));
                Displayable *closestPoints = DisplayableFactory::createPointsSet(vPoints);
                dispManager->add(closestPoints);

                dispManager->process();
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "The two closest points not found");
				Logging::write(true, Error);
			}
			break;
		}
		// Filter edges whose length is lower than a threshold.
		case FILTER_EDGES:
		{
			// Check if Delaunay triangulation already created.
            Status *status = storeService->getStatus();
			if (status->isTriangulationCreated())
			{
                // Add Delaunay triangulation filtering edges
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel(), Config::getMinLengthEdge());
                dispManager->add(dispDelaunay);

                dispManager->process();
            }
			break;
		}
		// Draw triangles circumcentres.
		case CIRCUMCENTRES:
		{
			// Check if triangulation created.
            Status *status = storeService->getStatus();
			if (status->isDelaunayCreated() ||
				status->isTriangulationCreated())
			{
                // Add circles
                vector<Circle> vCircles;
                for (int faceID=1; faceID<storeService->getDcel()->getNFaces() ;faceID++)
                {
                    // Skip imaginary faces.
                    if (!storeService->getDcel()->imaginaryFace(faceID))
                    {
                        // Get points of the triangle.
                        int		points[NPOINTS_TRIANGLE];	// Triangle points.
                        storeService->getDcel()->getFaceVertices(faceID, points);

                        // Build circle.
                        vector<Point<TYPE>> vPoints;
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[0]-1));
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[1]-1));
                        vPoints.push_back(*storeService->getDcel()->getRefPoint(points[2]-1));
                        Circle circle = Circle(vPoints);
                        vCircles.push_back(circle);
                    }
                }

                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                Displayable *circles = DisplayableFactory::createCircleSet(vCircles);
                dispManager->add(circles);

                dispManager->process();
			}
			break;
		}
		case EDGE_CIRCLES:
		{
			// Check if triangulation created.
            Status *status = storeService->getStatus();
			if (status->isTriangulationCreated())
			{
                int		edgeIndex=0;        // Edge index.
                int		nEdges=0;			// # edges in the storeService->getDcel()->
                TYPE  	radius; 			// Circle radius.
                Line	line;				// Edge line.
                Point<TYPE> origin, dest;  	// Origin and destination points.
                Point<TYPE> middle;	    	// Edge middle point.

                // Add circles
                vector<Circle> vCircles;

                // Loop all faces (but external).
                nEdges = storeService->getDcel()->getNEdges();
                for (edgeIndex=0; edgeIndex<nEdges ;edgeIndex++)
                {
                    // Skip imaginary edges.
                    if (!storeService->getDcel()->hasNegativeVertex(edgeIndex+1))
                    {
                        // Create line.
                        storeService->getDcel()->getEdgePoints(edgeIndex, origin, dest);
                        line = Line(origin, dest);

                        // Compute middle point of edge.
                        line.getMiddle(middle);

                        // Create circle
                        radius = origin.distance(middle);
                        Circle circle = Circle(&middle, radius);

                        // Add circles
                        vCircles.push_back(circle);
                    }
                }

                // Add Delaunay triangulation
                Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
                dispManager->add(dispDelaunay);

                // Add points to display.
                Displayable *circles = DisplayableFactory::createCircleSet(vCircles);
                dispManager->add(circles);

                dispManager->process();
			}
			break;
		}
		// Print DCEL data.
		case DCEL_INFO:
		{
            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(storeService->getDcel());
            dispManager->add(dispDelaunay);

            vector<Text> vPointsInfo;
            this->createDcelPointsInfo(*storeService->getDcel(), vPointsInfo);
            Displayable *dispPointsInfo = DisplayableFactory::createTextSet(vPointsInfo);
            dispManager->add(dispPointsInfo);

            vector<Text> vEdgesInfo;
            this->createDcelEdgeInfo(*storeService->getDcel(), vEdgesInfo);
            Displayable *dispEdgesInfo = DisplayableFactory::createTextSet(vEdgesInfo);
            dispManager->add(dispEdgesInfo);

            vector<Text> vFacesInfo;
            this->createDcelFacesInfo(*storeService->getDcel(), vFacesInfo);
            Displayable *dispFacesInfo = DisplayableFactory::createTextSet(vFacesInfo);
            dispManager->add(dispFacesInfo);

            dispManager->process();
			break;
		}
		// Print Voronoi data.
		case VORONOI_INFO:
		{
		    Voronoi *voronoi = storeService->getVoronoi();

            // Add Delaunay triangulation
            Displayable *dispDelaunay = DisplayableFactory::createDcel(voronoi->getRefDcel());
            dispManager->add(dispDelaunay);

            vector<Text> vPointsInfo;
            this->createDcelPointsInfo(*voronoi->getRefDcel(), vPointsInfo);
            Displayable *dispPointsInfo = DisplayableFactory::createTextSet(vPointsInfo);
            dispManager->add(dispPointsInfo);

            vector<Text> vEdgesInfo;
            this->createDcelEdgeInfo(*voronoi->getRefDcel(), vEdgesInfo);
            Displayable *dispEdgesInfo = DisplayableFactory::createTextSet(vEdgesInfo);
            dispManager->add(dispEdgesInfo);

            vector<Text> vFacesInfo;
            this->createDcelFacesInfo(*voronoi->getRefDcel(), vFacesInfo);
            Displayable *dispFacesInfo = DisplayableFactory::createTextSet(vFacesInfo);
            dispManager->add(dispFacesInfo);

            dispManager->process();
			break;
		}
		// Write points to a flat file.
		case WRITE_POINTS:
		{
			DcelWriter::writePoints(Config::getOutFlatFilename(), INVALID, *storeService->getDcel());
			break;
		}
		// Write points to a DCEL file.
		case WRITE_DCEL:
		{
			DcelWriter::write(Config::getOutDCELFilename(), false, *storeService->getDcel());
			break;
		}
		// Write DCEL and graph files.
		case WRITE_DELAUNAY:
		{
            Delaunay *delaunay = storeService->getDelaunay();
            DelaunayIO::write(Config::getOutDCELFilename(), Config::getOutGraphFilename(), *delaunay);
			break;
		}
		// Write voronoi DCEL file.
		case WRITE_VORONOI:
		{
			VoronoiIO::write(Config::getOutVoronoiFilename(), *storeService->getVoronoi());
			break;
		}
		// Write Gabriel graph data.
		case WRITE_GABRIEL:
		{
			GabrielIO::writeBinary(Config::getOutGabrielFilename(), *storeService->getGabriel());
			break;
		}
		// Clear data.
		case CLEAR:
		{
			// Update execution status flags.
            Status *status = storeService->getStatus();
			status->reset();

			// Update menu entries.
			this->m.updateMenu();

			// Clear screen.
            dispManager->process();

			// Reset data.
			break;
		}
		// Quit application.
		case QUIT:
		{
			// Quit application.
			quit = true;
			break;
		}
		// Not implemented functionality.
		case CHECK_DCEL:
		case ZOOM:
		{
			std::cout << "Not implemented." << std::endl;
			break;
		}
		default:
		{
			if (firstTime)
			{
				// Clear screen.
				firstTime = false;
                dispManager->process();
			}
			break;
		}
	}

	// Delete iteration resources
    delete cmd;

	// Exit application.
	if (quit)
	{
		// PENDING. Anything to deallocate.
		delete this->log;
		exit(0);
	}
	else
	{
		// Reset menu option.
		m.resetMenuOption();
	}
}
