#define _USE_MATH_DEFINES 
#pragma once
#include <GameEmGine.h>
#include "Minion.h"
#include <EmGineAudioPlayer.h>
typedef EmGineAudioPlayer AudioPlayer;
class Turret : public Model
{
public:
	Turret();
	Turret(const char* str);
	~Turret();

	void setLifeTime(float t);
	float getLifeTime();

	void setHealth(int v);
	int getHealth();

	void update(float dt);
	void setMinions(std::vector<Minion*> minions);
private:
	float m_lifeTime;
	int m_health;
	
	std::vector<Model*> bullets;
	float move = .1f;
	std::vector<Minion*> m_minions;
	std::vector<Coord3D> velocity;
};

