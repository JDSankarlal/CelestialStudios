//#define _USE_MATH_DEFINES 

#include <GameEmGine.h>
#include "Splash.h"
//#include "Game.h"

int main()
{
	Transformer::applyAspect();

	GameEmGine::init("The Real Game", 1920, 1080, 0, 0, 0, false);
	GameEmGine::setScene(new Splash);
	//GameEmGine::getWindow()->setFullScreen(true);
	GameEmGine::run();
	return 0;
}