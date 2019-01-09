#pragma once
#include <GameEmGine.h>
#include <functional>

class Splash:public Scene
{
	// Set intro screen
	Splash()
	{
		srand(clock());

		mod.push_back(new Model("Models/Screen/Splash/splashScreen.obj"));
		context->addModel(mod.back());
		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);

		LightSource::setSceneAmbient({255,255,255,255});

		context->setCameraType(ORTHOGRAPHIC);
		context->setCameraPosition({0,0,-100});

		//engine stuff
		context->setFPSLimit(60);
		context->setBackgroundColour(0.05f, 0.0f, 0.1f);

		thisUpdate = update;

		Scene::update = update;

	}

	// doing the update for splash screen
	void updateSplash()
	{
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(0, 255, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient >= 250)
			{
				fadein = false;
				fadeout = true;
				splashT = 0;
				splashAmbient = 255;
			}
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,1});
			if(splashAmbient <= 5)
			{
				//fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;


				context->removeModel(mod[0]);
				mod.clear();


				//IntroInite();
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{
		updateSplash();
	}

private:
	static std::function<void(double)> thisUpdate;

	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
};


