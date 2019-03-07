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
	
	int getInitialHealth();

	PlayerType type;
protected:
	int initialHealth = 100;
private:
	int health = 100;
	int bulletCount = 30;
	
};
