#ifndef DELAUNAY_TESTDATA_H
#define DELAUNAY_TESTDATA_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <iostream>
#include <map>
#include <string>
using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define TEST_NAME_FIELD                     "Name"

#define TEST_POINTS_IN_FIELD                "PointsIn"
//#define TEST_DELAUNAY_IN_FIELD              "DelaunayIn"
//#define TEST_DELAUNAY_GRAPH_IN_FIELD        "DelaunayGraphIn"

#define TEST_DELAUNAY_OUT_FIELD             "DelaunayOut"
#define TEST_DELAUNAY_GRAPH_OUT_FIELD       "DelaunayGraphOut"
#define TEST_VORONOI_OUT_FIELD              "VoronoiOut"


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
class TestData
{
    map<string, string> hash;

public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
    TestData() = default;

    /**
     *
     * @param key
     * @param strValue
     */
    void add(string &key, string &strValue) { hash.insert(std::pair<string,string>(key, strValue)); }

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
    bool getValue(const string &key, string &strOut)
    {
        bool isSuccess=false;
        try
        {
            strOut = hash.at(key);
            isSuccess = true;
        }
        catch (const std::out_of_range& oor)
        {
            std::cout << "Key " << key << " does not exists. Out of Range error: " << oor.what() << endl;
        }

        return isSuccess;
    }
};


#endif //DELAUNAY_TESTDATA_H
