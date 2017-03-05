/*
 * Menu.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include "Menu.h"

/**************************************************************************
* 							Defines section
**************************************************************************/
// Input points set menu entries text.
#define	INPUTSET_MENU_TEXT				"Data set"
#define	INPUTSET_RANDOM_TEXT			"Generate randomly"
#define	INPUTSET_CLUSTER_TEXT			"Generate clusters"
#define	INPUTSET_READ_FLAT_TEXT			"Read flat file"
#define	INPUTSET_READ_DCEL_TEXT			"Read DCEL file"
#define	INPUTSET_READ_DELAUNAY_TEXT		"Read Delaunay files"
#define	INPUTSET_READ_VORONOI_TEXT		"Read Voronoi file"
#define INPUTSET_READ_GABRIEL_TEXT		"Read Gabriel file"
#define	INPUTSET_SHAKE_DCEL_TEXT		"Shake points"

// Parameters menu entries text.
#define PARAMETERS_MENU_TEXT			"Read parameters"

// Triangulation menu entries text.
#define	TRIANGULATIONS_MENU_TEXT		"Triangulations"
#define	TRIANGULATIONS_DELAUNAY_TEXT	"Delaunay"
#define	TRIANGULATIONS_STAR_TEXT		"Star triangulation"

// Figures menu entries.
#define FIGURES_MENU_TEXT				"Figures"
#define FIGURES_CONVEX_TEXT				"Convex hull"
#define FIGURES_CIRCUMCENTRES_TEXT		"Circumcentres"
#define FIGURES_EDGES_CRICLES_TEXT		"Edges circles"
#define FIGURES_VORONOI_TEXT			"Voronoi"
#define FIGURES_GABRIEL_TEXT			"Gabriel"
#define FIGURES_FILTER_EDGES			"Filter edges"
#define FIGURES_VORONOI_PATH_TEXT		"Voronoi path"
#define	FIGURES_TRIANGULATION_PATH_TEXT	"Triangulation path"

// Location menu entries.
#define LOCATION_MENU_TEXT					"Location"
#define	LOCATION_CLOSEST_POINTS_TEXT		"Closest to a given"
#define	LOCATION_2CLOSEST_POINTS_TEXT		"Two closest"
#define	LOCATION_FIND_FACE_TEXT				"Locate face"

// Export menu entries text.
#define	EXPORT_MENU_TEXT				"Export"
#define EXPORT_FLAT_FILE				"Write flat file"
#define EXPORT_DCEL_FILE				"Write DCEL file"
#define EXPORT_DELAUNAY_FILE			"Write Delaunay files"
#define EXPORT_VORONOI_FILE				"Write Voronoi file"
#define EXPORT_GABRIEL_FILE				"Write Gabriel file"

// Export menu entries text.
#define	INFORMATION_MENU_TEXT			"Information"
#define INFORMATION_DCEL_TEXT			"Dcel info"
#define INFORMATION_VORONOI_TEXT		"Voronoi info"

// Other menu entries text.
#define	ZOOM_MENU_TEXT					"Zoom"
#define	CLEAR_MENU_TEXT					"Clear"
#define	QUIT_MENU_TEXT					"Quit"

#define INITIAL_MENU        0
#define SET_MENU            1
#define TRIANGULATION_MENU	2
#define VORONOI_MENU        3
#define GABRIEL_MENU        4


//------------------------------------------------------------------------
// Functions.
//------------------------------------------------------------------------
void menu_level_1(int menu_Id);

int menu_Option;

//------------------------------------------------------------------------
// Constructors.
//------------------------------------------------------------------------
Menu::Menu()
{
	// Initialize class attributes.
	this->menu_Id = 0;
	this->sub_Menu_Input = 0;
	this->sub_Menu_Id2 = 0;
	this->sub_Menu_Id3 = 0;
	this->sub_Menu_Id4 = 0;
	this->sub_Menu_Id5 = 0;
	this->status = 0;
	menu_Option = DEFAULT_OPTION;
};


Menu::Menu(Status *status)
{
	// Initialize class attributes.
	this->menu_Id = 0;
	this->sub_Menu_Input = 0;
	this->sub_Menu_Id2 = 0;
	this->sub_Menu_Id3 = 0;
	this->sub_Menu_Id4 = 0;
	this->sub_Menu_Id5 = 0;
	this->status = status;

	updateMenu();

	menu_Option = DEFAULT_OPTION;
};

