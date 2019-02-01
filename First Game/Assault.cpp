#include "Assault.h"



Assault::Assault():Player()
{
	setHealth(100);
}

Assault::Assault(Model & model) : Player(model)
{
	setHealth(100);
}

Assault::Assault(const char * path) : Player(path)
{
	setHealth(100);
}


Assault::~Assault()
{
}
