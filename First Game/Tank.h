#pragma once
#include "Player.h"
class Tank :
	public Player
{
public:
	void init();
	Tank();
	Tank(Player& model);
	Tank(const char * path);
	~Tank();

	float getTimeSinceLastShield();
	void setTimeSinceLastShield(float v);
	void update(float dt);

	//Tank
	float shieldTime;
	bool tankShield = false;
private:
	float timeSinceLastShield;
};

