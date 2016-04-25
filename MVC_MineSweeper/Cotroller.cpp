#include"stdafx.h"
#include"Controller.h"

Controller::Controller(Model *m)
{
	model = m;
}

void Controller::Click(int x, int y,int z)
{
	int j = (x - z) / 20;
	int i = (y - 60) / 20;
	model->Click(i,j);
}

void Controller::RightClick(int x, int y,int z)
{
	int j = (x - z) / 20;
	int i = (y - 60) / 20;
	model->PutFlag(i, j);
}

void Controller::RestartClicked(int m,int n,int bombs)
{
	model->RestartGame(m,n,bombs);
}

void Controller::IncremetTime()
{
	model->IncrementTime();
}
