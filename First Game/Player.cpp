#include "Player.h"



Player::Player() : Model()
{
}

Player::Player(Model& model) : Model(model)

{

}

Player::Player(const char * path) :Model(path)
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