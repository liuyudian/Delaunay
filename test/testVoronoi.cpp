/*
 * testVoronoi.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: juan
 */

#include "Delaunay.h"
#include "Voronoi.h"
#include "testVoronoi.h"
#include <stdio.h>
#include <unistd.h>

//#define DEBUG_TEST_VORONOI_BUILD_INIT
//#define DEBUG_TEST_VORONOI_BUILD
//#define DEBUG_TEST_VORONOI_COMPARE

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestVoronoiBuild::initParameters()
{
	// Add # points parameter.
	NumericValidator *nPointsDefinition = new NumericValidator(3, INT_MAX);
	ParameterInt *nPointsParameter = new ParameterInt(TYPE_N_POINTS_LABEL, nPointsDefinition);
	this->parameters.add(nPointsParameter);

	// Add # tests parameter.
	NumericValidator *nTestsDefinition = new NumericValidator(1, INT_MAX);
	ParameterInt *nTestsParameter = new ParameterInt(TYPE_N_TESTS_LABEL, nTestsDefinition);
	this->parameters.add(nTestsParameter);

	// Add delta # points parameter.
	NumericValidator *deltaPointsDefinition = new NumericValidator(1, 100);
	ParameterInt *deltaPointsParameter = new ParameterInt(TYPE_DELTA_STEP_LABEL, deltaPointsDefinition);
	this->parameters.add(deltaPointsParameter);

	// Add # steps parameter.
	NumericValidator *stepsDefinition = new NumericValidator(0, INT_MAX);
	ParameterInt *stepsParameter = new ParameterInt(TYPE_N_STEPS_LABEL, stepsDefinition);
	this->parameters.add(stepsParameter);
#ifdef DEBUG_TEST_VORONOI_BUILD_INIT
	// Print parameters data.
	this->printParameters();
#endif
}

/***************************************************************************
* Name: 	printParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints the test parameters.
***************************************************************************/
void TestVoronoiBuild::printParameters()
{
	cout << "---------------------------------------------------" << endl;
	cout << "Test Voronoi printParameters" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Number of points " << this->nPoints << endl;
	cout << "Delta points " << this->deltaPoints << endl;
	cout << "Number of steps " << this->nSteps << endl;
	cout << "Number of test  " << this->nTests << endl;
	cout << "Output folder " << this->outputFolder << endl;
	cout << "---------------------------------------------------" << endl;
}

/***************************************************************************
* Name: 	applyParameter
* IN:		parameter		parameter to update
* 			value			value to set
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	set a class attribute to "value"
***************************************************************************/
void TestVoronoiBuild::applyParameter(Parameter *parameter, string value)
{
	string parameterName;
	int convertedValue=0;

	// Get parameter label.
	parameterName = parameter->getName();
	if (parameterName == TYPE_N_POINTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;
		this->setPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_TESTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setTests(convertedValue);
	}
	else if (parameterName == TYPE_DELTA_STEP_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setDeltaPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_STEPS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setSteps(convertedValue);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter:");
		Logging::buildText(__FUNCTION__, __FILE__, parameterName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	builds several Delaunay triangulation using different set of
* 				points and if any fails the dcel data is written to a file.
***************************************************************************/
void TestVoronoiBuild::main()
{
	Dcel		dcel;				// Dcel data.
	Delaunay	delaunay;			// Delaunay data.
	Voronoi		voronoi;			// Voronoi data.
	int			failedTestIndex=1;	// Index of last failed test.
	int			testIndex=0;		// Current test index.
	int			stepIndex=0;		// Current step index.
	string 		fileName;
	int			currentNPoints=0;

	// Print test parameters.
	this->printParameters();

	// Execute tests.
	currentNPoints = this->nPoints;
	for (stepIndex=0; stepIndex<this->nSteps ;stepIndex++)
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Executing step ");
		Logging::buildText(__FUNCTION__, __FILE__, (stepIndex+1));
		Logging::buildText(__FUNCTION__, __FILE__, "/");
		Logging::buildText(__FUNCTION__, __FILE__, this->nSteps);
		Logging::write(true, Info);

		for (testIndex=0; testIndex< this->nTests ;testIndex++)
		{
			// Execute current test.
			delaunay.setDCEL(&dcel);
			if (!dcel.generateRandom(currentNPoints))
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error generating data set in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else if (!delaunay.incremental())
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Error building Delaunay triangulation in iteration ");
				Logging::buildText(__FUNCTION__, __FILE__, (testIndex+1));
				Logging::write(true, Error);
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Start building voronoi diagram test ");
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, "/");
				Logging::buildText(__FUNCTION__, __FILE__, this->nTests);
				Logging::write(true, Info);
				Logging::buildText(__FUNCTION__, __FILE__, "Current number of points ");
				Logging::buildText(__FUNCTION__, __FILE__, currentNPoints);
				Logging::write(true, Info);

				// Write test data.
				ostringstream convert;
				convert << failedTestIndex;
				fileName = this->outputFolder + "Voronoi_" + convert.str() + ".txt";
				this->dump(fileName, dcel);
				if (!voronoi.init(&dcel))
				{
					// Compute Voronoi diagram.
					if (!voronoi.build())
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Voronoi diagram built for test ");
						remove(fileName.c_str());
					}
					else
					{
						failedTestIndex++;
						Logging::buildText(__FUNCTION__, __FILE__, "Error building voronoi diagram in test ");
					}
				}
				else
				{
					failedTestIndex++;
					Logging::buildText(__FUNCTION__, __FILE__, "Error initializing voronoi in test ");
				}
				Logging::buildText(__FUNCTION__, __FILE__, testIndex+1);
				Logging::write(true, Info);
				sleep(1);

				// Reset voronoi data.
				voronoi.reset();
			}
		}

		// Update # points to generate.
		currentNPoints = currentNPoints*this->deltaPoints;
	}
}

