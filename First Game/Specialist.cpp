#include "Specialist.h"



Specialist::Specialist():Player()
{
}

Specialist::Specialist(Model & model) : Player(model)
{
}

Specialist::Specialist(const char * path) : Player(path)
{
}


Specialist::~Specialist()
{
}
