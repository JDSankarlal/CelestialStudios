#include "Player.h"



Player::Player() : Model()
{
}

Player::Player(Model& model) : Model(model)

{

}
Player::Player(Player& model) : Model(model)

{
	type = model.type;
}
Player::Player(const char * path) :Model(path)
{

}

Player::~Player()
{
}

int Player::getHealth()
{
	return health;
}

void Player::setHealth(int v)
{
	health = v;
}

int Player::getBulletCount()
{
	return bulletCount;
}

void Player::setBulletCount(int v)
{
	bulletCount = v;
}

float Player::getTimeSinceLastMissile() 
{ 
	return timeSinceLastMissile; 
} 
 
void Player::setTimeSinceLastMissile(float v) 
{ 
	timeSinceLastMissile = v; 
} 

int Player::getInitialHealth()
{
	return initialHealth;
}
