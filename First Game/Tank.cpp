#include "Tank.h"



Tank::Tank():Player()
{
}

Tank::Tank(Model & model) : Player(model)
{
}

Tank::Tank(const char * path) : Player(path)
{
}


Tank::~Tank()
{
}
