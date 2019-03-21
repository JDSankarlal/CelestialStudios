#pragma once
#include <GameEmGine.h>
#include "Game.h"

class PlayerSelect:public Scene
{
public:
	// Set menu screen
	//const float time = 0;
	//time = (float)dt;
	void init()
	{
		mod.push_back(new Model("Models/Scene/PlayerSelect/PlayerSelect.obj"));
		GameEmGine::addModel(mod.back()); //Mod 0 
		mod.push_back(new Model("Models/Scene/Menu/Start.obj"));
		GameEmGine::addModel(mod.back()); //Mod 1
		mod[1]->setToRender(false);



		mod.push_back(new Model("Models/ClassPH/Assault/assaultPH.obj")); // I wrote them like this because they will all be different models eventually I think.
	//	GameEmGine::addModel(mod.back()); // mod 2
		mod[2]->getTransformer().setRotation({0,270,180}), mod[2]->getTransformer().setScale(10);
		//	mod[2]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Tank/tankPH.obj"));
		//	GameEmGine::addModel(mod.back()); // mod 3
		mod[3]->getTransformer().setRotation({0,270,180}), mod[3]->getTransformer().setScale(10);
		//	mod[3]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Medic/medicPH.obj"));
		//	GameEmGine::addModel(mod.back()); // mod 4
		mod[4]->getTransformer().setRotation({0,270,180}), mod[4]->getTransformer().setScale(10);
		//	mod[4]->setToRender(false);

		mod.push_back(new Model("Models/ClassPH/Specialist/specPH.obj"));
		//	GameEmGine::addModel(mod.back()); // mod 5
		mod[5]->getTransformer().setRotation({0,270,180}), mod[5]->getTransformer().setScale(10);
		//	mod[5]->setToRender(false);


			//TODO: Add back button and more flashy start button and "Press A to ready" buttons
		float extra = 0;
		for(int a = 0; a < 4; a++)
		{
			if(a == 2)
				extra = .7f;
			else if(a == 3)
				extra = 2;
			mod.push_back(new Model(*classes[0]));
			mod[6 + a]->getTransformer().setPosition(float(-42.2 + a * 27.5 + extra), -25, 0), mod[6 + a]->getTransformer().setRotation({0,270,0}), mod[6 + a]->getTransformer().setScale(1, 15, 7);
			mod[6 + a]->setToRender(true);
			GAME::addModel(mod.back()); //6, 7, 8, 9
		}



		//See GDD for general layout of this screen.

		mod[0]->addChild(mod[1]);

		LightSource::setSceneAmbient({0,0,0,255});

		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
		mod[1]->getTransformer().setRotation({90,0,0});
		mod[1]->getTransformer().setScale(15.0f);
		mod[1]->getTransformer().setPosition({mod[0]->getWidth() - mod[1]->getWidth() - 200, -9.f * 1 + 15,0});

		LightSource::setSceneAmbient({0,0,0,255});

		keyPressed = [=](int a, int b) {keyInputPressed(a, b);  };
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if(key == 'B')
		{
			fadeout = true;
		}
	}

