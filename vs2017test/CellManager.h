#pragma once
#include "CompareCells.h"
#include "Cell.h"
#include <queue>
#include <vector>


using namespace std;

class CellManager
{
private:
	Cell* cell;
	priority_queue<Cell*, vector <Cell*>, CompareCellsBestFirst > neighborsQueue;


public:
	CellManager(Cell* _cell);
	Cell* GetClosestNeighbor();
	void AddNeighbor(Cell* neighbor);
	int NumOfNeighbors();
	Cell* GetCell();
	int GetRow();
	int GetCol();
	Cell* GetParent();
	void SetG(int _g);
	int GetG();


};

