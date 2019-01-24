#define _USE_MATH_DEFINES 

#include <GameEmGine.h>
#include "Splash.h"

GameEmGine game("The Real Game", 1920, 1080, 0, 0, 0, false);

int main()
{
	game.setScene(new Game);

	game.run();
	return 0;
}