#define _USE_MATH_DEFINES 
//#define 

#include <GameEmGine.h>
#include "Splash.h"



int main()
{
	
GameEmGine::init("The Real Game", 1920, 1080, 0, 0, 0, false);
GameEmGine::setScene(new Splash);

GameEmGine::run();
	return 0;
}