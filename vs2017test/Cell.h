#pragma once

#include <math.h>


class Cell
{
private:
	int row, col, g;
	Cell* parent;
public:
	Cell(int r, int c, Cell* pc);
	int GetRow();
	int GetCol();
	Cell* GetParent();
	int GetG();
	void SetG(int _g);
	void SetParent(Cell* parent);
};

