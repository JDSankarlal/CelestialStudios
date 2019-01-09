#pragma once
#include <GameEmGine.h>
#include "Player.h"
#include "Boss.h"

class Game:public Scene
{
public:
	/// - Collision Class - ///
	bool collisions(Model *l, Model *k)
	{
		//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
		Coord3D thing = l->getCenter() - k->getCenter();

		float distanceX = abs(thing.x);
		float distanceZ = abs(thing.z);

		float capW = (l->getWidth() + k->getWidth()) / 2;
		float capD = (l->getDepth() + k->getDepth()) / 2;

		if(std::abs(distanceX) <= capW)
			if(std::abs(distanceZ) <= capD)
				return true;

		return false;
	}

	bool collisionsB(Model *l, Model *k)
	{
		//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
		Coord3D thing = l->getCenter() - k->getCenter();

		float distanceX = abs(thing.x);
		float distanceZ = abs(thing.z);

		float capW = (l->getWidth() + k->getWidth()) / 2;
		float capD = (l->getDepth() + k->getDepth()) / 2;

		if(std::abs(distanceX) <= capW + 4)
			if(std::abs(distanceZ) <= capD + 4)
				return true;

		return false;
	}

	/// - 3D Collision Function - ///
	bool collisions3D(Model *l, Model *k)
	{
		//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
		Coord3D thing = l->getCenter() - k->getCenter();

		float distanceX = abs(thing.x);
		float distanceY = abs(thing.y);
		float distanceZ = abs(thing.z);

		float capW = (l->getWidth() + k->getWidth()) / 2;
		float capH = (l->getHeight() + k->getHeight()) / 2;
		float capD = (l->getDepth() + k->getDepth()) / 2;

		if(std::abs(distanceX) <= abs(capW))
			if(std::abs(distanceZ) <= abs(capD))
				if(std::abs(distanceY) <= abs(capH))
					return true;

		return false;
	}
	
	//instance key is pressed
	static void keyInputPressed(int key, int modifier)
	{
		modifier;
		m_left = (key == 'A' ? true : m_left);
		m_right = (key == 'D' ? true : m_right);
		m_up = (key == 'E' ? true : m_up);
		m_down = (key == 'Q' ? true : m_down);
		m_in = (key == 'W' ? true : m_in);
		m_out = (key == 'S' ? true : m_out);

		rotLeft = (key == GLFW_KEY_LEFT ? true : rotLeft);
		rotRight = (key == GLFW_KEY_RIGHT ? true : rotRight);
		rotUp = (key == GLFW_KEY_UP ? true : rotUp);
		rotDown = (key == GLFW_KEY_DOWN ? true : rotDown);

		printf("key PRESSED code: %d\n\n", key);
	}

	//instance key is released
	static void keyInputReleased(int key, int modifier)
	{
		modifier;
		m_left = (key == 'A' ? false : m_left);
		m_right = (key == 'D' ? false : m_right);
		m_in = (key == 'W' ? false : m_in);
		m_out = (key == 'S' ? false : m_out);
		m_up = (key == 'E' ? false : m_up);
		m_down = (key == 'Q' ? false : m_down);

		rotLeft = (key == GLFW_KEY_LEFT ? false : rotLeft);
		rotRight = (key == GLFW_KEY_RIGHT ? false : rotRight);
		rotUp = (key == GLFW_KEY_UP ? false : rotUp);
		rotDown = (key == GLFW_KEY_DOWN ? false : rotDown);

		//changes fps limit
		if(key == GLFW_KEY_KP_6)
			game.setFPSLimit(game.getFPSLimit() + 1);
		if(key == GLFW_KEY_KP_4)
			game.setFPSLimit(game.getFPSLimit() - 1);

		if(key == GLFW_KEY_F) //Toggles Fullscreen
		{
			static bool full;
			game.getWindow()->setFullScreen(full = !full);
			printf("Full Screen: %s\n", full ? "true" : "false");
		}

		if(key == GLFW_KEY_SPACE) //changes the model that is being moved
		{
			static CAMERA_TYPE type = PERSPECTIVE;
			game.setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
		}

		if(key == GLFW_KEY_TAB)
			movePlayer = !movePlayer;

		if(key == GLFW_KEY_F5) //resets the camera
		{
			GameEmGine::m_modelShader->refresh();

			//game.setCameraAngle(0, { 1, 1, 1 });
			//	game.setCameraPosition({0,0,0});
		}

		if(key == 'R')
			game.setCameraAngle(0, {1,1,1});

		printf("key RELEASED code: %d\n\n", key);
	}

	static void mouseButtonReleased(int button, int _mod)
	{
		_mod;
		if(button == LEFT_BUTTON)
			leftM = InputManager::getMouseCursorPosition();
		if(button == RIGHT_BUTTON)
			rightM = InputManager::getMouseCursorPosition();
	}

