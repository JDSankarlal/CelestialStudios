#pragma once
#include "Player.h"


class Assault :
	public Player
{
public:
	void init();
	Assault();
	Assault(Player& model);
	Assault(const char * path);
	~Assault();
	virtual float getTimeSinceLastMissile();
	virtual void setTimeSinceLastMissile(float v);

	bool missileCollision(Model* mod);

	void update(float dt);

	//Assault
	std::vector<Model*> pMissiles;
	std::vector<Coord3D> missileVelocity;


private:
	static Model* missile;//44
	float m_timeSinceLastMissile=0;
};

