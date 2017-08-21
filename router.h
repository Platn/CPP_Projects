// Authors: Kevin Le, Calvin Huang
#include <iostream>
#ifndef ROUTER_H
  #define ROUTER_H

#include "RouterRunner.h"

using namespace std;

struct AdjNode
{
	int cost;
	/* Edge's start will be the start of the next one, 
	** its end is the next connection, it does not need to 
	** store the middle
	*/
	AdjNode *previous;
	AdjNode *next;
	Edge connect; 
	AdjNode() : cost(21470000), previous(NULL){}
};

struct BinaryHeap
{
	int currentSize;
	int tableSize;
	AdjNode *nodes;
	
	BinaryHeap() : currentSize(0)
	{
		nodes = new AdjNode[1000000];
	}
	
	AdjNode & findMin()
	{
		return nodes[1];
	}
	
	bool isEmpty()
	{
		return currentSize == 0;
	}
	
	void deleteMin()
	{
		nodes[1] = nodes[currentSize--];
		percolateDown(1);
	}
	
	void insert(AdjNode & passedNode)
	{
		int hole = ++currentSize;
		for(; hole > 1 && passedNode.cost <= nodes[hole / 2].cost; hole /= 2)
		{
			if(passedNode.cost < nodes[hole / 2].cost)
			{
				nodes[hole] = nodes[hole / 2];
			}
		}
		
		nodes[hole] = passedNode;
	}
	
	void percolateDown(int hole)
	{
		int child;
		AdjNode newNode = nodes[hole];
		
		for(; hole * 2 <= currentSize; hole = child)
		{
			child = hole * 2;
          if( child != currentSize && nodes[ child + 1 ].cost > nodes[ child ].cost )
          {
             child++;
             if( nodes[child].cost <= newNode.cost )
             {
				nodes[ hole ] = nodes[ child ];
			 }
			 
			 
			 
             else
             {
				break;
			 }
		  }
		}	
		
		nodes[hole] = newNode;
		
	}
	
	void printList()
	{
		cout << "\nBinary Heap Print List: " << endl;
		for(int i = 1; i <= currentSize; i++)
		{
			cout << nodes[i].cost << ' ';
		}
		
		cout << endl;
	}
};

struct checkCity
{
	int size;
	CityPos * listOfCities;
	checkCity(int size_) : size(size_)
	{
		listOfCities = new CityPos[size];
	}
	
	bool isEmpty()
	{
		if(size == 0)
		{
			return true;
		}
		
		return false;
	}
	
	void searchAndDestroy(short x, short y)
	{
		if(isEmpty())
		{
			return;
		}
		for(int i = 0; i < size; i++)
		{
			if (listOfCities[i].x == x && listOfCities[i].y == y)
			{
				listOfCities[i] = listOfCities[size - 1];
				
				size--;
				break;
			}
		}
	}
	
	void printList()
	{
		cout << "\nCheck Print List: " << endl;
		if(!isEmpty())
		{
			for(int i = 0; i < size; i++)
			{
				cout << listOfCities[i].x << ' ' << listOfCities[i].y << endl;
			}
		}
	}
};

class Router
{
public:
  int vertexCount;
  Map1000 *cityMap; 
  char **visitedEdge; 
  AdjNode * source;
  BinaryHeap heap;
  
  int width;
  
