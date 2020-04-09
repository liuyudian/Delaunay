#ifndef DELAUNAY_GRAPHREADER_H
#define DELAUNAY_GRAPHREADER_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Graph.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GraphReader
{

    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
    static bool readFlat(const string &strFileName, Graph &graph);
    static bool readBinary(const string &fileName, Graph &graph) { return false; };

public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn          read
     * @brief       reads the graph data from a file in plain or binary format depending on "isBinary" format.
     *
     * @param   fileName  (IN) File name
     * @param   isBinary  (IN) Binary format flag
     * @param   graph     (OUT) Graph to update
     * @return  true if read successfully
     *          false otherwise
     */
    static bool read(const string &fileName, bool isBinary, Graph &graph);
};

#endif //DELAUNAY_GRAPHREADER_H