#pragma once
#include <functional>

class GameEmGine;

class Scene
{
public:
	virtual ~Scene() {}

	virtual void init(){}

	//shader initialization
	virtual void shaderInit() {}

	inline void setContext(GameEmGine* game) { context = game; }

	//updates within game loop
	virtual void update(double dt) { dt; }

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