//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
void Menu::updateMenu()
{
	// Check if first time menu created.
	if (this->status->isFirstTime())
	{
		// Clear current menu.
		if (menu_Id != 0)
		{
			glutDestroyMenu(menu_Id);
		}

		// Input points sub-menu.
		sub_Menu_Input = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_RANDOM_TEXT, RANDOMLY);
		glutAddMenuEntry(INPUTSET_CLUSTER_TEXT, CLUSTER);
		glutAddMenuEntry(INPUTSET_READ_FLAT_TEXT, READ_POINTS_FILE);
		glutAddMenuEntry(INPUTSET_READ_DCEL_TEXT, READ_DCEL);
		glutAddMenuEntry(INPUTSET_READ_DELAUNAY_TEXT, READ_DELAUNAY);
		glutAddMenuEntry(INPUTSET_READ_VORONOI_TEXT, READ_VORONOI);
		glutAddMenuEntry(INPUTSET_READ_GABRIEL_TEXT, READ_GABRIEL);

		// Main menu.
		menu_Id = glutCreateMenu(menu_level_1);
		glutAddSubMenu(INPUTSET_MENU_TEXT, sub_Menu_Input);
		glutAddMenuEntry(PARAMETERS_MENU_TEXT, PARAMETERS);
		glutAddMenuEntry(QUIT_MENU_TEXT, QUIT);
		glutAttachMenu(GLUT_RIGHT_BUTTON);

		this->status->setFirstTime(false);
	}
	else
	{
		// Clear current menu.
		glutDestroyMenu(menu_Id);

		// Input points submenu.
		sub_Menu_Input = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_RANDOM_TEXT, RANDOMLY);
		glutAddMenuEntry(INPUTSET_CLUSTER_TEXT, CLUSTER);
		glutAddMenuEntry(INPUTSET_READ_FLAT_TEXT, READ_POINTS_FILE);
		glutAddMenuEntry(INPUTSET_READ_DCEL_TEXT, READ_DCEL);
		glutAddMenuEntry(INPUTSET_READ_DELAUNAY_TEXT, READ_DELAUNAY);
		glutAddMenuEntry(INPUTSET_READ_VORONOI_TEXT, READ_VORONOI);
		glutAddMenuEntry(INPUTSET_READ_GABRIEL_TEXT, READ_GABRIEL);
		glutAddMenuEntry(INPUTSET_SHAKE_DCEL_TEXT, SHAKE_POINTS);
		// PENDING glutAddMenuEntry("Check data set", CHECK_DCEL);

		// Triangulations submenu.
		if (this->status->isSetCreated())
		{
			sub_Menu_Id2 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(TRIANGULATIONS_STAR_TEXT, STAR_TRIANGULATION);
			glutAddMenuEntry(TRIANGULATIONS_DELAUNAY_TEXT, DELAUNAY);
		}

		// Main figures submenu.
		if (this->status->isTriangulationCreated() || this->status->isDelaunayCreated())
		{
			sub_Menu_Id3 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(FIGURES_CONVEX_TEXT, CONVEX_HULL);
			glutAddMenuEntry(FIGURES_CIRCUMCENTRES_TEXT, CIRCUMCENTRES);
			glutAddMenuEntry(FIGURES_EDGES_CRICLES_TEXT, EDGE_CIRCLES);
			glutAddMenuEntry(FIGURES_VORONOI_TEXT, VORONOI);
			glutAddMenuEntry(LOCATION_CLOSEST_POINTS_TEXT, CLOSEST_POINT);
			glutAddMenuEntry(LOCATION_2CLOSEST_POINTS_TEXT, TWO_CLOSEST);
			glutAddMenuEntry(LOCATION_FIND_FACE_TEXT, FIND_FACE);
			glutAddMenuEntry(FIGURES_TRIANGULATION_PATH_TEXT, TRIANGULATION_PATH);
			glutAddMenuEntry(FIGURES_FILTER_EDGES, FILTER_EDGES);

			if (this->status->isVoronoiCreated())
			{
				glutAddMenuEntry(FIGURES_GABRIEL_TEXT, GABRIEL);
				glutAddMenuEntry(FIGURES_VORONOI_PATH_TEXT, VORONOI_PATH);
			}
		}

		// Output data submenu.
		if (this->status->isSetCreated())
		{
			sub_Menu_Id4 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(EXPORT_FLAT_FILE, WRITE_POINTS);
			if (this->status->isTriangulationCreated())
			{
				glutAddMenuEntry(EXPORT_DCEL_FILE, WRITE_DCEL);
				if (this->status->isDelaunayCreated())
				{
					glutAddMenuEntry(EXPORT_DELAUNAY_FILE, WRITE_DELAUNAY);
				}
				if (this->status->isVoronoiCreated())
				{
					glutAddMenuEntry(EXPORT_VORONOI_FILE, WRITE_VORONOI);
				}
				if (this->status->isGabrielCreated())
				{
					glutAddMenuEntry(EXPORT_GABRIEL_FILE, WRITE_GABRIEL);
				}
			}
		}

		// Information submenu.
		if (this->status->isSetCreated())
		{
			sub_Menu_Id5 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(INFORMATION_DCEL_TEXT, DCEL_INFO);
			if (this->status->isVoronoiCreated())
			{
				glutAddMenuEntry(INFORMATION_VORONOI_TEXT, VORONOI_INFO);
			}
		}

		// Main menu.
		menu_Id = glutCreateMenu(menu_level_1);
		glutAddSubMenu(INPUTSET_MENU_TEXT, sub_Menu_Input);
		glutAddMenuEntry(PARAMETERS_MENU_TEXT, PARAMETERS);
		if (this->status->isSetCreated())
		{
			glutAddSubMenu(TRIANGULATIONS_MENU_TEXT, sub_Menu_Id2);
		}
		if (this->status->isTriangulationCreated() || this->status->isDelaunayCreated())
		{
			glutAddSubMenu(FIGURES_MENU_TEXT, sub_Menu_Id3);
		}
		if (this->status->isSetCreated())
		{
			glutAddSubMenu(EXPORT_MENU_TEXT, sub_Menu_Id4);
		}
		if (this->status->isSetCreated())
		{
			glutAddSubMenu(INFORMATION_MENU_TEXT, sub_Menu_Id5);
		}
		glutAddMenuEntry(CLEAR_MENU_TEXT, CLEAR);
		glutAddMenuEntry(ZOOM_MENU_TEXT, ZOOM);
		glutAddMenuEntry(QUIT_MENU_TEXT, QUIT);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

int	Menu::getMenuOption()
{
	return(menu_Option);
}

void Menu::setMenuOption(int value)
{
	menu_Option = value;
}

void Menu::resetMenuOption()
{
	menu_Option = DEFAULT_OPTION;
}

void menu_level_1(int menu_Id)
{
	menu_Option = menu_Id;
}
