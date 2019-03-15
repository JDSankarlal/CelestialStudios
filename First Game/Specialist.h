#pragma once
#include "Player.h"
class Specialist :
	public Player
{
public:
	void init();
	Specialist();
	Specialist(Player& model);
	Specialist(const char * path);
	~Specialist();

	virtual float getTimeSinceLastTurret();
	virtual void setTimeSinceLastTurret(float v);

	bool hitTurret(Model* mod);

	void update(float dt);

	//Specialist
	//Turret
	static Model* turret ;//98
	std::vector<Model*> pTurrets;
	bool turretActive = false;
	float turretTime;

private:
	float timeSinceLastTurret;
};

