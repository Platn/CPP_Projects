// Authors: Kevin Le, Calvin Huang
#include "router.h"
#include <iostream>
using namespace std;

/* Router Constructor will take in the map as a short and take in the int width
 Set the width of myWidth to the width that is passed in
 vertexCount = the width multiplied by the width again
 
 Output the initialization of the array, set the map to a new short array pointer of size width
 Output the subarray initialization
 Loop through the row while the rowCount is less than the width. Increment
 Output the Row being initialized
 Set the map by row equal to a new 
 Inside do the same for the columns
 
 Print the map once its done
*/

Router::Router(const Map1000 *map, int width)
{
	cityMap = new Map1000(); // This can be alternated into a double short array
	
	source = new AdjNode();
	
	
	vertexCount = width * width; // Number of vertices in the graph
	visitedEdge = new char*[width];
	this->width = width;
	
	for(int i = 0; i < width; ++i)
	{
		visitedEdge[i] = new char[width];
	}
	
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < width; j++)
		{
			visitedEdge[i][j] = 'F';
		}
	}
	
	for(int i = width - 1; i >= 0; i--)
	{
		for(int j = 0; j < width; j++)
		{
			cityMap->map1000[j][i] = map->map1000[j][i];
		}
	}
	
} // Router()

Router::~Router()
{
}  // ~Router()

void Router::findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount)
{
	source->cost = 0;
	Dijkstras(cityPos, cityCount, paths, pathCount);
	//~ while(source->next != NULL)
	//~ {
		//~ source = source->next;
	//~ }
	
	//~ cout << source->cost;
}


