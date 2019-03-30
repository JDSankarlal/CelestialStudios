#pragma once
#include "Player.h"
class Medic:
	public Player
{
public:
	void init();
	Medic();
	Medic(Player& model);
	Medic(const char* path);
	~Medic();

	virtual float getTimeSinceLastHeal();
	virtual void setTimeSinceLastHeal(float v);
	void getHealing(Player* player);
	void update(float dt);
	//Medic
	Model* healingCircle;//93
	float circleTime;
	bool isHealing = false;
private:
	float timeSinceLastHeal;
};

