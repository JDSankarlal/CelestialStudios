#include "Assault.h"



Assault::Assault():Player()
{
	type = assault;
	initialHealth = 100.0f;
	setHealth(100);
}

Assault::Assault(Model & model) : Player(model)
{
	initialHealth = 100.0f;
	setHealth(100);
}

Assault::Assault(const char * path) : Player(path)
{
	initialHealth = 100.0f;
	setHealth(100);
}


Assault::~Assault()
{
}
