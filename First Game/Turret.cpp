#include "Turret.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

void Turret::setLifeTime(float t)
{
	m_lifeTime = t;
}

float Turret::getLifeTime()
{
	return m_lifeTime;
}

void Turret::setHealth(int v)
{
	m_health = v;
}

int Turret::getHealth()
{
	return m_health;
}
