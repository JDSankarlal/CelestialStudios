#pragma once
#include "Player.h"
#include "Minion.h"
#include <GameEmGine.h>


class Boss: public Model
{
public:
	void init();
	Boss();
	Boss(Model& model);
	Boss(const char* path);
	~Boss();

	void setPlayers(Player* players[4]);

	bool randAttacks();
	float getHealth();
	void setHealth(float v);
	std::vector<Model*>& getMissials();
	void update(float dt);

	std::vector<Minion*> minions;
	std::vector<Model* > missles;
	Model* lazer;
private:

	void shootLazer(int playerIndex);
	//boss's blood bar
	Model* m_lifeBar;
	Model* m_baseBar;

	float m_initialHealth, m_health;

	static Model* minion;
	int attackDamage;
	float attackCooldown;
	Player** targets;
};

