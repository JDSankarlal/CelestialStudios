#include "Medic.h"



Medic::Medic():Player()
{
	type = medic;
	initialHealth = 150.0f;
	setHealth(150);
}

Medic::Medic(Model & model) : Player(model)
{
	initialHealth = 150.0f;
	setHealth(150);
}

Medic::Medic(const char * path) : Player(path)
{
	initialHealth = 150.0f;
	setHealth(150);
}


Medic::~Medic()
{
}
