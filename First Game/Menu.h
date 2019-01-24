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
		GameEmGine::addModel(mod.back()); //Mod 0 
		mod.push_back(new Model("Models/Screen/Menu/Start.obj"));
		GameEmGine::addModel(mod.back()); //Mod 1
		mod.push_back(new Model("Models/Screen/Menu/Options.obj"));
		GameEmGine::addModel(mod.back()); //Mod 2
		mod.push_back(new Model("Models/Screen/Menu/Exit.obj"));
		GameEmGine::addModel(mod.back()); //Mod 3

		mod[0]->addChild(mod[1]);
		mod[0]->addChild(mod[2]);
		mod[0]->addChild(mod[3]);

		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
		LightSource::setSceneAmbient({0,0,0,255});

		//float windowHeight = (float)GameEmGine::getWindowHeight();
		//float windowWidth = (float)GameEmGine::getWindowWidth();
		//mod[1]->getTransformer().setPosition({windowWidth / 2,windowHeight /2,0});
		//mod[2]->getTransformer().setPosition({windowWidth / 2,windowHeight /2,0});
		//mod[3]->getTransformer().setPosition({windowWidth / 2,windowHeight /2,0});
		//float tmp= mod[0]->getHeight() / 4 ;
		for(unsigned int i = 1; i < mod.size(); i++)
		{
			mod[i]->getTransformer().setRotation({90,0,0});
			mod[i]->getTransformer().setScale(10.0f);
			mod[i]->getTransformer().setPosition({mod[0]->getWidth() - mod[i]->getWidth() - 200, -9.f * i + 15,0});

		}
		LightSource::setSceneAmbient({0,0,0,255});

		keyPressed = [=](int a, int b) {keyInputPressed(a, b);  };
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if (key == 'B')
			fadeout = true;
	}

	// doing the update for menu screenb
	void updateMenu()
	{
		static bool menuMoved = false;

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
		if(GameEmGine::isControllerConnected(0))
		{
			if(Xinput::buttonPressed(GameEmGine::getController(0).buttons.A))
			{
				fadeout = true;
			}

			static int lastOption;
			if(GameEmGine::getController(0).Coord2D_sticks[LS].y >= 0.8)
			{
				if(!menuMoved)
				{
					lastOption = option;
					option--;
					if(option < 1)
						option = 3;
					lerpParam = 0;
					mod[lastOption]->getTransformer().setScale(10);
					tmp = mod[option]->getTransformer().getScale();
					menuMoved = true;
				}
			}
			if(GameEmGine::getController(0).Coord2D_sticks[LS].y <= -0.8)
			{
				if(!menuMoved)
				{
					lastOption = option;
					option++;
					if(option > 3)
						option = 1;
					lerpParam = 0;
					mod[lastOption]->getTransformer().setScale(10);
					tmp = mod[option]->getTransformer().getScale();
					menuMoved = true;
				}
			}

			if(abs(GameEmGine::getController(0).Coord2D_sticks[LS].y) < .3f)
				menuMoved = false;
		}

		mod[option]->getTransformer().setScale(lerp(tmp, Coord3D(12.0f), lerpParam));
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

				GameEmGine::removeModel(mod[0]);
				mod.clear();

				//GamePlayInit();
				GameEmGine::setScene(new Game);
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


