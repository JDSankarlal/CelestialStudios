#include "Specialist.h"



Specialist::Specialist():Player()
{
	setHealth(75);
}

Specialist::Specialist(Model & model) : Player(model)
{
	setHealth(75);
}

Specialist::Specialist(const char * path) : Player(path)
{
	setHealth(75);
}


Specialist::~Specialist()
{
}
