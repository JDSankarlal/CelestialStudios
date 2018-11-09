#pragma once
#include <GameEmGine.h>

class Boss
{
public:
	Boss();
	~Boss();

	bool randAttacks();

private:
	int health;
	int attackDamage;
	float attackCooldown;
};

