#include"Model.h"

class Controller
{
	Model *model;
public:
	Controller(Model *);
	void Click(int x, int y, int z);
	void RightClick(int x, int y,int z);
	void RestartClicked(int m, int n, int bombs);
	void IncremetTime();
};
