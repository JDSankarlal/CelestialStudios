#pragma once
#include <GameEmGine.h>
#include "Game.h"

class PlayerSelect:public Scene
{
public:
	void onSceneExit() {}

	~PlayerSelect() { /*delete game;*/ }

	void init()
	{
		LightManager::enableShadows(false);

		Text test;

		mod.clear();
		fadein = true;
		fadeout = false;
		splashT = 0;
		splashAmbient = 0;
		lerpParam = 1;
		option[0] =
			option[1] =
			option[2] =
			option[3] = 0;

		LightManager::addLight(&sceneLight);
		sceneLight.setLightType(Light::DIRECTIONAL);
		sceneLight.rotate({-45,0,0});
		sceneLight.setAmbient({0,0,0,255});
		sceneLight.setDiffuse({0,0,0,255});
		sceneLight.setSpecular({0,0,0,255});

		//GameEmGine::m_modelShader->sendUniform("darken", 0);

		Model tmp(new PrimitivePlane(115, 63), "Player Select Scene");
		tmp.replaceTexture(0, 0, ResourceManager::getTexture2D("texture/scenes/playerselect/playerselect.png"));
		mod.push_back(tmp);
		mod.back().scale(16);
		mod.back().setTransparent(true);
		mod.back().setColour(1, 1, 1, 0);


		mod.push_back(Model("Models/Scene/Menu/Start.obj"));
		(*std::next(mod.begin(), 1)).setToRender(false);

		playerSelections.push_back(new Assault("Models/Class/Assault/Idle/ASI1.obj"));
		playerSelections.push_back(new Tank("Models/Class/Tank/Idle/TKI1.obj"));
		playerSelections.push_back(new Medic("Models/Class/Medic/Idle/MCI1.obj"));
		playerSelections.push_back(new Specialist("Models/Class/Specialist/Idle/SCI1.obj"));

		GameEmGine::setCameraType(Camera::ORTHOGRAPHIC);
		GameEmGine::getMainCamera()->translate({0,0,-100});

		//TODO: Add back button and more flashy start button and "Press A to ready" buttons
		float extra = 0;
		for(int a = 0; a < 4; a++)
		{
			if(a == 1)
				extra = .75f;
			else if(a == 2)
				extra = .6f;
			else if(a == 3)
				extra = 0;

			Model tmp2(new PrimitivePlane(16, 28), "Class Icon");
			tmp2.setTransparent(true);
			tmp2.replaceTexture(0, 0, classes[0]);
			mod.push_back(tmp2);
			mod.back().setParent(&*mod.begin());

			//translate(float(-42.2 + a * 27.5 + extra) -20.9f, -1 <- ORIGINAL
			(*std::next(mod.begin(), 2 + a)).translate(-43.5f + a * 16 + a * 13.05f - extra, 2, 0);
			(*std::next(mod.begin(), 2 + a)).setToRender(true);
		}

		for(int a = 0; a < 4; a++)
		{
			if(a == 1)
				extra = .75f;
			else if(a == 2)
				extra = .6f;
			else if(a == 3)
				extra = 0;

			Model tmp2(new PrimitivePlane(16, 10), "Class Description");
			tmp2.replaceTexture(0, 0, classDescription[0]);
			tmp2.setTransparent(true);
			mod.push_back(tmp2); mod.back().setParent(&*mod.begin());

			(*std::next(mod.begin(), 6 + a)).translate(-43.5f + a * 16 + a * 13.05f - extra, -7, 0);
			(*std::next(mod.begin(), 6 + a)).setToRender(true);
		}

		for(auto& a : mod)
			GameEmGine::addModel(&a);

		//start adding in character descriptions OH MY GOT WHY ISNT IT SHOWING UPFD;OIFOIHFDUISSDFJJSDK



		//See GDD for general layout of this screen.

		(*std::next(mod.begin(), 0)).addChild(&(*std::next(mod.begin(), 1)));
		(*std::next(mod.begin(), 0)).scale(16);
		(*std::next(mod.begin(), 1)).rotate({90,0,0});
		(*std::next(mod.begin(), 1)).scale(15.0f);
		(*std::next(mod.begin(), 1)).translate({(*std::next(mod.begin(),0)).getWidth() - (*std::next(mod.begin(),1)).getWidth() - 200, -9.f * 1 + 15,0});


		keyPressed = [&](int a, int b) {keyInputPressed(a, b); };
		mousePressed = [](int  a, int b){a, b; };
		mouseReleased = [](int key, int)
		{

			Model* obj;
			if(key == MOUSE_LEFT_BUTTON)
			{
				obj = GameEmGine::getMouseCollisionObject();
				if(obj)
					obj->enableTexture(false);
			}
		};
	}

	void keyInputPressed(int key, int modfier)
	{
		modfier;
		if(key == 'B')
		{
			fadeout = true;
		}

		if(key == GLFW_KEY_F5)
			Shader::refresh();
	}