	// Set game screen
	void GamePlayInit()
	{

		game.setCameraType(PERSPECTIVE);
		game.setFPSLimit(60);
		/// - Load mod into Scene - ///

		//Players
		mod.push_back(new Player("Models/AssaultModel/Idle/ACM1.obj"));
		game.addModel(mod.back());//0
		mod.push_back(new Player("Models/AssaultModel/Idle/ACM1.obj"));
		game.addModel(mod.back());//1
		mod.push_back(new Player("Models/AssaultModel/Idle/ACM1.obj"));
		game.addModel(mod.back());//2
		mod.push_back(new Player("Models/AssaultModel/Idle/ACM1.obj"));
		game.addModel(mod.back());//3

		static Animation walk[4], idle[4];

		for(int a = 0; a < 4; a++)
		{
			walk[a].addDir("Models/AssaultModel/Walk/");
			idle[a].addDir("Models/AssaultModel/Idle/");
			mod[a]->addAnimation("walk", &walk[a]);
			mod[a]->addAnimation("idle", &idle[a]);
			mod[a]->setAnimation("walk");

			walk[a].setAnimationSpeed(.25);
			walk[a].repeat(true);
			idle[a].stop();


			mod[a]->addAnimation("walk", &walk[a]);
			mod[a]->addAnimation("idle", &idle[a]);
			mod[a]->setAnimation("walk");
		}


		//Building 1s
		mod.push_back(new Model("Models/Buildings/Building1/building1.obj"));
		game.addModel(mod.back());//4
		mod.push_back(new Model(*mod[4]));
		game.addModel(mod.back());//5
		mod.push_back(new Model(*mod[4]));
		game.addModel(mod.back());//6

		//Project Nebula Sign
		mod.push_back(new Model("Models/Neon Signs/Project Nebula/signn.obj"));
		game.addModel(mod.back()); //7

		//Boss
		mod.push_back(new Boss("Models/BOSS/BOSS.obj"));
		game.addModel(mod.back()); //8
		//mod[8]->enableBoundingBox(true);

		//Floor
		mod.push_back(new Model("Models/Floor/Floor.obj"));
		game.addModel(mod.back()); //9

		//Light Posts
		mod.push_back(new Model("Models/Lamp/lampPost.obj"));
		game.addModel(mod.back()); //10
		mod.push_back(new Model(*mod[10]));
		game.addModel(mod.back());//11
		mod.push_back(new Model(*mod[10]));
		game.addModel(mod.back());//12
		mod.push_back(new Model(*mod[10]));
		game.addModel(mod.back());//13
		mod.push_back(new Model(*mod[10]));
		game.addModel(mod.back());//14
		mod.push_back(new Model(*mod[10]));
		game.addModel(mod.back());//15

		//Bench
		mod.push_back(new Model("Models/Bench/Bench.obj"));
		game.addModel(mod.back()); //16
		mod.push_back(new Model(*mod[16]));
		game.addModel(mod.back());//17

		//Planet
		mod.push_back(new Model("Models/Planet/planet.obj"));
		game.addModel(mod.back()); //18

		//Building 2s
		mod.push_back(new Model("Models/Buildings/Building2/building2.obj"));
		game.addModel(mod.back());//19
		mod.push_back(new Model(*mod[19]));
		game.addModel(mod.back());//20
		mod.push_back(new Model(*mod[19]));
		game.addModel(mod.back());//21

		//GraveStones
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj")); //22
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//23
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//24
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//25

		//Coloured ring "IDs"
		mod.push_back(new Model("Models/ID/Identifier.obj"));//26
		mod.push_back(new Model(*mod.back()));//27
		mod.push_back(new Model(*mod.back()));//28
		mod.push_back(new Model(*mod.back()));//29

		//Building 3s
		mod.push_back(new Model("Models/Buildings/Building3/House.obj"));
		game.addModel(mod.back());//30
		mod.push_back(new Model(*mod[30]));
		game.addModel(mod.back());//31
		mod.push_back(new Model(*mod[30]));
		game.addModel(mod.back());//32

		//Building 4s
		mod.push_back(new Model("Models/Buildings/Building4/tallBuilding.obj"));
		game.addModel(mod.back());//33
		mod.push_back(new Model(*mod[33]));
		game.addModel(mod.back());//34

		mod.push_back(new Model("Models/DiedTree/tree.obj"));
		game.addModel(mod.back());//35
		mod.push_back(new Model(*mod[35]));
		game.addModel(mod.back());//36
		mod.push_back(new Model(*mod[35]));
		game.addModel(mod.back());//37
		mod.push_back(new Model(*mod[35]));
		game.addModel(mod.back());//38

		mod.push_back(new Model("Models/Buildings/Building5/smallShop.obj"));
		game.addModel(mod.back());//39
		mod.push_back(new Model(*mod[39]));
		game.addModel(mod.back());//40
		mod.push_back(new Model(*mod[39]));
		game.addModel(mod.back());//41

		mod.push_back(new Model("Models/Buildings/Building6/Building6.obj"));
		game.addModel(mod.back());//42
		mod.push_back(new Model(*mod[42]));
		game.addModel(mod.back());//43

		mod.push_back(new Model("Models/Missile/BossMissile.obj"));
		game.addModel(mod.back()); //44
		mod.push_back(new Model(*mod[44]));
		game.addModel(mod.back());//45
		mod.push_back(new Model(*mod[44]));
		game.addModel(mod.back());//46
		mod.push_back(new Model(*mod[44]));
		game.addModel(mod.back());//47

		mod.push_back(new Model("Models/Bullet/bullet.obj"));//48

		mod.push_back(new Model("Models/Trash/TrashCan.obj"));
		game.addModel(mod.back()); //49
		mod.push_back(new Model(*mod[49]));
		game.addModel(mod.back());//50

		mod.push_back(new Model("Models/Picnic/PicnicTable.obj"));
		game.addModel(mod.back()); //51
		mod.push_back(new Model(*mod[51]));
		game.addModel(mod.back());//52

		mod.push_back(new Model("Models/PizzaSign/PIZZA.obj"));
		game.addModel(mod.back()); //53

		mod.push_back(new Model("Models/AssaultModel/Weapon/AssaultClassGun.obj"));
		game.addModel(mod.back()); //54
		mod.push_back(new Model(*mod[54]));
		game.addModel(mod.back()); //55
		mod.push_back(new Model(*mod[54]));
		game.addModel(mod.back()); //56
		mod.push_back(new Model(*mod[54]));
		game.addModel(mod.back()); //57

		mod.push_back(new Model("Models/Planet/Planet2/planet.obj"));
		game.addModel(mod.back()); //58
		//Pause Menu
		//mod.push_back(new Model("Models/Pause Menu/Pause Menu.obj"));//33

		//collision floor
		mod.push_back(new Model("Models/Floor/Floor2.obj"));//59
		game.addModel(mod[59]);

		mod[59]->setToRender(false);
		mod[59]->getTransformer().setScale(1.f, 1.0f, 1.5f), mod[59]->getTransformer().setPosition(0.0f, 0.15f, 5.0f);

		//missile hit box
		mod.push_back(new Model(*mod[44]));//60

		game.addModel(mod.back()); //44
		mod[60]->setToRender(false);
		mod[60]->getTransformer().setScale(6, 1, 1);
		mod.push_back(new Model(*mod[60]));//61
		game.addModel(mod.back());//45
		mod.push_back(new Model(*mod[60]));//62
		game.addModel(mod.back());//46
		mod.push_back(new Model(*mod[60]));//63
		game.addModel(mod.back());//47


		//mod[61]->setToRender(false);
		//mod[61]->getTransformer().setScale(2);
		//mod[62]->setToRender(false);
		//mod[62]->getTransformer().setScale(2);
		//mod[63]->setToRender(false);
		//mod[63]->getTransformer().setScale(2);

		mod[44]->addChild(mod[60]);
		mod[45]->addChild(mod[61]);
		mod[46]->addChild(mod[62]);
		mod[47]->addChild(mod[63]);


		/// - Set Model Transforms - ///
		//Player Transforms
		mod[0]->getTransformer().setScale(1.2f), mod[0]->getTransformer().setPosition(1.0f, 0.0f, -5.0f);
		mod[1]->getTransformer().setScale(1.2f), mod[1]->getTransformer().setPosition(-1.0f, 0.0f, -5.0f);
		mod[2]->getTransformer().setScale(1.2f), mod[2]->getTransformer().setPosition(2.0f, 0.0f, -5.0f);
		mod[3]->getTransformer().setScale(1.2f), mod[3]->getTransformer().setPosition(-2.0f, 0.0f, -5.0f);
		mod[0]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[1]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[2]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[3]->getTransformer().setRotation(Coord3D(0, 180, 0));


		//Building Transforms
		//Building 1s
		mod[4]->getTransformer().setScale(2), mod[4]->getTransformer().setPosition(-16.75f, 0.0f, -2.0f);
		mod[5]->getTransformer().setScale(2), mod[5]->getTransformer().setPosition(16.75f, 0.0f, 10.0f), mod[5]->getTransformer().setRotation({0.0f,180.0f,0.0f});
		mod[6]->getTransformer().setScale(2), mod[6]->getTransformer().setPosition(-4.0f, 0.0f, 22.75f), mod[6]->getTransformer().setRotation({0.0f,-90.0f,0.0f});

		//Building 2s
		mod[19]->getTransformer().setScale(1.75f), mod[19]->getTransformer().setPosition(-16.4f, 0.0f, 3.0f);
		mod[20]->getTransformer().setScale(1.75f), mod[20]->getTransformer().setPosition(16.4f, 0.0f, 1.2f), mod[20]->getTransformer().setRotation({0.0f, 180.0f, 0.0f});;
		mod[21]->getTransformer().setScale(1.75f), mod[21]->getTransformer().setPosition(13.5f, 0.0f, 22.4f), mod[21]->getTransformer().setRotation({0.0f, -90.0f, 0.0f});
		//Buildings 3s
		mod[30]->getTransformer().setPosition(7.25f, 0.0f, 23.6f);
		mod[31]->getTransformer().setPosition(19.5f, 0.0f, 5.35f), mod[31]->getTransformer().setRotation({0,180,0});
		mod[32]->getTransformer().setPosition(-8.6f, 0.0f, 25.35f), mod[32]->getTransformer().setRotation({0,-90,0});
		//Building 4s
		mod[33]->getTransformer().setPosition(18.25f, 0.0f, 18.5f), mod[33]->getTransformer().setRotation({0,-90,0});
		mod[34]->getTransformer().setPosition(-18.25f, 0.0f, 8.25), mod[34]->getTransformer().setRotation({0,90,0});
		//Building 5s
		mod[39]->getTransformer().setScale(1.0f, 1.0f, 1.05f), mod[39]->getTransformer().setPosition(19.6f, 0.0f, 14.25f), mod[39]->getTransformer().setRotation({0,180,0});
		mod[40]->getTransformer().setScale(1.25f, 1.0f, 1.0f), mod[40]->getTransformer().setPosition(-16.9f, 0.0f, 16.35f), mod[40]->getTransformer().setRotation({0,90,0});
		mod[41]->getTransformer().setScale(1.0f, 1.3f, 1.6f), mod[41]->getTransformer().setPosition(1.0f, 0.0f, 25.5f), mod[41]->getTransformer().setRotation({0,-90,0});
		//Building 6s
		mod[42]->getTransformer().setScale(1.0f, 1.5f, 1.8f), mod[42]->getTransformer().setPosition(-14.2f, 0.0f, 22.9f), mod[42]->getTransformer().setRotation({0,-90,0});
		mod[43]->getTransformer().setPosition(16.65f, 0.0f, -3.15f), mod[43]->getTransformer().setRotation({0,180,0});

		//Project Nebula Sign Transforms
		mod[7]->getTransformer().setScale(3), mod[7]->getTransformer().setPosition(7.5f, 5.34f, 22.5f);

		//Boss Trarrnsforms
		mod[8]->getTransformer().setScale(6), mod[8]->getTransformer().setPosition(0.0f, 0.0f, 16.0f), mod[8]->getTransformer().setRotation({0.0f, 180.0f, 0.0f});

		//Floor Transforms
		mod[9]->getTransformer().setScale(2.0f, 1.0f, 1.75f), mod[9]->getTransformer().setPosition(0.0f, 0.0f, 5.0f);

		//Street Light Transforms
		mod[10]->getTransformer().setScale(0.5f), mod[10]->getTransformer().setPosition(13.0f, 0.0f, -1.0f);
		mod[11]->getTransformer().setScale(0.5f), mod[11]->getTransformer().setPosition(13.0f, 0.0f, 7.0f);
		mod[12]->getTransformer().setScale(0.5f), mod[12]->getTransformer().setPosition(13.0f, 0.0f, 15.0f);
		mod[13]->getTransformer().setScale(0.5f), mod[13]->getTransformer().setPosition(-13.0f, 0.0f, -1.0f), mod[13]->getTransformer().setRotation({0.0f,180.0f,0.0f});
		mod[14]->getTransformer().setScale(0.5f), mod[14]->getTransformer().setPosition(-13.0f, 0.0f, 7.0f), mod[14]->getTransformer().setRotation({0.0f,180.0f,0.0f});
		mod[15]->getTransformer().setScale(0.5f), mod[15]->getTransformer().setPosition(-13.0f, 0.0f, 15.0f), mod[15]->getTransformer().setRotation({0.0f,180.0f,0.0f});

		//Bench Transforms
		mod[16]->getTransformer().setPosition(-13.0f, 0.0f, 3.0f);
		mod[17]->getTransformer().setPosition(13.0f, 0.0f, 3.0f), mod[17]->getTransformer().setRotation({0.0f,180.0f,0.0f});

		//Planet Transforms
		mod[18]->getTransformer().setPosition(9.0f, 12.0f, 36.0f);
		mod[58]->getTransformer().setPosition(-10.0f, 11.0f, 25.0f);

		//ID rings?
		mod[26]->setColour({255,0,0});
		mod[26]->getTransformer().setScale(0.65f), mod[26]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[26]->getTransformer().setRotation({0,-90,0});

		mod[27]->setColour({0,0,255});
		mod[27]->getTransformer().setScale(0.65f), mod[27]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[27]->getTransformer().setRotation({0,-90,0});

		mod[28]->setColour({0,255,0});
		mod[28]->getTransformer().setScale(0.65f), mod[28]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[28]->getTransformer().setRotation({0,-90,0});

		mod[29]->setColour({255,255,0});
		mod[29]->getTransformer().setScale(0.65f), mod[29]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[29]->getTransformer().setRotation({0,-90,0});

		//Trees
		mod[35]->getTransformer().setScale(0.3f), mod[35]->getTransformer().setPosition(13.0f, 0.0f, -3.0f), mod[35]->getTransformer().setRotation({0,-0,0});
		mod[36]->getTransformer().setScale(0.3f), mod[36]->getTransformer().setPosition(-13.0f, 0.0f, -3.0f), mod[36]->getTransformer().setRotation({0,-0,0});
		mod[37]->getTransformer().setScale(0.3f), mod[37]->getTransformer().setPosition(13.0f, 0.0f, 11.0f), mod[37]->getTransformer().setRotation({0,-0,0});
		mod[38]->getTransformer().setScale(0.3f), mod[38]->getTransformer().setPosition(-13.0f, 0.0f, 11.0f), mod[38]->getTransformer().setRotation({0,-0,0});

		//Missiles
		mod[44]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[45]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[46]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[47]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);

