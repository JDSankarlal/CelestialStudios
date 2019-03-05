#include "Specialist.h"



Specialist::Specialist():Player()
{
	type = specialist;
	initialHealth = 75;
	setHealth(75);
}

Specialist::Specialist(Model & model) : Player(model)
{
	type = specialist;
	initialHealth = 75;
	setHealth(75);
}

Specialist::Specialist(const char * path) : Player(path)
{
	type = specialist;
	initialHealth = 75;
	setHealth(75);
}


Specialist::~Specialist()
{
}

float Specialist::getTimeSinceLastTurret()
{
	return timeSinceLastTurret;
}

void Specialist::setTimeSinceLastTurret(float v)
{
	timeSinceLastTurret = v;
}