	// doing the update for menu screenb
	void updateMenu(double dt)
	{
		static float time = 0;
		time += (float)dt;

		static float flipTime;

		//static bool assaultSelected;
		//static bool medicSelected;
		//static bool specialistSelected;
		//static bool tankSelected;

		static bool isConnected[4] = {false,false,false,false};

		static bool menuMoved[] = {false,false,false,false};

		players.resize(4);

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


		static Coord3D tmp = Coord3D(20.0f);
		float extra = 0;

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

							(*std::next(mod.begin(), 6 + a)).replaceTexture(0, 0, classes[option[a]]);
							lerpParam = 0;
							if(a == 2)
								extra = .7f;
							else if(a == 3)
								extra = 2;

							//GameEmGine::removeModel((*std::next(mod.begin(),2 + a]);
							(*std::next(mod.begin(), 2 + a)).setColour({255,255,255});
							(*std::next(mod.begin(), 2 + a)).replaceTexture(0, 0, classes[option[a]]);

							GameEmGine::addModel(&(*std::next(mod.begin(), 2 + a)));


							//translate(float(-42.2 + a * 27.5 + extra) <- ORIGINAL
		//(*std::next(mod.begin(),6 + a]->translate(float(-42.2 + a * 27.5 + extra), -20.9f, 0), (*std::next(mod.begin(),6 + a]->rotate({ 0,270,0 }), (*std::next(mod.begin(),6 + a]->setScale(1, 15, 7);
						//	GameEmGine::removeModel((*std::next(mod.begin(),6 + a]);
							(*std::next(mod.begin(), 6 + a)).replaceTexture(0, 0, classDescription[option[a]]);
							(*std::next(mod.begin(), 6 + a)).setColour({255,255,255});

							(*std::next(mod.begin(), 6 + a)).setToRender(true);
							GameEmGine::addModel(&(*std::next(mod.begin(), 6 + a))); //6, 7, 8, 9

							//tmp = (*std::next(mod.begin(),option]->getScale();
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
							(*std::next(mod.begin(), 2 + a)).setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						else
						{
							//AudioPlayer::createAudioStream("Audio/playerSelectedAlready.wav");
							//AudioPlayer::play();
						}
						break;
					case 1:
						if(tankSelected == false)
						{
							tankSelected = true;
							players[a] = playerSelections[1];
							(*std::next(mod.begin(), 2 + a)).setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						else
						{
							//AudioPlayer::createAudioStream("Audio/playerSelectedAlready.wav");
							//AudioPlayer::play();
						}
						break;
					case 2:
						if(medicSelected == false)
						{
							medicSelected = true;
							players[a] = playerSelections[2];
							(*std::next(mod.begin(), 2 + a)).setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						else
						{
							//AudioPlayer::createAudioStream("Audio/playerSelectedAlready.wav");
							//AudioPlayer::play();
						}
						break;
					case 3:
						if(specialistSelected == false)
						{
							specialistSelected = true;
							players[a] = playerSelections[3];
							(*std::next(mod.begin(), 2 + a)).setColour(1, 0, 0);
							menuMoved[a] = true;
							isConnected[a] = true;
						}
						else
						{
							//AudioPlayer::createAudioStream("Audio/playerSelectedAlready.wav");
							//AudioPlayer::play();
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
							menuMoved[a] = false;
							(*std::next(mod.begin(), 6 + a)).setColour(1, 1, 1);
						}
						break;
					case 1:
						if(tankSelected == true)
						{
							tankSelected = false;
							isConnected[a] = false;
							(*std::next(mod.begin(), 6 + a)).setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					case 2:
						if(medicSelected == true)
						{
							medicSelected = false;
							isConnected[a] = false;
							(*std::next(mod.begin(), 6 + a)).setColour(1, 1, 1);
							menuMoved[a] = false;
						}
						break;
					case 3:
						if(specialistSelected == true)
						{
							specialistSelected = false;
							isConnected[a] = false;
							(*std::next(mod.begin(), 6 + a)).setColour(1, 1, 1);
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
		//(*std::next(mod.begin(),option]->setScale(lerp(tmp, Coord3D(12.0f), lerpParam));
		//(*std::next(mod.begin(),option]->setColour(lerp(ColourRGBA{ 255,255,255 }, ColourRGBA{ 0,255,255 }, lerpParam));
		//lerpParam += .1f;

		if(lerpParam >= 1)
		{
			lerpParam = 1;
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

				for(int a = 0; a < 4; a++)
				{
					isConnected[a] = false;
					menuMoved[a] = false;
				}

				//GamePlayInit();
				static Game game;
				game.playerTypes(players);
				tankSelected = false;
				medicSelected = false;
				assaultSelected = false;
				specialistSelected = false;
				GameEmGine::setScene(&game);
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
	std::vector<Player*>playerSelections;


	Texture2D classes[4]
	{ResourceManager::getTexture2D("texture/scenes/playerselect/Assault/assaultClassArt.png"), ResourceManager::getTexture2D("texture/scenes/playerselect/Tank/tankClassArt.png"),
		  ResourceManager::getTexture2D("texture/scenes/playerselect/Medic/medicClassArt.png"), ResourceManager::getTexture2D("texture/scenes/playerselect/Specialist/SpecialistClassArt.png")};

	Texture2D classDescription[4]
	{ResourceManager::getTexture2D("texture/scenes/playerselect/Assault/assaultDescription.png"),  ResourceManager::getTexture2D("texture/scenes/playerselect/Tank/tankDescription.png"),
		  ResourceManager::getTexture2D("texture/scenes/playerselect/Medic/medicDescription.png"),  ResourceManager::getTexture2D("texture/scenes/playerselect/Specialist/specialistDescription.png")};

	std::vector<Player*>players;
	std::list<Model> mod;
	Light sceneLight;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
	float lerpParam = 1;
	int option[4] = {0,0,0,0};
	bool assaultSelected;
	bool medicSelected;
	bool specialistSelected;
	bool tankSelected;

};