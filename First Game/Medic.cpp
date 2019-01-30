#include "Medic.h"



Medic::Medic():Player()
{
}

Medic::Medic(Model & model) : Player(model)
{
}

Medic::Medic(const char * path) : Player(path)
{
}


Medic::~Medic()
{
}