/***************************************************************************
* Name: 	dump
* IN:		dcelFileName	dcelFilefile where dcel points are written
* 			dcel			dcel data to write
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Writes to output files the data required to reproduce the fail
***************************************************************************/
void TestVoronoiBuild::dump(string dcelFileName, Dcel &dcel)
{
	ofstream ofs;			// Output file.

	// Open file.
	ofs.open(dcelFileName.c_str(), ios::out);

	// Check file is opened.
	if (ofs.is_open())
	{
		// Write DCEL data.
		if (!dcel.writePoints(dcelFileName, dcel.getNVertex()))
		{
			cout << "Cannot open file " << dcelFileName << " to write dcel data" << endl;
		}
	}
	// Error opening points file.
	else
	{
		cout << "Cannot open file " << dcelFileName << " to write points" << endl;
	}
}

/***************************************************************************
* Name: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	free test resources. The data allocated are the parameters
* 				and validators allocated in initParameters
***************************************************************************/
void TestVoronoiBuild::deallocate()
{
	// PENDING https://github.com/juannavascalvente/Delaunay/issues/2
	/*int	i=0;					// Loop counter.
	Parameter *param;
	NumericValidator *validator;
	for (i=0; i<this->parameters.getNElements() ;i++)
	{
		param = *this->parameters.at(i);
	}
	*/
}

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestVoronoiCompare::initParameters()
{
	// Add files list parameter.
	FileValidator *filesValidator = new FileValidator();
	ParameterFile *fileParameter = new ParameterFile(TEST_FILE_LABEL, filesValidator);
	this->parameters.add(fileParameter);
#ifdef DEBUG_TEST_VORONOI_COMPARE
	// Print parameters data.
	this->printParameters();
#endif
}

/***************************************************************************
* Name: 	printParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints the test parameters.
***************************************************************************/
void TestVoronoiCompare::printParameters()
{
	cout << "Test Voronoi printParameters" << endl;
	cout << "Files list: " << this->filesNamesFile << endl;
}

/***************************************************************************
* Name: 	applyParameter
* IN:		parameter		parameter to update
* 			value			value to set
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	set a class attribute to "value"
***************************************************************************/
void TestVoronoiCompare::applyParameter(Parameter *parameter, string value)
{
	string parameterName;

	// Get parameter label.
	parameterName = parameter->getName();
	if (parameterName == TEST_FILE_LABEL)
	{
		// Copy file name list.
		this->setFileNamesFile(value);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter:");
		Logging::buildText(__FUNCTION__, __FILE__, parameterName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	prepare
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	executes any previous checks or actions previous to the
* 				tests execution
***************************************************************************/
bool TestVoronoiCompare::prepare()
{
	bool success=true;
	string 	line;
	ifstream ifs;
	ifstream fileTestIfs;

	// Open configuration file.
	ifs.open(this->filesNamesFile.c_str(), ios::in);

	// Check file is opened.
	if (!ifs.is_open())
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
		Logging::buildText(__FUNCTION__, __FILE__, this->filesNamesFile.c_str());
		Logging::write(true, Error);
		success = false;
	}
	else
	{
		// Check all files exist.
		while (getline(ifs, line))
		{
			// Open file.
			fileTestIfs.open(line.c_str(), ios::in);

			// Check file exists.
			if (fileTestIfs.is_open())
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Adding file to list: ");
				Logging::buildText(__FUNCTION__, __FILE__, line);
				Logging::write(true, Info);

				this->filesList.add(line);
				fileTestIfs.close();
			}
			else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Skip file that does not exist: ");
				Logging::buildText(__FUNCTION__, __FILE__, line);
				Logging::write(true, Error);
			}
		}

		// Close file list.
		ifs.close();
	}

	return(success);
}

/***************************************************************************
* Name: 	main
* IN:		NONE
* OUT:		NONE
* RETURN:	true			if test prepared
* 			false			i.o.c.
* GLOBAL:	NONE
* Description: 	compares the Voronoi read from input files with the voronoi
* 				computed using the points in the input files
***************************************************************************/
void TestVoronoiCompare::main()
{
	int i=0;		// Loop counter.
	ifstream ifs;	// File stream.
	string fileName;

	// Print test parameters.
	this->printParameters();

	Logging::buildText(__FUNCTION__, __FILE__, "Number of files to compare: ");
	Logging::buildText(__FUNCTION__, __FILE__, this->filesList.getNElements());
	Logging::write(true, Info);

	// Read all files.
	for (i=0; i<this->filesList.getNElements() ;i++)
	{
		// Open file.
		fileName = *this->filesList.at(i);
		ifs.open(fileName.c_str(), ios::in);

		// Check file is opened.
		if (!ifs.is_open())
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
			Logging::buildText(__FUNCTION__, __FILE__, fileName);
			Logging::buildText(__FUNCTION__, __FILE__, " in iteration ");
			Logging::buildText(__FUNCTION__, __FILE__, i);
			Logging::write(true, Error);
		}
		else
		{
			Logging::buildText(__FUNCTION__, __FILE__, "Comparing with Voronoi from ");
			Logging::buildText(__FUNCTION__, __FILE__, fileName);
			Logging::write(true, Info);

			// Close file list.
			ifs.close();
		}
	}
}

/***************************************************************************
* Name: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	free test resources. The data allocated are the parameters
* 				and validators allocated in initParameters
***************************************************************************/
void TestVoronoiCompare::deallocate()
{
	cout << "Dealllocating TestVoronoiCompare" << endl;
	// PENDING https://github.com/juannavascalvente/Delaunay/issues/2
}


