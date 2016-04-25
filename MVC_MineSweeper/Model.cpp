#include"stdafx.h"
#include"Model.h"
#include"Changes.h"

Model::Model()
{
	isGameOver = false;
	isGameWin = false;
	secondsPassed = 0;
	minutesPassed = 0;
	mtable = ntable = 0;
	NotifyObservers(Change::TIME);
}

void Model::InitTable(int m, int n, int numberOfBombs)
{

	GenerateBombs(n*m);
	this->numberOfBombs = numberOfBombs;
	numberOfRemainingFlags = numberOfBombs;
	mtable = m;
	ntable = n;
	table.resize(m);
	discoveredTable.resize(m);



	for (int i=0;i<m;i++)
	{
		table[i].resize(n);
		discoveredTable[i].resize(n);
		for (int j = 0; j < n; j++)
		{
			table[i][j] = 0;
			discoveredTable[i][j] = DiscoveredStates::UNDISCOVERED;
		}
	}

	for (int i = 0; i < numberOfBombs; i++)
	{
		int row = (int)(bombs[i] / n);
		int column = (int)(bombs[i] % n);

		table[row][column] = -1;
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int numberOfNeighborBombs = NumberOfNeighborBombs(i, j);

			if (numberOfNeighborBombs != 0)
			{
				table[i][j] = numberOfNeighborBombs;
			}
		}
	}


	NotifyObservers(Change::TABLE);
	NotifyObservers(Change::FLAGS);
}

void Model::AddObserver(Observer*o)
{
	for (int i=0;i<(int)ob.size();i++)
		if (o == ob[i])
			return;
	ob.push_back(o);
}

void Model::RemoveObserver(Observer*o)
{
	for (int i=0;i<(int)ob.size();i++)
		if (o == ob[i])
		{
			ob.erase(ob.begin()+i);
			return;
		}
}

void Model::NotifyObservers(Change c)
{
	for (int i=0;i<(int)ob.size();i++)
		ob[i]->Update(this, c);
}

void Model::Click(int i, int j)
{
	if (i<0 || j<0 || i>=mtable || j>=ntable)
		return;
	if (isGameOver || isGameWin)
	{
		return;
	}

	if (discoveredTable[i][j] == DiscoveredStates::FLAG)
	{
		return;
	}

	if (table[i][j] == -1)
	{
		DiscoverAllBombs();
		isGameOver = true;
		NotifyObservers(Change::GAME_LOST);
	}

	if (discoveredTable[i][j] == DiscoveredStates::UNDISCOVERED)
	{
		discoveredTable[i][j] = DiscoveredStates::DISCOVERED;
	}

	if (table[i][j] == 0)
	{
		DiscoverCells(i, j);
	}
	if (CheckIfIsWin())
	{
		NotifyObservers(Change::GAME_WON);
	}
	NotifyObservers(Change::TABLE);
}

void Model::DblClick(int i, int j)
{
	int flags = NumberOfNeighborFlags(i, j);

	if (table[i][j] == flags)
	{
		Click(i - 1, j - 1);
		Click(i - 1, j);
		Click(i - 1, j + 1);
		Click(i, j - 1);
		Click(i, j + 1);
		Click(i + 1, j - 1);
		Click(i + 1, j);
		Click(i + 1, j + 1);
	}
}

void Model::PutFlag(int i, int j)
{
	if (i<0 || j<0 || i >= mtable || j >= ntable)
		return;
	if (isGameOver || isGameWin)
	{
		return;
	}
	if (numberOfRemainingFlags == 0)
	{
		return;
	}
	if (discoveredTable[i][j] == DiscoveredStates::UNDISCOVERED)
	{
		numberOfRemainingFlags--;
		discoveredTable[i][j] = DiscoveredStates::FLAG;
	}
	else if (discoveredTable[i][j] == DiscoveredStates::FLAG) 
	{
		discoveredTable[i][j] = DiscoveredStates::UNDISCOVERED;
		numberOfRemainingFlags++;
	}

	NotifyObservers(Change::TABLE);
	NotifyObservers(Change::FLAGS);
}

void Model::GenerateBombs(int numberOfCells)
{
	bombs.resize(numberOfCells);

	for (int i = 0; i < numberOfCells; i++)
	{
		bombs[i] = i;
	}

	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	int random = rand() % 1000 + 1;
	for (int i = 0; i < random; i++)
		std::random_shuffle(bombs.begin(), bombs.end());
}

