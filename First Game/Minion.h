#pragma once
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

private:
	int health = 30;
	float moveSpeed;
	float timeSinceLastSpawn;
};

