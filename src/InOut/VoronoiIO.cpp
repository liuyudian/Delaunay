/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "DcelWriter.h"
#include "Voronoi.h"
#include "VoronoiIO.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void VoronoiIO::print(const Voronoi &voronoi, std::ostream &out)
{
    DcelWriter::print(voronoi.dcel, out);
}


bool VoronoiIO::read(const string &strFileName, Voronoi &voronoi)
{
    // Read voronoi DCEL file.
    bool isSuccess = DcelReader::read(strFileName, false, voronoi.dcel);
    voronoi.setValid(isSuccess);

    return isSuccess;
}


bool VoronoiIO::write(const string &strFileName, const Voronoi &voronoi)
{
    bool isSuccess = DcelWriter::write(strFileName, voronoi.dcel, false);

    return isSuccess;
}