/*
**************************
** Made By Tamir Reznik **
**    ID - 308346170    **
**************************
*/


#include "glut.h"
#include <time.h>
#include <vector>
#include<stack>
#include "CellManager.h"
#include <iostream>
#include "main.h"
#include "CompareCells.h"

using namespace std;

int maze[MSZ][MSZ] = { 0 }; // 0 is SPACE
int targetRow, targetCol;
bool bfs_is_on = false, bestFirst_is_on = false, AStar_is_on = false;

Cell* targetCell, * startCell;

vector <Cell*> BFSGrays;
stack <CellManager*> bestFirstGrays;
priority_queue<Cell*, vector <Cell*>, CompareCellsAStar > aStarGrays;


void InitMaze()
{
	int i, j;

	// set margins
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1)  // this is mostly SPACE
			{
				if (rand() % 100 < 10) // 10% WALLs
					maze[i][j] = WALL;
				else
					maze[i][j] = SPACE;
			}
			else // this is mostly WALL
			{
				if (rand() % 100 < 40) // 40% SPACEs
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}

	maze[MSZ / 2][MSZ / 2] = START;


	targetCell = new Cell(rand() % MSZ, rand() % MSZ, nullptr);
	targetCol = targetCell->GetCol();
	targetRow = targetCell->GetRow();
	maze[targetCell->GetRow()][targetCell->GetCol()] = TARGET;
	startCell = new Cell(MSZ / 2, MSZ / 2, nullptr);
}

void init_BFS()
{
	BFSGrays = vector <Cell*>();
	BFSGrays.push_back(startCell); // add first cell to grays
}

void init_BestFirstSearch()
{
	bestFirstGrays = stack <CellManager*>();

	//push start cell
	bestFirstGrays.push(new CellManager(startCell));

	//remember target cell 
	targetCell = new Cell(targetRow, targetCol, nullptr);


}

void init_AStar()
{

	aStarGrays = priority_queue<Cell*, vector <Cell*>, CompareCellsAStar >();

	startCell->SetG(0);

	aStarGrays.push(startCell);

}

void init()
{
	glClearColor(0.8, 0.7, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);

	srand(time(0));

	InitMaze();
	init_BFS();
	init_BestFirstSearch();
	init_AStar();
}

void DrawMaze()
{
	int i, j;
	double sx, sy; // cell size
	double x, y;

	sx = 2.0 / MSZ;
	sy = 2.0 / MSZ;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])
			{
			case SPACE:
				glColor3d(1, 1, 1);   // white
				break;
			case WALL:
				glColor3d(0.7, 0.4, 0);   // dark-orange
				break;
			case GRAY_START:
			case START:
				glColor3d(0, 1, 0);   // cyan
				break;
			case TARGET:
				glColor3d(1, 0, 0);   // red
				break;
			case GRAY:
				glColor3d(1, 1, 0);   // yellow
				break;
			case BLACK:
				glColor3d(0.2, 0.2, 0.2);   // black
				break;
			case PATH:
				glColor3d(0, 0.7, 1);   //  blue
				break;
			}
			// draw square maze[i][j]
			x = 2 * (j / (double)MSZ) - 1;
			y = 2 * (i / (double)MSZ) - 1;

			glBegin(GL_POLYGON);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();
			glColor3d(0, 0, 0);

			glBegin(GL_LINE_LOOP);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();

		}
}

void RestorePath(Cell* current)
{
	while (current->GetParent() != nullptr)
	{
		maze[current->GetRow()][current->GetCol()] = PATH;
		current = current->GetParent();
	}
}

void CheckNeighborBFS(Cell* current, int row, int col)
{
	Cell* next;
	if (maze[row][col] == TARGET)
	{
		bfs_is_on = false;
		RestorePath(current);
	}
	else
		if (maze[row][col] == SPACE) // paint it gray
		{
			maze[row][col] = GRAY;
			next = new Cell(row, col, current);
			BFSGrays.push_back(next);
		}
}

void CheckNeighborAStar(Cell* current, Cell* neighbor)
{

	Cell tempNeighbor = *neighbor;
	switch (maze[neighbor->GetRow()][neighbor->GetCol()])
	{
	case TARGET://path found algorithm is finish
		cout << "Solution Found!\n";
		AStar_is_on = false;
		RestorePath(current);
		break;
	case GRAY:// check if a better path found to the neighbor Cell and update neighbor accordingly
		tempNeighbor.SetG(current->GetG() + 1);
		if (CompareCellsAStar::compareCells(neighbor, &tempNeighbor))
		{
			neighbor->SetG(tempNeighbor.GetG());
			neighbor->SetParent(current);
		}
		break;
	case SPACE://set relevant info (g, parentCell, color in maze) and add it to queue
		neighbor->SetG(current->GetG() + 1);
		neighbor->SetParent(current);
		maze[neighbor->GetRow()][neighbor->GetCol()] = GRAY;
		aStarGrays.push(neighbor);
		break;
	default:
		break;
	}


}

void CheckNeighborBestFirst(CellManager* current)
{
	if (maze[current->GetRow()][current->GetCol() + 1] == TARGET
		|| maze[current->GetRow() + 1][current->GetCol()] == TARGET
		|| maze[current->GetRow()][current->GetCol() - 1] == TARGET
		|| maze[current->GetRow() - 1][current->GetCol()] == TARGET)
	{
		cout << "Solution Found!\n";
		bestFirst_is_on = false;
		RestorePath(current->GetCell());
	}



}

