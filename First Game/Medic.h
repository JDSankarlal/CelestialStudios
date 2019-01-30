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
};

