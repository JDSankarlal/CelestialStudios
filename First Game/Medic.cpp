#include "Medic.h"



Medic::Medic():Player()
{
	type = medic;
	initialHealth = 150;
	setHealth(150);
}

Medic::Medic(Model & model) : Player(model)
{
	type = medic;
	initialHealth = 150;
	setHealth(150);
}

Medic::Medic(const char * path) : Player(path)
{
	type = medic;
	initialHealth = 150;
	setHealth(150);
}


Medic::~Medic()
{
}

float Medic::getTimeSinceLastHeal()
{
	return timeSinceLastHeal;
}

void Medic::setTimeSinceLastHeal(float v)
{
	timeSinceLastHeal = v;
}