void AddNeighborsBestFirst(CellManager* current)
{
	if (maze[current->GetRow() + 1][current->GetCol()] == SPACE)
		current->AddNeighbor(new Cell(current->GetRow() + 1, current->GetCol(), current->GetCell()));

	if (maze[current->GetRow()][current->GetCol() + 1] == SPACE)
		current->AddNeighbor(new Cell(current->GetRow(), current->GetCol() + 1, current->GetCell()));

	if (maze[current->GetRow()][current->GetCol() - 1] == SPACE)
		current->AddNeighbor(new Cell(current->GetRow(), current->GetCol() - 1, current->GetCell()));

	if (maze[current->GetRow() - 1][current->GetCol()] == SPACE)
		current->AddNeighbor(new Cell(current->GetRow() - 1, current->GetCol(), current->GetCell()));

}

void AddNeighborsAStar(Cell* current)
{
	CheckNeighborAStar(current, new Cell(current->GetRow() + 1, current->GetCol(), current));
	CheckNeighborAStar(current, new Cell(current->GetRow(), current->GetCol() + 1, current));
	CheckNeighborAStar(current, new Cell(current->GetRow() - 1, current->GetCol(), current));
	CheckNeighborAStar(current, new Cell(current->GetRow(), current->GetCol() - 1, current));



}

void BestFirstIteration()
{
	CellManager* current = bestFirstGrays.top();
	int currentRow = current->GetRow(), currentCol = current->GetCol();

	if (maze[currentRow][currentCol] == SPACE || maze[currentRow][currentCol] == START)
	{
		maze[currentRow][currentCol] = maze[currentRow][currentCol] == START ? GRAY_START : GRAY;
		AddNeighborsBestFirst(current);
	}

	CheckNeighborBestFirst(current);

	if (current->NumOfNeighbors() > 0)
		bestFirstGrays.push(new CellManager(current->GetClosestNeighbor()));

	else
		if (maze[currentRow][currentCol] == GRAY_START)
		{
			//Can't find target
			cout << "no more grays....no solution\n";
			bestFirst_is_on = false;
		}
		else
		{
			bestFirstGrays.pop();
			maze[currentRow][currentCol] = BLACK;
		}
}

void BFSIteration()
{
	Cell* current;
	int curr_row, curr_col;


	if (BFSGrays.empty()) // no more grays....no solution
	{
		cout << "no more grays....no solution\n";
		bfs_is_on = false;
	}
	else
	{
		// take the first element, paint it black and add to grays its white neighbors
		current = BFSGrays.front();
		BFSGrays.erase(BFSGrays.begin()); // remove the front element
		curr_row = current->GetRow();
		curr_col = current->GetCol();
		// paint it black
		if (maze[curr_row][curr_col] != START)
			maze[curr_row][curr_col] = BLACK;
		// look for white neighbors
		// up
		CheckNeighborBFS(current, curr_row + 1, curr_col);
		if (bfs_is_on)		// down
			CheckNeighborBFS(current, curr_row - 1, curr_col);
		if (bfs_is_on)		// left
			CheckNeighborBFS(current, curr_row, curr_col - 1);
		if (bfs_is_on)		// right
			CheckNeighborBFS(current, curr_row, curr_col + 1);
	}
}

void AStarIteration()
{
	//get cell with minimum(F) from priority queue 
	Cell* current = aStarGrays.top();
	aStarGrays.pop();
	int currentRow = current->GetRow(), currentCol = current->GetCol();

	//checking each of the four neighbor cells
	CheckNeighborAStar(current, new Cell(currentRow + 1, currentCol, current));
	if (!AStar_is_on)
		return;
	CheckNeighborAStar(current, new Cell(currentRow, currentCol + 1, current));
	if (!AStar_is_on)
		return;
	CheckNeighborAStar(current, new Cell(currentRow - 1, currentCol, current));
	if (!AStar_is_on)
		return;
	CheckNeighborAStar(current, new Cell(currentRow, currentCol - 1, current));
	if (!AStar_is_on)
		return;

	//once the Cell is out of the queue need to paint it black his path from start not going to change
	if (maze[currentRow][currentCol] == GRAY)
		maze[currentRow][currentCol] = BLACK;

	//no more grays to check --> Can't find target
	if (aStarGrays.empty())
	{
		cout << "no solution\n";
		AStar_is_on = false;
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle()
{
	//Sleep(200);

	if (bfs_is_on)
		BFSIteration();

	if (bestFirst_is_on)
		BestFirstIteration();

	if (AStar_is_on)
		AStarIteration();



	glutPostRedisplay(); // indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // BFS
		bfs_is_on = true;
		break;
	case 2: // Best First Search
		bestFirst_is_on = true;
		break;

	case 3:
		AStar_is_on = true;
		break;
	case 4:
		bestFirst_is_on = false;
		bfs_is_on = false;
		AStar_is_on = false;
		init();
		break;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Pathfinding Algorithms");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("Best First Search", 2);
	glutAddMenuEntry("A-Star Search", 3);
	glutAddMenuEntry("New Maze", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();

}