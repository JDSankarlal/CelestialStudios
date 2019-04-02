#pragma once
#include <GameEmGine.h>
#include "PlayerSelect.h"
class Controls :public Scene
{
public:
	// Set splash screen and start update
	void init()
	{
		mod.clear();
		fadein = true;
		fadeout = false;
		splashT = 0;
		splashAmbient = 0;
		

		
		GameEmGine::m_modelShader->sendUniform("darken", 0);

		mod.push_back(new Model("Models/Scene/Controls/controller.obj"));
		mod[0]->getTransformer().setScale(1.0f, 1.5f, 1.0f);

		GameEmGine::addModel(mod.back());
		LightSource::setSceneAmbient({ 0,0,0,255 });
		LightSource::setLightAmount(0);

		GameEmGine::setCameraType(ORTHOGRAPHIC);
		GameEmGine::setCameraPosition({ 0,0,-100 });

		keyPressed = [&](int a, int b) {keyInputPressed(a, b);  };
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
		if (fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(0, 255, splashT);
			LightSource::setSceneAmbient({ splashAmbient,splashAmbient,splashAmbient,splashAmbient });
			if (splashAmbient >= 250)
			{
				fadein = false;
				splashT = 0;
				splashAmbient = 255;
				LightSource::setSceneAmbient({ splashAmbient,splashAmbient,splashAmbient,splashAmbient });
			}
		}

		if (!fadein)
		{
		}

		for (int a = 0; a < 4; a++)
			if (GameEmGine::getController(a)->isButtonPressed(CONTROLLER_A))
			{
				fadeout = true;
			}

		if (fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({ splashAmbient,splashAmbient,splashAmbient,splashAmbient });
			if (splashAmbient <= 5)
			{
				fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;

				GameEmGine::removeModel(mod[0]);
				mod.clear();


				//menuInite();
				GameEmGine::setScene(new PlayerSelect);
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



