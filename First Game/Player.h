#pragma once
#include <Model.h>


class Player : public Model
{
public:
	Player();
	Player(Model & model);
	Player(const char * path);
	virtual ~Player();
	virtual int getHealth();
	virtual void setHealth(int v);
	virtual int getBulletCount();
	virtual void setBulletCount(int v);
	virtual float getTimeSinceLastMissile();
	virtual void setTimeSinceLastMissile(float v);

	enum PlayerType
	{
		assault,
		tank,
		medic,
		specialist
	} type;
private:
	int health = 100;
	int bulletCount = 30;
	float timeSinceLastMissile;
};
