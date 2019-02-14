#include "Tank.h"



Tank::Tank():Player()
{
	type = tank;
	initialHealth = 250.0f;
	setHealth(250);
}

Tank::Tank(Model & model) : Player(model)
{
	initialHealth = 250.0f;
	setHealth(250);
}

Tank::Tank(const char * path) : Player(path)
{
	initialHealth = 250.0f;
	setHealth(250);
}


Tank::~Tank()
{
}
