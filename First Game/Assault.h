#pragma once
#include "Player.h"
class Assault :
	public Player
{
public:
	Assault();
	Assault(Model & model);
	Assault(const char * path);
	~Assault();
	virtual float getTimeSinceLastMissile();
	virtual void setTimeSinceLastMissile(float v);

private:
	float timeSinceLastMissile;
};