		//Trash
		mod[49]->getTransformer().setScale(1.0f, 1.3f, 1.0f), mod[49]->getTransformer().setPosition(-13.25f, 0.0f, 13.0f);
		mod[50]->getTransformer().setScale(1.0f, 1.3f, 1.0f), mod[50]->getTransformer().setPosition(13.25f, 0.0f, 0.5f);

		//Picnic table wut, cyberpunk game?
		mod[51]->getTransformer().setScale(1.0f, 1.0f, 1.5f), mod[51]->getTransformer().setPosition(-11.0f, 0.0f, 19.0f), mod[51]->getTransformer().setRotation({0.0f,90.0f,0.0f});
		mod[52]->getTransformer().setScale(1.0f, 1.0f, 1.5f), mod[52]->getTransformer().setPosition(-6.75f, 0.0f, 19.0f), mod[52]->getTransformer().setRotation({0.0f,90.0f,0.0f});

		//Pizza Sign
		mod[53]->getTransformer().setScale(1.5f), mod[53]->getTransformer().setPosition(-9.5f, 5.4f, 22.3f);

		//Assault Weapons
		mod[54]->getTransformer().setScale(0.075f), mod[54]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[51]->getTransformer().setRotation({0.0f,0.0f,0.0f});
		mod[55]->getTransformer().setScale(0.075f), mod[55]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[55]->getTransformer().setRotation({0.0f,0.0f,0.0f});
		mod[56]->getTransformer().setScale(0.075f), mod[56]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[56]->getTransformer().setRotation({0.0f,0.0f,0.0f});
		mod[57]->getTransformer().setScale(0.075f), mod[57]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[57]->getTransformer().setRotation({0.0f,0.0f,0.0f});

