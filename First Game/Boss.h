#pragma once
#include <GameEmGine.h>

class Boss : public Model
{
public:
	Boss();
	Boss(Model & model);
	Boss(const char * path);
	~Boss();

	bool randAttacks();
	int getHealth();
	void setHealth(int v);

private:
	int health = 1000;
	int attackDamage;
	float attackCooldown;
};

