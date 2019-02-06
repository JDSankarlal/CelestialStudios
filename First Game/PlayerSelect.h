#pragma once
#include <GameEmGine.h>
#include "Game.h"
class PlayerSelect :public Scene
{
public:
	// Set menu screen
	void init()
	{
		mod.push_back(new Model("Models/Screen/Menu/menu.obj"));
		GameEmGine::addModel(mod.back()); //Mod 0 
		mod.push_back(new Model("Models/Screen/Menu/Start.obj"));
		GameEmGine::addModel(mod.back()); //Mod 1
		mod.push_back(new Model("Models/ClassPH/Assault/assaultPH.obj")); // I wrote them like this because they will all be different models eventually I think.
		GameEmGine::addModel(mod.back()); // mod 2

		mod[2]->getTransformer().setRotation({ 0,270,180 }), mod[2]->getTransformer().setScale(10);
		mod[2]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Tank/tankPH.obj"));
		GameEmGine::addModel(mod.back()); // mod 3

		mod[3]->getTransformer().setRotation({ 0,270,180 }), mod[3]->getTransformer().setScale(10);
		mod[3]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Medic/medicPH.obj"));
		GameEmGine::addModel(mod.back()); // mod 4

		mod[4]->getTransformer().setRotation({ 0,270,180 }), mod[4]->getTransformer().setScale(10);
		mod[4]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Specialist/specPH.obj"));
		GameEmGine::addModel(mod.back()); // mod 5

		mod[5]->getTransformer().setRotation({ 0,270,180 }), mod[5]->getTransformer().setScale(10);
		mod[5]->setToRender(false);
		//TODO: Add back button and more flashy start button and "Press A to ready" buttons

		mod.push_back(new Model(*mod[3]));
		GAME::addModel(mod.back()); //6
		mod[6]->getTransformer().setPosition(-30, 0, 0), mod[6]->getTransformer().setRotation({ 0,270,180 }), mod[6]->getTransformer().setScale(8);
		mod[6]->setToRender(true);
		mod.push_back(new Model(*mod[3]));
		GAME::addModel(mod.back()); //7
		mod[7]->getTransformer().setPosition(-10, 0, 0), mod[7]->getTransformer().setRotation({ 0,270,180 }), mod[7]->getTransformer().setScale(8);
		mod[7]->setToRender(true);
		mod.push_back(new Model(*mod[3]));
		GAME::addModel(mod.back()); //8
		mod[8]->getTransformer().setPosition(10, 0, 0), mod[8]->getTransformer().setRotation({ 0,270,180 }), mod[8]->getTransformer().setScale(8);
		mod[8]->setToRender(true);
		mod.push_back(new Model(*mod[3]));
		GAME::addModel(mod.back()); //9
		mod[9]->getTransformer().setPosition(30, 0, 0), mod[9]->getTransformer().setRotation({ 0,270,180 }), mod[9]->getTransformer().setScale(8);
		mod[9]->setToRender(true);

		//See GDD for general layout of this screen.

		mod[0]->addChild(mod[1]);

		LightSource::setSceneAmbient({ 0,0,0,255 });

		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
		mod[1]->getTransformer().setRotation({ 90,0,0 });
		mod[1]->getTransformer().setScale(15.0f);
		mod[1]->getTransformer().setPosition({ mod[0]->getWidth() - mod[1]->getWidth() - 200, -9.f * 1 + 15,0 });

		LightSource::setSceneAmbient({ 0,0,0,255 });

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
		static Model *classes[4]
		{ new Assault("Models/ClassPH/Assault/assaultPH.obj"),new Tank("Models/ClassPH/Tank/tankPH.obj"),
			new  Medic("Models/ClassPH/Medic/medicPH.obj"),new Specialist("Models/ClassPH/Specialist/specPH.obj") };
		static bool menuMoved[] = { false,false,false,false };

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

		static Coord3D tmp = Coord3D(20.0f);

		for (int a = 0; a < 4; a++)
			if (GameEmGine::isControllerConnected(a))
			{
				if (abs(GameEmGine::getController(a).Coord2D_sticks[LS].x) >= 0.8)
				{
					if (!menuMoved[a])
					{
						
						option[a] += GameEmGine::getController(a).Coord2D_sticks[LS].x < 0 ? 1 : -1;

						option[a] = option[a] > 3 ? 0 : option[a] < 0 ? 3 : option[a];

						lerpParam = 0;
						//mod[lastOption]->getTransformer().setScale(10);
						GameEmGine::removeModel(mod[6 + a]);
						mod[6 + a]->setColour({ 255,255,255 });
						*mod[6 + a] = *classes[option[a]];
						mod[6+a]->getTransformer().setPosition(float(-30+a*20), 0, 0), mod[6 + a]->getTransformer().setRotation({ 0,270,180 }), mod[6 + a]->getTransformer().setScale(8);
						GameEmGine::addModel(mod[6 + a]);

						//tmp = mod[option]->getTransformer().getScale();
						menuMoved[a] = true;
					}
				}

				if (abs(GameEmGine::getController(a).Coord2D_sticks[LS].x) < .3f)
					menuMoved[a] = false;

				if (Xinput::buttonPressed(GameEmGine::getController(a).buttons.A))
				{
					switch (option[a])
					{
					case 1:
						fadeout = true;
						break;
						mod[1]->setColour(255, 0, 0, 0);
						//players[a] = assault
					case 2:
						//players[a] = tank
						break;
					case 3:
						//players[a] = medic
						break;
					case 4:
						//players[a] = specialist
						break;
					default:
						break;
					}
				}
			}

		//TODO: Set this to change a picture instead of this
		//mod[option]->getTransformer().setScale(lerp(tmp, Coord3D(12.0f), lerpParam));
		//mod[option]->setColour(lerp(ColourRGBA{ 255,255,255 }, ColourRGBA{ 0,255,255 }, lerpParam));
		//lerpParam += .1f;

		if (lerpParam >= 1)
		{
			lerpParam = 1;
		}

		if (fadeout)
		{
			splashT += 0.01f;
			splashT = splashT > 1 ? 1 : splashT;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({ splashAmbient,splashAmbient,splashAmbient,splashAmbient });
			if (splashAmbient <= 5)
			{
				fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;

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
	int option[4] = {0,0,0,0};
};