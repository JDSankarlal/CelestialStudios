#include "Tank.h"



Tank::Tank():Player()
{
	type = tank;
	initialHealth = 250;
	setHealth(250);
}

Tank::Tank(Model & model) : Player(model)
{
	type = tank;
	initialHealth = 250;
	setHealth(250);
}

Tank::Tank(const char * path) : Player(path)
{
	type = tank;
	initialHealth = 250;
	setHealth(250);
}


Tank::~Tank()
{
}

float Tank::getTimeSinceLastShield()
{
	return timeSinceLastShield;
}

void Tank::setTimeSinceLastShield(float v)
{
	timeSinceLastShield = v;
}