#include "Boss.h"



Boss::Boss() : Model()
{
}

Boss::Boss(Model& model) :Model(model)
	
{

}

Boss::Boss(const char * path):Model(path)
{
	
}

Boss::~Boss()
{
}

bool Boss::randAttacks()
{
	//Randomly Pick an attack (attacks should be other functions, this semester just missile attacks)
	//Use Attack
	//If Player(s) standing too close use stomp OR flop attack instead
	//Wait x Amount of time before repeating
	return false;
}

int Boss::getHealth()
{
	return health;
}

void Boss::setHealth(int v)
{
	health = v;
}
