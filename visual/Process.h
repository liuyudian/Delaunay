/*
 * Process.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef PROCESS_H_
#define PROCESS_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayManager.h"

#include "Menu.h"
#include "Status.h"
#include "Config.h"
#include "Dcel.h"
#include "Delaunay.h"
#include "Gabriel.h"
#include "Logging.h"
#include "Queue.h"
#include "StarTriangulation.h"
#include "StoreService.h"
#include "Text.h"
#include "Voronoi.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Process
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    DisplayManager      *dispManager;
    StoreService        *storeService;

	Menu 			    m;				// Menu object.
	Logging 		    *log;			// Log file.

    /*******************************************************************************************************************
     * Private methods declaration
     *******************************************************************************************************************/
	void execute();
	static void executeWrapper();
	bool readData(int option);

protected:
	static Process *instance;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Process(int argc, char **argv, bool printData, StoreService *storeServiceIn);
	~Process();

    /**
     * @fn      start
     * @brief   starts infinite loop
     */
    static void start();

    /**
     * @fn      setInstance
     * @brief   Sets "instance" to the object that is going to be executed
     *          by the main loop process
     * @param   process   instance to be executed by main loop
     */
	static void setInstance(Process *process);
};

#endif /* PROCESS_H_ */
