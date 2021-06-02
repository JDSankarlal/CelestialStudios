#pragma once
#include <GameEmGine.h>
#include "Game.h"
#include "Controls.h"

class Menu:public Scene
{
public:
	void onSceneExit() {}

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
		LightManager::enableShadows(false);

		mod.clear();
		fadein = true;
		fadeout = false;
		splashT = 0;
		splashAmbient = 0;
		lerpParam = 1;
		option = 1;



		//GameEmGine::m_modelShader->sendUniform("darken", false);

		Model tmp(new PrimitivePlane(115, 65), "intro Scene");
		tmp.replaceTexture(0, 0, ResourceManager::getTexture2D("texture/scene/menu/menu.png"));
		mod.push_back(tmp);
		mod.back().scale(16);
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);

		mod.push_back(Model("Models/Scene/Menu/Start.obj"));
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);

		mod.push_back(Model("Models/Scene/Menu/Options.obj"));
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);

		mod.push_back(Model("Models/Scene/Menu/Exit.obj"));
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);

		for(auto& a : mod)
			GameEmGine::addModel(&a); //Mod 0 

		mod[0].addChild(&mod[1]);
		mod[0].addChild(&mod[2]);
		mod[0].addChild(&mod[3]);

		mod[0].scale(16);


		LightManager::addLight(&sceneLight);
		sceneLight.setLightType(Light::DIRECTIONAL);
		sceneLight.rotate({-45,0,0});
		sceneLight.setAmbient({0,0,0,255});
		sceneLight.setDiffuse({0,0,0,255});
		sceneLight.setSpecular({0,0,0,255});

		for(unsigned int i = 1; i < mod.size(); i++)
		{
			mod[i].rotate({90,0,0});
			mod[i].scale(10.0f);
		}


		GameEmGine::setCameraType(Camera::ORTHOGRAPHIC);
		GameEmGine::getMainCamera()->translate({0,0,-100});
		GameEmGine::getMainCamera()->rotate({0,0,0});


		keyPressed = [&](int a, int b) {keyInputPressed(a, b);  };
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if(key == 'B')
			fadeout = true;

	}

	// doing the update for menu screen
	void updateMenu()
	{

		for(unsigned int i = 1; i < mod.size(); i++)
		{
			
			mod[i].translate({20, -mod[i].getHeight() * i+15 , 0});
		}


		static bool menuMoved[] = {false,false,false,false};



		if(GetAsyncKeyState(VK_LEFT))
			mod[1].translateBy(-100.f, 0, 0);
		if(GetAsyncKeyState(VK_RIGHT))
			mod[1].translateBy(100.f, 0, 0);


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
			for(auto& a : mod)
				a.setColour(1, 1, 1, splashT);
		}



		if(fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(maxi, mini, splashT);
			sceneLight.setAmbient({splashAmbient,splashAmbient,splashAmbient,255});

			if(splashT >= 1)
			{
				fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = mini;

				
				static Controls controls;
				GameEmGine::setScene(&controls);
			}
			for(auto& a : mod)
				a.setColour(1, 1, 1, 1 - splashT);
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
						mod[lastOption].scale(10);
						mod[lastOption].setColour({255,255,255});
						tmp = mod[option].getScale();
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

		mod[option].scale(lerp(tmp, Coord3D(12.0f), lerpParam));
		mod[option].setColour(lerp(ColourRGBA{255,255,255}, ColourRGBA{0,255,255}, lerpParam));
		lerpParam += .1f;

		if(lerpParam >= 1)
		{
			lerpParam = 1;
		}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateMenu();
	}

private:
	std::vector<Model> mod;
	Light sceneLight;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
	float lerpParam = 1;
	int option = 1;
};