	// doing the update for menu screenb
	void updateMenu(double dt)
	{
		static float time = 0;
		time += (float)dt;

		static float flipTime;

		static bool assaultSelected;
		static bool medicSelected;
		static bool specialistSelected;
		static bool tankSelected;

		static bool isConnected[4] = {false,false,false,false};

		static bool menuMoved[] = {false,false,false,false};
		players.resize(4);
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
		float extra = 0;
		static std::vector<Player*>playerSelections
		{
			new Assault("Models/Class/Assault/Idle/ASI1.obj"),new Tank("Models/Class/Assault/Idle/ASI1.obj"),
			new  Medic("Models/Class/Assault/Idle/ASI1.obj"), new Specialist("Models/Class/Assault/Idle/ASI1.obj")
		};

		for(int a = 0; a < 4; a++)
		{
			if(GameEmGine::isControllerConnected(a))
			{
				if(abs(((XinputController*)GameEmGine::getController(a))->getSticks()[LS].x) >= 0.8)
				{
					if((time - flipTime) >= 0.2f)
					{
						if(menuMoved[a] == false)
						{
							flipTime = time;
							option[a] += ((XinputController*)GameEmGine::getController(a))->getSticks()[LS].x < 0 ? 1 : -1;
							option[a] = option[a] > 3 ? 0 : option[a] < 0 ? 3 : option[a];

							lerpParam = 0;
							if(a == 2)
								extra = .7f;
							else if(a == 3)
								extra = 2;
							GameEmGine::removeModel(mod[6 + a]);
							mod[6 + a]->setColour({255,255,255});
							*mod[6 + a] = *classes[option[a]];
							mod[6 + a]->getTransformer().setPosition(float(-42.2 + a * 27.5), -25, 0), mod[6 + a]->getTransformer().setRotation({0,270,0}), mod[6 + a]->getTransformer().setScale(1, 15, 7);
							GameEmGine::addModel(mod[6 + a]);

							//tmp = mod[option]->getTransformer().getScale();
						}
					}
				}

				//else
				if(((XinputController*)GameEmGine::getController(a))->isButtonPressed(CONTROLLER_A))
				{
					//fixthisnow = false;
					switch(option[a])
					{
					case 0:
						if(assaultSelected == false)
						{
							assaultSelected = true;
							players[a] = playerSelections[0];
							mod[6 + a]->setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						break;
					case 1:
						if(tankSelected == false)
						{
							tankSelected = true;
							players[a] = playerSelections[1];
							mod[6 + a]->setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						break;
					case 2:
						if(medicSelected == false)
						{
							medicSelected = true;
							players[a] = playerSelections[2];
							mod[6 + a]->setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						break;
					case 3:
						if(specialistSelected == false)
						{
							specialistSelected = true;
							players[a] = playerSelections[3];
							mod[6 + a]->setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						break;
					default:
						break;
					}
				}
				if(((XinputController*)GameEmGine::getController(a))->isButtonPressed(CONTROLLER_B))
				{
					//fixthisnow = true;
					switch(option[a])
					{
					case 0:
						if(assaultSelected == true)
						{
							assaultSelected = false;
							isConnected[a] = false;
							mod[6 + a]->setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					case 1:
						if(tankSelected == true)
						{
							tankSelected = false;
							isConnected[a] = false;
							mod[6 + a]->setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					case 2:
						if(medicSelected == true)
						{
							medicSelected = false;
							isConnected[a] = false;
							mod[6 + a]->setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					case 3:
						if(specialistSelected == true)
						{
							specialistSelected = false;
							isConnected[a] = false;
							mod[6 + a]->setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					default:
						break;

					}

				}
			}
			else
			{
				isConnected[a] = true;

				int count = 0;
				players[a] = playerSelections[count];



				for(int b = 0; b < 4; b++)
					if(b != a)
						if(players[b])
						{
							if(players[a]->type == players[b]->type)
								players[a] = playerSelections[++count];
						}
			}
			bool next = true;
			for(int j = 0; j < 4; j++)
				if(isConnected[j] != true)
					next = false;
			if(next == true)
			{
				fadeout = true;
			}
		}
		//TODO: Set this to change a picture instead of this
		//mod[option]->getTransformer().setScale(lerp(tmp, Coord3D(12.0f), lerpParam));
		//mod[option]->setColour(lerp(ColourRGBA{ 255,255,255 }, ColourRGBA{ 0,255,255 }, lerpParam));
		//lerpParam += .1f;

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
				Game* game = new Game;
				game->playerTypes(players);
				GameEmGine::setScene(game);
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateMenu(dt);
	}

private:
	Model* classes[4]
	{new Assault("Models/ClassPH/Assault/assaultPH.obj"),new Tank("Models/ClassPH/Tank/tankPH.obj"),
		new  Medic("Models/ClassPH/Medic/medicPH.obj"),new Specialist("Models/ClassPH/Specialist/specPH.obj")};
	std::vector<Player*>players;
	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
	float lerpParam = 1;
	int option[4] = {0,0,0,0};
};