		/// - Set Model Colour - ///
		//Players
		mod[0]->setColour(1, 0.5, 0.5);
		mod[1]->setColour(0.5, 0.5, 1);
		mod[2]->setColour(0.5, 1, 0.5);
		mod[3]->setColour(1, 1, 0.5);
		mod[0]->addChild(mod[26]);
		mod[1]->addChild(mod[27]);
		mod[2]->addChild(mod[28]);
		mod[3]->addChild(mod[29]);
		mod[0]->addChild(mod[54]);
		mod[1]->addChild(mod[55]);
		mod[2]->addChild(mod[56]);
		mod[3]->addChild(mod[57]);

		LightSource::setLightAmount(14);
		for(int a = 0; a < 6; a++)
		{
			//mod[10 + a]->boundingBoxUpdate();
			LightSource::setLightType(LIGHT_TYPE::DIRECTIONAL, a);
			LightSource::setParent(mod[10 + a], a);
			LightSource::setPosition({-5.0f,4.5,0.0f}, a);
			LightSource::setDirection({0.0f,-1.0f,0.0f}, a);

		}

		LightSource::setLightType(LIGHT_TYPE::POINT, 6);
		LightSource::setParent(mod[44], 6);
		LightSource::setDiffuse({255,100,0,100}, 6);
		LightSource::setAttenuationQuadratic(0.06f, 6);

