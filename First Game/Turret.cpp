#include "Turret.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

void Turret::setLifeTime(float t)
{
	lifeTime = t;
}

float Turret::getLifeTime()
{
	return lifeTime;
}

void Turret::setHealth(int v)
{
	health = v;
}

int Turret::getHealth()
{
	return health;
}
