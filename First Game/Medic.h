#pragma once
#include "Player.h"
class Medic :
	public Player
{
public:
	Medic();
	Medic(Model & model);
	Medic(const char * path);
	~Medic();

	virtual float getTimeSinceLastHeal();
	virtual void setTimeSinceLastHeal(float v);

private:
	float timeSinceLastHeal;
};