		LightSource::setLightType(LIGHT_TYPE::POINT, 7);
		LightSource::setParent(mod[0], 7);
		LightSource::setPosition({0, -0.75f, 0}, 7);
		LightSource::setDiffuse({255,0,0,100}, 7);
		LightSource::setAttenuationQuadratic(1.f, 7);

		LightSource::setLightType(LIGHT_TYPE::POINT, 8);
		LightSource::setParent(mod[1], 8);
		LightSource::setPosition({0, -0.75f, 0}, 8);
		LightSource::setDiffuse({0,0,255,100}, 8);
		LightSource::setAttenuationQuadratic(1.f, 8);

		LightSource::setLightType(LIGHT_TYPE::POINT, 9);
		LightSource::setParent(mod[2], 9);
		LightSource::setPosition({0, -0.75f, 0}, 9);
		LightSource::setDiffuse({0,255,0,100}, 9);
		LightSource::setAttenuationQuadratic(1.f, 9);

		LightSource::setLightType(LIGHT_TYPE::POINT, 10);
		LightSource::setParent(mod[3], 10);
		LightSource::setPosition({0, -0.75f, 0}, 10);
		LightSource::setDiffuse({255,255,0,100}, 10);
		LightSource::setAttenuationQuadratic(1.f, 10);

