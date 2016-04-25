#include"vector"
#include"Observer.h"
#include<algorithm>
#include<stack>

enum DiscoveredStates { UNDISCOVERED, DISCOVERED, FLAG};

class Model
{
	int numberOfBombs;
	int numberOfRemainingFlags;
	int secondsPassed;
	int minutesPassed;
	int mtable, ntable;
	bool isGameOver;
	bool isGameWin;
	std::vector<std::vector<int>> table;
	std::vector<std::vector<DiscoveredStates>> discoveredTable;
	std::vector<Observer*> ob;
	std::vector<int> bombs;
	void DiscoverAllBombs();
	void DiscoverCells(int i, int j);
	void DiscoverSection(int i,int j, std::stack<std::pair<int,int>> & waitingStack);
	void DiscoverSectionOnDiagonal(int i, int j);
	bool CheckIfIsWin();
public:
	Model();
	void InitTable(int m, int n, int numberOfBombs);
	void AddObserver(Observer*);
	void RemoveObserver(Observer*);
	void NotifyObservers(Change);
	void Click(int i, int j);
	void PutFlag(int i, int j);
	void GenerateBombs(int numberOfCells);
	void RestartGame(int m, int n, int bombs);
	void IncrementTime();
	int NumberOfNeighborBombs(int i, int j);
	int GetElementFromTableAt(int i, int j);
	int GetNumberOfRemainingFlags();
	DiscoveredStates GetElementFromDiscoveredTableAt(int i, int j);
	CString GetTimePassed();
	
};
