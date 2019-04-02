#pragma once
#include <GameEmGine.h>
#include "Game.h"
#include "Controls.h"

class Menu:public Scene
{
public:

	~Menu()
	{
		mod.clear();
		 fadein = true;
		 fadeout = false;
		 splashT = 0;
		 splashAmbient = 0;
		 lerpParam = 1;
		 option = 1;
	}

	// Set menu screen
	void init()
	{
		mod.clear();
		fadein = true;
		fadeout = false;
		splashT = 0;
		splashAmbient = 0;
		lerpParam = 1;
		option = 1;



		GameEmGine::m_modelShader->sendUniform("darken", false);

		mod.push_back(new Model("Models/Scene/Menu/menu.obj"));
		GameEmGine::addModel(mod.back()); //Mod 0 
		mod.push_back(new Model("Models/Scene/Menu/Start.obj"));
		GameEmGine::addModel(mod.back()); //Mod 1
		mod.push_back(new Model("Models/Scene/Menu/Options.obj"));
		GameEmGine::addModel(mod.back()); //Mod 2
		mod.push_back(new Model("Models/Scene/Menu/Exit.obj"));
		GameEmGine::addModel(mod.back()); //Mod 3

		mod[0]->addChild(mod[1]);
		mod[0]->addChild(mod[2]);
		mod[0]->addChild(mod[3]);

		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
		LightSource::setSceneAmbient({0,0,0,255}); //255

		for(unsigned int i = 1; i < mod.size(); i++)
		{
			mod[i]->getTransformer().setRotation({90,0,0});
			mod[i]->getTransformer().setScale(10.0f);
			mod[i]->getTransformer().setPosition({mod[0]->getWidth() - mod[i]->getWidth() - 200, -9.f * i + 15,0});

		}
		LightSource::setSceneAmbient({0,0,0,255});
		LightSource::setLightAmount(0);

		GameEmGine::setCameraType(ORTHOGRAPHIC);
		GameEmGine::setCameraPosition({ 0,0,-100 });
		GameEmGine::setCameraAngle(0,{ 1,1,1 });


		keyPressed = [=](int a, int b) {keyInputPressed(a, b);  };
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if(key == 'B')
			fadeout = true;
	}

	// doing the update for menu screenb
	void updateMenu()
	{
		static bool menuMoved[] = {false,false,false,false};

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

		static Coord3D tmp = Coord3D(20.0f);

		for(int a = 0; a < 4; a++)
			if(GameEmGine::isControllerConnected(a))
			{
				static int lastOption;
				if(abs(((XinputController*)GameEmGine::getController(a))->getSticks()[LS].y) >= 0.8)
				{
					if(!menuMoved[a])
					{
						lastOption = option;
						option += ((XinputController*)GameEmGine::getController(a))->getSticks()[LS].y < 0 ? 1 : -1;

						option = option > 3 ? 1 : option < 1 ? 3 : option;

						lerpParam = 0;
						mod[lastOption]->getTransformer().setScale(10);
						mod[lastOption]->setColour({255,255,255});
						tmp = mod[option]->getTransformer().getScale();
						menuMoved[a] = true;
					}
				}

				if(abs(((XinputController*)GameEmGine::getController(a))->getSticks()[LS].y) < .3f)
					menuMoved[a] = false;

				if(GameEmGine::getController(a)->isButtonPressed(CONTROLLER_A))
				{
					switch(option)
					{
					case 1:
						fadeout = true;
						break;
					case 2:

						break;
					case 3:
						GameEmGine::exit();
						break;
					default:
						break;
					}
				}
			}

		mod[option]->getTransformer().setScale(lerp(tmp, Coord3D(12.0f), lerpParam));
		mod[option]->setColour(lerp(ColourRGBA{255,255,255}, ColourRGBA{0,255,255}, lerpParam));
		lerpParam += .1f;

		if(lerpParam >= 1)
		{
			lerpParam = 1;
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

				//GamePlayInit();
				GameEmGine::setScene(new Controls);
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
	float lerpParam = 1;
	int option = 1;
};


