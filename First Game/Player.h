#define _USE_MATH_DEFINES 
#pragma once

#include <GameEmGine.h>
#include <Model.h>
#include <EmGineAudioPlayer.h>
typedef EmGineAudioPlayer AudioPlayer;
typedef GameEmGine GAME;
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
	void init(int index);
	Player(int index = 0);
	Player(Player& model);
	Player(const char* path);
	virtual ~Player();
	void setPlayerIndex(int index);
	virtual int getHealth();
	virtual void setHealth(int v);
	virtual float getTimeSinceLastMissile();
	virtual void setTimeSinceLastMissile(float v);
	int getBulletCount();
	void setBulletCount(int v);
	void hitByEnemy(Model* mod);
	bool bulletCollisions(Model* mod);
	void onPlayArea(Model* mod);
	virtual void update(float dt);

	int getInitialHealth();

	//Player Ability Variables
	float reloadTimer = false;
	bool reloading = false;
	float cooldown = false;
	bool f = false;
	PlayerType type;

	float shotBuzzTimer;
	float shotBuzzDir = .2f;
	float deathShakeTimer = 0;
	float deathShakeDir = 0.6f;

	bool dead;
protected:
	int m_initialHealth = 100;
	int m_index;
	float angle;
	float duration = 0;
	Animation squash;
	Model* graveStone, //22
		* bulletCircle,//74
		* ringID,//26
		* gun;//54

	static Model* bullet;//48

	std::vector<float> timer;
	std::vector<Model*> bullets;
	std::vector<Coord3D> velocity;
	float move = .1f;

	static Model
		* redBar, * blueBar, * greenBar, * yellowBar,
		* baseRedBar, * baseBlueBar, * baseGreenBar, * baseYellowBar;

	Model* m_baseBar, * m_lifeBar;

	bool gunControlLaw = false, dashControl = false;
	float time;

	int m_health = 100;
	int m_bulletCount = 30;
	float m_timeSinceLastMissile;
};
