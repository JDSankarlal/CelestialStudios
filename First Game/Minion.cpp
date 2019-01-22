#include "Minion.h"



Minion::Minion()
{
}

Minion::Minion(Model& model) : Model(model)

{

}

Minion::Minion(const char * path) :Model(path)
{

}


Minion::~Minion()
{
}

void Minion::setHealth(int v)
{
	health = v;
}

int Minion::getHealth()
{
	return health;
}