void Model::RestartGame(int m,int n,int bombs)
{
	isGameOver = false;
	isGameWin = false;
	mtable = m;
	ntable = n;
	numberOfBombs = bombs;
	minutesPassed = 0;
	secondsPassed = 0;
	GenerateBombs(ntable*mtable);
	InitTable(mtable, ntable,numberOfBombs);
	NotifyObservers(Change::TIME);
	NotifyObservers(Change::TABLE);
}


int Model::NumberOfNeighborBombs(int row, int column)
{
	int bombCounter = 0;

	if (table[row][column] == -1)
		return 0;

	if (row == 0 && column == 0)
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if (row == 0 && column == ntable-1)
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if (row == mtable - 1 && column == 0)
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if (row == mtable - 1 && column == ntable-1)
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if (row == 0 && (column != 0 && column != ntable-1))
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if (row == mtable-1 && (column != 0 && column != ntable - 1))
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if((row != 0 && row != mtable-1) && (column != 0 && column != ntable - 1))
	{
		for (int i = row-1; i < row + 2; i++)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if ((row != 0 && row != mtable - 1) && column == 0)
	{
		for (int i = row - 1; i < row + 2; i++)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}

	if ((row != 0 && row != mtable - 1) && column == ntable-1)
	{
		for (int i = row - 1; i < row + 2; i++)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (table[i][j] == -1)
				{
					bombCounter++;
				}
			}
		}
	}
	

	return bombCounter;
}

