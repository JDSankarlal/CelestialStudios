#define _USE_MATH_DEFINES 
#pragma once
#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include "Minion.h"
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
	void setMinions(std::list<Minion*> minions);
private:
	float m_lifeTime;
	int m_health;
	
	std::list<Model*> bullets;
	float move = .1f;
	std::list<Minion*> m_minions;
	std::vector<Coord3D<>> velocity;
};