		LightSource::setLightType(LIGHT_TYPE::POINT, 11);
		LightSource::setParent(mod[45], 11);
		LightSource::setDiffuse({255,100,0,100}, 11);
		LightSource::setAttenuationQuadratic(0.06f, 11);

		LightSource::setLightType(LIGHT_TYPE::POINT, 12);
		LightSource::setParent(mod[46], 12);
		LightSource::setDiffuse({255,100,0,100}, 12);
		LightSource::setAttenuationQuadratic(0.06f, 12);

		LightSource::setLightType(LIGHT_TYPE::POINT, 13);
		LightSource::setParent(mod[47], 13);
		LightSource::setDiffuse({255,100,0,100}, 13);
		LightSource::setAttenuationQuadratic(0.06f, 13);

		LightSource::setSceneAmbient({255,255,255,255});

		/// - Set Camera - ///

		game.setCameraPosition({0,15,-10});
		game.setCameraAngle(-45, {1,0,0});

		/// not needed ///
		keyPressed = (keyInputPressed);
		keyReleased = (keyInputReleased);
		mouseReleased = (mouseButtonReleased);


		audio.createAudioStream("Game Jam(Full).wav");

		audio.play(true);
	}

	//updates within game loop
	void update(double dt)
	{
		menu = false;
		static float  time = 0;
		time += (float)dt;
		//static float coolDown = 0;
		static float duration = 0;
		static bool f = true;
		static bool bossActive = true;

		float move = .1f;

		static float pointSize = 50.0f;
		//printf("%f\n", dt);
		static Player* player;
		static Boss*CandyMan = (Boss*)mod[8];
		//drawHealth(CandyMan->getHealth());

		static vector<float> timer[4];
		static vector<Model*> pMissiles[4];
		static vector<Coord3D> missileVelocity[4];
		static vector<Model*> bullets[4];
		static vector<Coord3D> velocity[4];
		static bool makeShitLessCancer[4], makeShitLessCancer2[4];//stops the creation of bullets when trigger is healed down
		static float  curveroni[4] = {0 ,0,0,0};
		static bool hasTarget[4] = {0 ,0,0,0};

		static bool dead[4];

		static float lastTime = (float)clock() / CLOCKS_PER_SEC;

		static Animation squash[4];

		static bool init = false;

		static float angle[4] = {180,180,180,180};



		for(int a = 0; a < 4; a++)
		{
			if(!dead[a])
			{
				curveroni[a] += .01f;
				if(curveroni[a] >= 1)
				{
					hasTarget[a] = false;
				}
				curveroni[a] = fmodf(curveroni[a], 1);

				static Coord3D bossTarget[4];

				//gets a   target for missile (player 1,2,3 or 4) randomly
				if(!hasTarget[a])
				{

					bossTarget[a] = mod[a]->getTransformer().getPosition();

					hasTarget[a] = true;
				}

				if(mod[8])
					if(hasTarget[a])
					{
						Coord3D
							p1[4],
							p2[4],
							c1[4],
							c2[4];
						Coord3D cat[4];
						Coord3D  pointPosition[4];
						p1[a] = mod[8]->getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 2.0f),//start point
							p2[a] = bossTarget[a],//end point 
							c1[a] = p1[a] - Coord3D{0,100,100},//control point
							c2[a] = p2[a] - Coord3D{0,150,100};//control point

						cat[a] = catmull
						(
							c1[a],
							p1[a],
							p2[a],
							c2[a],
							curveroni[a]
						);
						pointPosition[a] = cat[a];
						mod[44 + a]->getTransformer().setPosition(pointPosition[a].x, pointPosition[a].y, pointPosition[a].z);
					}
			}
		}

		if(!init)
		{

			for(int a = 0; a < 4; a++)
			{
				squash[a].addDir("Models/RIP/Rip Ani/");
				squash[a].setAnimationSpeed(.2f);
				//	squash[a].repeat(true);
			}

		}

		if(init)
			if(movePlayer)
				for(int a = 0; a < 4; a++)
				{
					if(game.isControllerConnected(a))
					{
						if(!dead[a])
						{
							player = (Player*)mod[a];
							Xinput p1 = game.getController(a);

							if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
							{

								angle[a] = acosf(p1.Coord2D_sticks[RS].x /
									sqrtf(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
										+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
								angle[a] += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
								angle[a] = fmodf(angle[a], 360);
							}

							/// - Missile Collisions with Player - ///
							for(int b = 0; b < 4; b++)
							{
								bool collision = collisions3D(player, mod[60 + b]);
								if(collision)
								{
									curveroni[a] = 1;
									mod[44 + b]->getTransformer().setPosition(mod[8]->getCenter());
									player->setHealth(player->getHealth() - 35);
									//Coord3D test = ;

								}
							}
							//Player comes near Boss
							if(collisionsB(player, CandyMan))
							{
								player->getTransformer().setPosition(player->getTransformer().getPosition().x, player->getTransformer().getPosition().y, player->getTransformer().getPosition().z - 15);
								player->setHealth(player->getHealth() - 35);
							}
							if(player->getHealth() <= 0)
							{
								dead[a] = true;
								mod[22 + a]->setColour(player->getColour());
								mod[22 + a]->getTransformer().setScale(0.75f * 2, 1 * 2, 0.5 * 2), mod[22 + a]->getTransformer().setPosition(player->getTransformer().getPosition()), mod[22 + a]->getTransformer().setRotation({0.0f,270.0f,0.0f});
								game.addModel(mod[22 + a]);
								mod[22 + a]->addAnimation("squash", &squash[a]);

								mod[22 + a]->setAnimation("squash");
								game.removeModel(player);
							}
							if(p1.triggers[RT] >= .95 && !makeShitLessCancer[a])
							{
								if(player->getBulletCount() > 0)
								{
									makeShitLessCancer[a] = true;

									bullets[a].push_back(nullptr);
									game.addModel(bullets[a].back() = new Model(*mod[48]));
									bullets[a].back()->getTransformer().reset();
									bullets[a].back()->setColour(player->getColour());
									Coord3D pos = mod[a]->getTransformer().getPosition();
									bullets[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
									bullets[a].back()->getTransformer().setScale(0.13f);

									bullets[a].back()->getTransformer().setRotation({90 , angle[a] ,0});


									float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
									float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

									velocity[a].push_back(Coord3D());
									velocity[a].back() = Coord3D(cosVal * move * 3, 0, sinVal * move * 3);

									timer[a].push_back(0);
									audio.createAudioStream("pew.wav");
									audio.play();
									player->setBulletCount(player->getBulletCount() - 1);
								}
							}
							else if(p1.triggers[RT] < .95 && makeShitLessCancer[a])
								makeShitLessCancer[a] = false;

							/// - Button Presses on controller - ///
							//Start button quits game
							if(p1.buttonPressed(p1.buttons.START))
							{
								puts("\nExiting Game\n");
								game.exit();
							}
							if(p1.buttonPressed(p1.buttons.X))
							{
								player->setBulletCount(30);
								puts("RELOADING!!!\n");
							}
							if(p1.buttonPressed(p1.buttons.Y))
							{
								if(time - player->getTimeSinceLastMissile() >= 3)
								{
									pMissiles[a].push_back(nullptr);
									game.addModel(pMissiles[a].back() = new Model(*mod[44]));
									pMissiles[a].back()->getTransformer().reset();
									pMissiles[a].back()->setColour(player->getColour());
									Coord3D pos = mod[a]->getTransformer().getPosition();
									pMissiles[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
									pMissiles[a].back()->getTransformer().setScale(0.4f);

									pMissiles[a].back()->getTransformer().setRotation({0 , angle[a] ,0});

									float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
									float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

									missileVelocity[a].push_back(Coord3D());
									missileVelocity[a].back() = Coord3D(cosVal * move * 6, 0, sinVal * move * 6);
									player->setTimeSinceLastMissile(time);

									timer[a].push_back(0);
									//audio.createStream("pew.wav");
									//audio.play();
									puts("SPECIAL ABILITY\n");
								}
							}

							/// - Left Trigger to Dash - ///

							if(p1.triggers[LT] >= .95)
							{
								static float coolDown[4];

								//get deltaTime put into duraction variable

								if(time - coolDown[a] >= 3)
								{
									if(f == true)
									{
										duration = time;
										f = false;
									}
									move = 0.5f;
									if(time - 0.1f >= duration)
									{
										move = 0.1f;
										//If triggers up then coolDown = time;
										coolDown[a] = time;
										f = true;
									}
								}

							}
							else//Do the same with the LT button, have it so will only work every X seconds.
							{
								move -= .001f;
								if(move <= .1f)
									move = .1f;
								//f = false;
								makeShitLessCancer2[a] = false;
							}

							mod[a]->getTransformer().setRotation({0,angle[a], 0});
							mod[a]->getTransformer().translateBy(p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move); //move player
							float speed = p1.Coord2D_sticks[LS].x*p1.Coord2D_sticks[LS].x + p1.Coord2D_sticks[LS].y*p1.Coord2D_sticks[LS].y;

							mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);

							if(!collisions(mod[a], mod[59]))
								mod[a]->getTransformer().setPosition(abs(mod[a]->getTransformer().getPosition().x) > mod[59]->getWidth() / 2 ? mod[a]->getTransformer().getPosition().x < 0 ? -mod[59]->getWidth() / 2 : mod[59]->getWidth() / 2 : mod[a]->getTransformer().getPosition().x,
									0,
									abs(mod[a]->getTransformer().getPosition().z) > mod[59]->getDepth() / 2 ? mod[a]->getTransformer().getPosition().z < 0 ? -mod[59]->getDepth() / 2 : mod[59]->getDepth() / 2 : mod[a]->getTransformer().getPosition().z);

							if(!speed)
								mod[a]->getAnimation("walk")->pause();
							else
							{
								mod[a]->getAnimation("walk")->play();
								mod[a]->setAnimation("walk");
								mod[a]->getAnimation("walk")->setAnimationSpeed(.25f / speed);
							}
						}
						/// - Bullet Collisions - ///
						for(unsigned b = 0; b < bullets[a].size(); b++)
							if(bullets[a][b])
							{
								timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
								bullets[a][b]->getTransformer().translateBy(velocity[a][b].x, velocity[a][b].y, velocity[a][b].z);

								if(timer[a][b] >= 1)
								{
									game.removeModel(bullets[a][b]);
									bullets[a].erase(bullets[a].begin() + b);
									velocity[a].erase(velocity[a].begin() + b);
									timer[a].erase(timer[a].begin() + b);
									break;
								}

								if(mod[8])
									if(collisions(bullets[a][b], mod[8]))
									{
										game.removeModel(bullets[a][b]);
										bullets[a].erase(bullets[a].begin() + b);
										velocity[a].erase(velocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										//Boss a.k.a model 8, is now called CandyMan for teh purposes of functions.
										CandyMan->setHealth(CandyMan->getHealth() - 10);// When hit takes damage
										if(CandyMan->getHealth() <= 0)
										{
											game.removeModel(CandyMan); // If health = 0 then boss dead
											//	mod[8] = nullptr;
											bossActive = false;
											puts("Killed The BOSS\n");
										}
										puts("Hit The BOSS\n");
										break;
									}
							}
						for(unsigned b = 0; b < pMissiles[a].size(); b++)
							if(pMissiles[a][b])
							{
								timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
								pMissiles[a][b]->getTransformer().translateBy(missileVelocity[a][b].x, missileVelocity[a][b].y, missileVelocity[a][b].z);

								if(timer[a][b] >= 1)
								{
									game.removeModel(pMissiles[a][b]);
									pMissiles[a].erase(pMissiles[a].begin() + b);
									missileVelocity[a].erase(missileVelocity[a].begin() + b);
									timer[a].erase(timer[a].begin() + b);
									break;
								}

								if(mod[8])
									if(collisions(pMissiles[a][b], mod[8]))
									{
										game.removeModel(pMissiles[a][b]);
										pMissiles[a].erase(pMissiles[a].begin() + b);
										missileVelocity[a].erase(missileVelocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										//Boss a.k.a model 8, is now called CandyMan for teh purposes of functions.
										CandyMan->setHealth(CandyMan->getHealth() - 50);// When hit takes damage
										if(CandyMan->getHealth() <= 0)
										{
											game.removeModel(CandyMan); // If health = 0 then boss dead
											//	mod[8] = nullptr;
											bossActive = false;
											puts("Killed The BOSS\n");
										}
										puts("Hit The BOSS\n");
										break;
									}
							}


					}
				}
			else
			{
				//	mod[a]->getAnimation("squash")->update(mod[a]->getShader(),mod[a]->getMesh());
			}

		lastTime = (float)clock() / CLOCKS_PER_SEC;

		if(!movePlayer)
			if(game.isControllerConnected(0))
			{
				Xinput p1 = game.getController(0);

				p1.numButtons;
				p1.numSticks;
				float angle1 = 0;
				if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
				{

					angle1 = acosf(p1.Coord2D_sticks[RS].x /
						sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
							+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
					angle1 += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle1) * 2 : 0) + 90;//90 represents the start angle
					angle1 = fmodf(angle1, 360);
				}

				if(Xinput::buttonPressed(p1.buttons.A))
					printf("%d\n", p1.buttons.A);

				//move camera
				move *= 2;

				game.moveCameraPositionBy({p1.Coord2D_sticks[LS].x * move , 0 * move, p1.Coord2D_sticks[LS].y * move});//move camera
				game.moveCameraAngleBy(ang * (abs(p1.Coord2D_sticks[RS].x) + abs(p1.Coord2D_sticks[RS].y)), {p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0});//rotate camera
				//game.getMainCamera()->getTransformer().rotateBy({ ang *p1.Coord2D_sticks[RS].y ,ang *p1.Coord2D_sticks[RS].x ,0}, { p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0 });
				game.moveCameraPositionBy({0 ,p1.triggers[LT] * -move,0});//move out
				game.moveCameraPositionBy({0 ,p1.triggers[RT] * move,0});//move out
				move /= 2;
			}
		//}
		init = true;
	}
};