  Router(const Map1000 *map, int width);
  ~Router();
  void findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount);
  void printMap()
  {
	cout << "\nPrint Map: " << endl;
	for(int i = width - 1; i >= 0; i--)
	{
		for(int j = 0; j < width; j++)
		{
			cout << i << " ";
			cout << cityMap->map1000[j][i] << ' ';
		}
		
		cout << endl;
	}
	
	cout << endl;
  }
  
  void printTFMap()
  {
	  cout << "\nPrint TF Map: " << endl;
	for(int i = width - 1; i >= 0; i--)
	{
		for(int j = 0; j < width; j++)
		{
				cout << visitedEdge[j][i] << ' ';
		}
		
		cout << endl;
	}
	
	cout << endl;
  }
  
  void Dijkstras(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount)
  {
	  
	  checkCity check(cityCount);
	  for(int i = 0; i < cityCount; i++)
	  {
		  check.listOfCities[i] = cityPos[i];
	  }
	  
	  
	  short x = cityPos[0].x;
	  source->connect.startX = x; 
	  short y = cityPos[0].y;
	  
	  source->connect.startY = y;
	  visitedEdge[x][y] = 'T';
	  check.searchAndDestroy(x, y);
	  
	  AdjNode * minValue = NULL;
	  AdjNode newNode = addHeapEdges(source);
	  minValue = &newNode;
	  source->connect.endX = minValue->connect.startX;
	  source->connect.endY = minValue->connect.startY;
	  source->next = minValue;
	  visitedEdge[minValue->connect.startX][minValue->connect.startY] = 'T';
	  check.searchAndDestroy(minValue->connect.startX, minValue->connect.startY);

	  
	  heap.deleteMin();

	  int count = 0;
	  while(true)
	  {
		  newNode = addHeapEdges(minValue);
		  
		  minValue->connect.endX = newNode.connect.startX;
		  minValue->connect.endY = newNode.connect.startY;
		  minValue->next = &newNode;
		  
		  
		  visitedEdge[minValue->connect.startX][minValue->connect.startY] = 'T';
		  check.searchAndDestroy(minValue->connect.startX, minValue->connect.startY);
		  
		  minValue = NULL;
		  minValue = &newNode;
	
		  
		  if(check.isEmpty())
		  {
			  break; 
		  }
		  
		  if(!heap.isEmpty())
		  {
			  heap.deleteMin();
		  }
		  count++; // Take me out!
	  }
  }
  
  AdjNode addHeapEdges(AdjNode * passedNode)
  {
	  AdjNode tempNode;
	  for(int i = -1; i <= 1; i++)
	  {
		  for(int j = -1; j <= 1; j++)
		  {
			  
			  tempNode.previous = passedNode;
		      tempNode.connect.startX = passedNode->connect.startX + i;
			  tempNode.connect.startY = passedNode->connect.startY + j;
			  if(checkBounds(passedNode->connect.startX + i) && checkBounds( passedNode->connect.startY + j) 
			  && !(usedPos(passedNode->connect.startX + i, passedNode->connect.startY + j)))
			  {
				  
				  int weightedDiff = cityMap->map1000[passedNode->connect.startX][passedNode->connect.startY]
				  - cityMap->map1000[tempNode.connect.startX][tempNode.connect.startY];
				  tempNode.cost = (weightedDiff * weightedDiff) + 10;
				  tempNode.cost = passedNode->cost + tempNode.cost;
				  if(visitedEdge[tempNode.connect.startX][tempNode.connect.startY] == 'T')
				  {
					  continue;
				  }
				  visitedEdge[tempNode.connect.startX][tempNode.connect.startY] = 'H';
				  heap.insert(tempNode);
			  }
		  }
		  
	  }
	  //~ heap.printList();
	  return heap.findMin();
	  // Return the end node with the minimum value
  }
  /* Pass in the value of x and y with +/-*/
  
  
  
  int calcEdges(short currentCityWeight, short x, short y, int cost) 
  {  
	 int checkValue = (cityMap->map1000[x][y] - currentCityWeight);
	 checkValue = checkValue * checkValue + 10; 
	 return checkValue; // Place holder
  }
  
  bool checkBounds(short valueX)
  {
	  if(valueX < 0 || !(valueX < width))
	  {
		  return false;
	  }
	  
	  return true;
  }
  
  bool usedPos(short x, short y)
  {
	  if(visitedEdge[x][y] != 'F')
	  {
		  return true;
	  }
	  
	  return false;
  }
};


#endif

/* 
 * Notes:
 * Map is an array of the entire map as a 1000 x 1000
 * CityPos is used to store the position of the cities (City Position remains constant)
 * Edges is used to store the start and end of the vertex
 * 
 * 
 * Things Done: 
 * Created a city map of 1000 x 1000, cities have not been recieved yet
 * 
*/
