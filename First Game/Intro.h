#pragma once
#include <GameEmGine.h>
#include "Menu.h"
class Intro:public Scene
{
public:
	// Set splash screen and start update
	void init()
	{
		mod.push_back(new Model("Models/Screen/Intro/Background/introBackGround.obj"));
		mod[0]->getTransformer().setScale(1.0f,1.5f, 1.0f);
		context->addModel(mod.back());
		LightSource::setSceneAmbient({0,0,0,255});

		keyPressed = [=](int a, int b) {keyInputPressed(a, b);  };
		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if (key == 'A')
			fadeout = true;
	}

	// doing the update for intro screen
	void updateIntro()
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
		if(!fadein)
		{
			LightSource::setLightAmount(1);
			LightSource::setLightType(LIGHT_TYPE::POINT, 0);
			LightSource::setPosition({0.0f,45.0f,-1000.0f}, 0);
			LightSource::setDiffuse({10,10,10,10}, 0);
			LightSource::setAttenuationQuadratic(0.04f, 0);
			LightSource::setDirection({0.0f,0.0f,.0f}, 0);
		}
		if(Xinput::buttonPressed(context->getController(0).buttons.A))
		{
			fadeout = true;
		}
		if(fadeout)
		{
			LightSource::setLightAmount(0);
			splashT += 0.01f;
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
				
				
				//menuInite();
				context->setScene(new Game);
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateIntro();

	}

private:
	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
};


