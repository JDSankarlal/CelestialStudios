#include "Medic.h"



Medic::Medic():Player()
{
	type = medic;
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