int Model::NumberOfNeighborFlags(int row, int column)
{
	int flagCounter = 0;

	if (discoveredTable[row][column] == DiscoveredStates::FLAG)
		return 0;

	if (row == 0 && column == 0)
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if (row == 0 && column == ntable - 1)
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if (row == mtable - 1 && column == 0)
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if (row == mtable - 1 && column == ntable - 1)
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if (row == 0 && (column != 0 && column != ntable - 1))
	{
		for (int i = row; i < row + 2; i++)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if (row == mtable - 1 && (column != 0 && column != ntable - 1))
	{
		for (int i = row; i > row - 2; i--)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if ((row != 0 && row != mtable - 1) && (column != 0 && column != ntable - 1))
	{
		for (int i = row - 1; i < row + 2; i++)
		{
			for (int j = column - 1; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if ((row != 0 && row != mtable - 1) && column == 0)
	{
		for (int i = row - 1; i < row + 2; i++)
		{
			for (int j = column; j < column + 2; j++)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}

	if ((row != 0 && row != mtable - 1) && column == ntable - 1)
	{
		for (int i = row - 1; i < row + 2; i++)
		{
			for (int j = column; j > column - 2; j--)
			{
				if (discoveredTable[i][j] == DiscoveredStates::FLAG)
				{
					flagCounter++;
				}
			}
		}
	}


	return flagCounter;
}

int Model::GetElementFromTableAt(int i, int j)
{
	return table[i][j];
}

int Model::GetNumberOfRemainingFlags()
{
	return numberOfRemainingFlags;
}

DiscoveredStates Model::GetElementFromDiscoveredTableAt(int i, int j)
{
	return discoveredTable[i][j];
}

void Model::IncrementTime()
{
	if (isGameWin || isGameOver)
	{
		return;
	}
	secondsPassed++;
	if (secondsPassed == 60)
	{
		minutesPassed++;
		secondsPassed = 0;
	}

	NotifyObservers(Change::TIME);
}

CString Model::GetTimePassed()
{
	CString output;
	output.Format(_T("%.2d:%.2d"), minutesPassed, secondsPassed);
	return output;
}

void Model::DiscoverAllBombs()
{
	for (int i = 0; i < mtable; i++)
	{
		for (int j = 0; j < ntable; j++)
		{
			if (table[i][j] == -1)
			{
				discoveredTable[i][j] = DiscoveredStates::DISCOVERED;
			}
		}
	}
}

void Model::DiscoverCells(int i, int j)
{
	std::stack<std::pair<int, int>> waitingStack;

	std::pair<int, int> currentPosition;
	currentPosition.first = i;
	currentPosition.second = j;

	waitingStack.push(currentPosition);

	discoveredTable[currentPosition.first][currentPosition.second] = DiscoveredStates::DISCOVERED;

	while (!waitingStack.empty())
	{
		currentPosition = waitingStack.top();
		waitingStack.pop();

		int row = currentPosition.first;
		int column = currentPosition.second;

		//verifica colt stanga sus
		if (row == 0 && column == 0)
		{
			DiscoverSection(row, column + 1, waitingStack);
			DiscoverSection(row + 1, column, waitingStack);		
			DiscoverSectionOnDiagonal(row + 1, column + 1);
		}

		//verifica colt dreapta sus
		if (row == 0 && column == ntable - 1)
		{
			DiscoverSection(row, column - 1, waitingStack);
			DiscoverSection(row+1, column, waitingStack);
			DiscoverSectionOnDiagonal(row + 1, column - 1);
		}

		//verificare colt stanga jos
		if (row == mtable - 1 && column == 0)
		{
			DiscoverSection(row, column + 1, waitingStack);
			DiscoverSection(row-1, column, waitingStack);
			DiscoverSectionOnDiagonal(row - 1, column + 1);
		}

		//verificare colt dreapta jos
		if (row == mtable - 1 && column == ntable - 1)
		{
			DiscoverSection(row, column - 1, waitingStack);
			DiscoverSection(row - 1, column, waitingStack);
			DiscoverSectionOnDiagonal(row - 1, column - 1);
		}

		//verificare muchie sus
		if (row == 0 && (column != 0 && column != ntable - 1))
		{
			DiscoverSection(row, column - 1, waitingStack);
			DiscoverSection(row, column + 1, waitingStack);
			DiscoverSection(row+1, column, waitingStack);

			DiscoverSectionOnDiagonal(row + 1, column + 1);
			DiscoverSectionOnDiagonal(row + 1, column - 1);
		}

		//verificare muchie jos
		if (row == mtable - 1 && (column != 0 && column != ntable - 1))
		{
			DiscoverSection(row, column - 1, waitingStack);
			DiscoverSection(row, column + 1, waitingStack);
			DiscoverSection(row - 1, column, waitingStack);

			DiscoverSectionOnDiagonal(row - 1, column + 1);
			DiscoverSectionOnDiagonal(row - 1, column - 1);
		}

		//verificare toata tabla
		if ((row != 0 && row != mtable - 1) && (column != 0 && column != ntable - 1))
		{
			DiscoverSection(row, column - 1, waitingStack);
			DiscoverSection(row, column + 1, waitingStack);
			DiscoverSection(row - 1, column, waitingStack);
			DiscoverSection(row + 1, column, waitingStack);

			DiscoverSectionOnDiagonal(row + 1, column + 1);
			DiscoverSectionOnDiagonal(row + 1, column - 1);
			DiscoverSectionOnDiagonal(row - 1, column + 1);
			DiscoverSectionOnDiagonal(row - 1, column - 1);
		}

		//verificare muchie stanga
		if ((row != 0 && row != mtable - 1) && column == 0)
		{
			DiscoverSection(row - 1, column, waitingStack);
			DiscoverSection(row + 1, column, waitingStack);
			DiscoverSection(row, column + 1, waitingStack);

			DiscoverSectionOnDiagonal(row - 1, column + 1);
			DiscoverSectionOnDiagonal(row + 1, column + 1);
		}

		//verificare muchie dreapta
		if ((row != 0 && row != mtable - 1) && column == ntable - 1)
		{
			DiscoverSection(row - 1, column, waitingStack);
			DiscoverSection(row + 1, column, waitingStack);
			DiscoverSection(row, column - 1, waitingStack);

			DiscoverSectionOnDiagonal(row - 1, column - 1);
			DiscoverSectionOnDiagonal(row + 1, column - 1);
		}
	}
}

void Model::DiscoverSection(int i, int j, std::stack<std::pair<int, int>>& waitingStack)
{
	if (table[i][j] != -1)
	{
		std::pair<int, int> currentPosition;

		if (discoveredTable[i][j] == DiscoveredStates::UNDISCOVERED)
		{
			discoveredTable[i][j] = DiscoveredStates::DISCOVERED;

			if (table[i][j] == 0)
			{
				currentPosition.first = i;
				currentPosition.second = j;

				waitingStack.push(currentPosition);
			}
		}
	}
}

void Model::DiscoverSectionOnDiagonal(int i, int j)
{
	if (table[i][j] != -1)
	{
		std::pair<int, int> currentPosition;

		if (discoveredTable[i][j] == DiscoveredStates::UNDISCOVERED)
		{
			if (table[i][j] != 0)
			{
				discoveredTable[i][j] = DiscoveredStates::DISCOVERED;
			}
		}
	}
}

bool Model::CheckIfIsWin()
{
	for (int i = 0; i < mtable; i++)
	{
		for (int j = 0; j < ntable; j++)
		{
			if (table[i][j] != -1)
			{
				if (discoveredTable[i][j] != DiscoveredStates::DISCOVERED)
				{
					isGameWin = false;
					return false;
				}
			}
		}
	}

	isGameWin = true;
	return true;
}
