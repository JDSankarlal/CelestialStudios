#define _USE_MATH_DEFINES 

#include <GameEmGine.h>
#include "Splash.h"
//#include "Game.h"

int main()
{
	GameEmGine::init("The Real Game", 1920, 1050, 0, 0, 0, false);
	GameEmGine::setScene(new Splash);

	GameEmGine::run();
	return 0;
}