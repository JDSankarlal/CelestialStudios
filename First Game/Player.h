#pragma once
#include <Model.h>

enum PlayerType
{
	assault,
	tank,
	medic,
	specialist
};

class Player: public Model
{
public:
	Player();
	Player(Model& model);
	Player(Player& model);
	Player(const char* path);
	virtual ~Player();
	virtual int getHealth();
	virtual void setHealth(int v);
	virtual int getBulletCount();
	virtual void setBulletCount(int v);
	virtual float getTimeSinceLastMissile();
	virtual void setTimeSinceLastMissile(float v);

	int getInitialHealth();

	//Player Ability Variables
	float reloadTimer = false;
	bool reloading = false;
	float cooldown = false;
	bool f = false;
	PlayerType type;

	clock_t shotBuzzTimer;
	float shotBuzzDir = .9f;
	clock_t deathShakeTimer = 0;
	float deathShakeDir = 1.5f;

	bool dead;
protected:
	int initialHealth = 100;
private:
	int health = 100;
	int bulletCount = 30;
	float timeSinceLastMissile;
};
