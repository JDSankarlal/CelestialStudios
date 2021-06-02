#pragma once
#include <GameEmGine.h>
#include "Menu.h"

class Intro:public Scene
{
public:
	void onSceneExit() {}

	// Set splash screen and start update
	void init()
	{
		LightManager::enableShadows(false);

		//		GameEmGine::m_modelShader->sendUniform("darken", 0);

		//Model tmp("Models/Scene/Intro/Background/introBackGround.obj");
		Model tmp(new PrimitivePlane(115, 65), "intro Scene");
		tmp.replaceTexture(0, 0, ResourceManager::getTexture2D("texture/scene/intro/introBackGround.png"));
		mod.push_back(tmp);
		mod.back().scale(16);
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);

		GameEmGine::addModel(&mod.back());

		LightManager::addLight(&sceneLight);
		sceneLight.setLightType(Light::DIRECTIONAL);
		sceneLight.rotate({-45,0,0});
		sceneLight.setAmbient({0,0,0,255});
		sceneLight.setDiffuse({0,0,0,255});
		sceneLight.setSpecular({0,0,0,255});

		keyPressed = [&](int a, int b) {keyInputPressed(a, b);  };

	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if(key == 'A' && !fadeout)
		{
			fadeout = true;
			splashT = 0;
		}
	}

	// doing the update for intro screen
	void updateIntro()
	{
		GLubyte maxi = GLubyte(255 * .3f);
		GLubyte mini = 0;
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(mini, maxi, splashT);
			sceneLight.setAmbient({splashAmbient,splashAmbient,splashAmbient,255});

			if(splashT >= 1)
			{
				fadein = false;

				splashAmbient = maxi;
			}
			mod[0].setColour(1, 1, 1, splashT);
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(maxi, mini, splashT);
			sceneLight.setAmbient({splashAmbient,splashAmbient,splashAmbient,255});

			mod[0].setColour(1, 1, 1, 1 - splashT);
			if(splashT >= 1)
			{
				//fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = mini;

				GameEmGine::removeModel(&mod[0]);
				mod.clear();

				static Menu menu;
				GameEmGine::setScene(&menu);
			}
		}

		for(int a = 0; a < 4; a++)
			if(GameEmGine::getController(a)->isButtonPressed(CONTROLLER_A)&&!fadeout)
			{
				fadeout = true;
				splashT = 0;
			}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateIntro();
	}

private:
	std::vector<Model> mod;
	Light sceneLight;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
};


