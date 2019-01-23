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

private:
	int health;
	float moveSpeed;
};

