#include "Specialist.h"



Specialist::Specialist():Player()
{
	type = specialist;
	initialHealth = 75.0f;
	setHealth(75);
}

Specialist::Specialist(Model & model) : Player(model)
{
	initialHealth = 75.0f;
	setHealth(75);
}

Specialist::Specialist(const char * path) : Player(path)
{
	initialHealth = 75.0f;
	setHealth(75);
}


Specialist::~Specialist()
{
}
