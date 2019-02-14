#include "Tank.h"



Tank::Tank():Player()
{
	type = tank;
	setHealth(250);
}

Tank::Tank(Model & model) : Player(model)
{
	setHealth(250);
}

Tank::Tank(const char * path) : Player(path)
{
	setHealth(250);
}


Tank::~Tank()
{
}
