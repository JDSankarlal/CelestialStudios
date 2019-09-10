#pragma once
#include "Player.h"
#include "Turret.h"
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
	std::vector<Turret*> pTurrets;
private:
	//Turret
	static Turret* turret ;//98
	bool turretActive = false;
	float turretTime;
	float timeSinceLastTurret;
};

