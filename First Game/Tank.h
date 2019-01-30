#pragma once
#include "Player.h"
class Tank :
	public Player
{
public:
	Tank();
	Tank(Model & model);
	Tank(const char * path);
	~Tank();
};

