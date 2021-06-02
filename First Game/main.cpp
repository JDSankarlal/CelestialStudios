//#define _USE_MATH_DEFINES 

#include <GameEmGine.h>
#include "Splash.h"
//#include "Game.h"

int main()
{
	GameEmGine::init("E.R.I.S.", 1820, 980, 0, 0, 0, false);//must go first
	
	PlayerSelect splash;
	GameEmGine::setScene(&splash);
	//GameEmGine::getWindow()->setFullScreen(true);
	GameEmGine::run();
	return 0;
}