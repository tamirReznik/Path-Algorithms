#include "CellManager.h"



CellManager::CellManager(Cell* _cell)
{
	cell = _cell;
}

Cell* CellManager::GetClosestNeighbor()
{
	Cell* neighbor = neighborsQueue.top();
	neighborsQueue.pop();
	return neighbor;
}

void CellManager::AddNeighbor(Cell* neighbor)
{
	neighborsQueue.push(neighbor);
}

int CellManager::NumOfNeighbors()
{
	return neighborsQueue.size();
}

Cell* CellManager::GetCell()
{
	return cell;
}
int CellManager::GetRow()
{
	return cell->GetRow();
}

int CellManager::GetCol()
{
	return cell->GetCol();
}

Cell* CellManager::GetParent()
{
	return cell->GetParent();
}

int CellManager::GetG()
{
	return cell->GetG();
}

void CellManager::SetG(int _g)
{
	cell->SetG(_g);
}