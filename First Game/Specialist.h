#pragma once
#include "Player.h"
class Specialist :
	public Player
{
public:
	Specialist();
	Specialist(Model & model);
	Specialist(const char * path);
	~Specialist();

	virtual float getTimeSinceLastTurret();
	virtual void setTimeSinceLastTurret(float v);

private:
	float timeSinceLastTurret;
};

