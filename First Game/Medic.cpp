#include "Medic.h"



Medic::Medic():Player()
{
	setHealth(150);
}

Medic::Medic(Model & model) : Player(model)
{
	setHealth(150);
}

Medic::Medic(const char * path) : Player(path)
{
	setHealth(150);
}


Medic::~Medic()
{
}
