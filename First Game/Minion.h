#pragma once
#include "player.h"
#include <Model.h>

class Minion : public Model
{
public:
	Minion();
	Minion(Model & model);
	Minion(const char * path);
	~Minion();

	int getHealth();
	void setHealth(int v);
	float getTimeSinceLastSpawn();
	void setTimeSinceLastSpawn(float v);
	static void addTarget(Player* mod);
	void update(float dt);

	void move(bool move);

private:
	bool m_move;
	static std::vector<Player*> targets;
	int m_health = 30;
	float m_moveSpeed;
	float m_timeSinceLastSpawn;
};

