#pragma once
class GameEmGine;

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}


	//shader initialization
	virtual void shaderInit() {}

	inline void setContext(GameEmGine* game) { context = game; }

	//updates within game loop
	virtual void update(double dt) { dt; }

	void(*render)(void);

	//instance key is pressed or held
	void(*keyPressed)(int key, int mod);

	//instance key is released
	void(*keyReleased)(int key, int mod);

	//instance button is pressed or held
	void(*mousePressed)(int key, int mod);

	//instance button is released
	void(*mouseReleased)(int key, int mod);

protected:
	GameEmGine* context;//this should ALWAYS be a GameEmGine object
};

