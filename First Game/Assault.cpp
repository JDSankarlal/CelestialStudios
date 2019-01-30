#include "Assault.h"



Assault::Assault():Player()
{
}

Assault::Assault(Model & model) : Player(model)
{
}

Assault::Assault(const char * path) : Player(path)
{
}


Assault::~Assault()
{
}
