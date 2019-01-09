#pragma once
#include <GameEmGine.h>
class Intro:public Scene
{
	// Set splash screen and start update
	void IntroInite()
	{
		intro = true;

		mod.push_back(new Model("Models/Screen/Intro/Background/introBackGround.obj"));
		game.addModel(mod.back());
		LightSource::setSceneAmbient({255,255,255,255});
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
			LightSource::setPosition({0.0f,45.0f,10.0f}, 0);
			LightSource::setDiffuse({10,10,10,10}, 0);
			LightSource::setAttenuationQuadratic(0.04f, 0);
			LightSource::setDirection({0.0f,0.0f,.0f}, 0);
		}
		if(Xinput::buttonPressed(game.getController(0).buttons.A))
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

				intro = false;
				game.removeModel(mod[0]);
				mod.clear();
				menuInite();
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{

		updateIntro();

	}
};


