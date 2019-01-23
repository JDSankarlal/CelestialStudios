#pragma once
#include <GameEmGine.h>
#include "Game.h"
class Menu:public Scene
{
public:
	// Set menu screen
	void init()
	{
		mod.push_back(new Model("Models/Screen/Menu/menu.obj"));
		context->addModel(mod.back()); //Mod 0 
		mod.push_back(new Model("Models/Screen/Menu/Start.obj"));
		context->addModel(mod.back()); //Mod 1
		mod.push_back(new Model("Models/Screen/Menu/Exit.obj"));
		context->addModel(mod.back()); //Mod 2
		mod.push_back(new Model("Models/Screen/Menu/Options.obj"));
		context->addModel(mod.back()); //Mod 3
		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);

		for (unsigned int i = 1; i < mod.size(); i++)
		{
			mod[i]->getTransformer().setRotation({90,0,0});
			mod[i]->getTransformer().setScale(5.0f);
		}
		LightSource::setSceneAmbient({0,0,0,255});
	}

	// doing the update for menu screenb
	void updateMenu()
	{
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(0, 255, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient >= 250)
			{
				fadein = false;
				splashT = 0;
				splashAmbient = 255;
				LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			}
		}

		if(Xinput::buttonPressed(context->getController(0).buttons.A))
		{
			fadeout = true;
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashT = splashT > 1 ? 1 : splashT;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient <= 5)
			{
				fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;

				context->removeModel(mod[0]);
				mod.clear();

				//GamePlayInit();
				context->setScene(new Game);
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateMenu();
	}

private:
	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
};


