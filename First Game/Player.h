#pragma once
#include <Model.h>

enum PlayerType
{
	assault,
	tank,
	medic,
	specialist
};

class Player : public Model
{
public:
	Player();
	Player(Model & model);
	Player(Player & model);
	Player(const char * path);
	virtual ~Player();
	virtual int getHealth();
	virtual void setHealth(int v);
	virtual int getBulletCount();
	virtual void setBulletCount(int v);
	
	float getInitialHealth();

	PlayerType type;
protected:
	float initialHealth = 100.0f;
private:
	int health = 100;
	int bulletCount = 30;
	
};
