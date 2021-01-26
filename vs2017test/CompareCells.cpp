#include "CompareCells.h"
extern Cell* targetCell;

extern int targetRow, targetCol;

bool CompareCellsBestFirst::operator()(Cell* pc1, Cell* pc2)
{
	{
		int horDisCell1 = abs(targetCol - pc1->GetCol());
		int vertDisCell1 = abs(targetRow - pc1->GetRow());
		int horDisCell2 = abs(targetCol - pc2->GetCol());
		int vertDisCell2 = abs(targetRow - pc2->GetRow());
		return horDisCell1 + vertDisCell1 > horDisCell2 + vertDisCell2;

	}
}
bool CompareCellsAStar::compareCells(Cell* pc1, Cell* pc2)
{
	int horDisCell1 = abs(targetCol - pc1->GetCol());
	int vertDisCell1 = abs(targetRow - pc1->GetRow());
	int horDisCell2 = abs(targetCol - pc2->GetCol());
	int vertDisCell2 = abs(targetRow - pc2->GetRow());
	return horDisCell1 + vertDisCell1 + pc1->GetG() > horDisCell2 + vertDisCell2 + pc2->GetG();
}
bool CompareCellsAStar::operator()(Cell* pc1, Cell* pc2)
{
	{
		return compareCells(pc1, pc2);

	}
}

