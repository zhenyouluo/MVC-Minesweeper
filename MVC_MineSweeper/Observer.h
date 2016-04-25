#include"Changes.h"

class Observer
{
public:
	virtual void Update(void*,Change) = 0;
};
