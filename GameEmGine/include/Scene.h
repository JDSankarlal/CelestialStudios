#pragma once
#include <functional>

/*
Tricking the  system to think the 
GameEmgine class is created
*/
class GameEmGine;

class Scene
{
public:
	virtual ~Scene() {}


	//shader initialization
	virtual void shaderInit() {}

	//works without it for some reason
	inline void setContext(GameEmGine* game) { context = game; }

	//updates within game loop
	virtual void update(double dt) = 0;
	virtual void init() = 0;

	std::function<void(void)>render;

	std::function<void(int,int)>
	//instance key is pressed or held
	keyPressed,

	//instance key is released
	keyReleased,

	//instance button is pressed or held
	mousePressed,

	//instance button is released
	mouseReleased;

protected:
	GameEmGine* context;//this should ALWAYS be a GameEmGine object
};

