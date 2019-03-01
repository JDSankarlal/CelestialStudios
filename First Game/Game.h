#pragma once
#define _USE_MATH_DEFINES
#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <vector>
#include "Player.h"
#include "Boss.h"
#include "Minion.h"
#include "Assault.h"
#include "Specialist.h"
#include "Medic.h"
#include "Tank.h"


typedef EmGineAudioPlayer AudioPlayer;
typedef GameEmGine GAME;

using std::vector;

class Game :public Scene
{
public:


	/// - Collision Class - ///

	bool collision(Model* l, Model* k)
	{
		//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
		Coord3D thing = l->getCenter() - k->getCenter();

		float distanceX = abs(thing.x);
		float distanceZ = abs(thing.z);

		float capW = (l->getWidth() + k->getWidth()) / 2;
		float capD = (l->getDepth() + k->getDepth()) / 2;

		if (std::abs(distanceX) <= capW)
			if (std::abs(distanceZ) <= capD)
				return true;

		return false;
	}


	///~ 3D Collision Function ~///

	bool collision3D(Model * l, Model * k)
	{
		//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
		Coord3D thing = l->getCenter() - k->getCenter();

		float distanceX = abs(thing.x);
		float distanceY = abs(thing.y);
		float distanceZ = abs(thing.z);

		float capW = (l->getWidth() + k->getWidth()) / 2;
		float capH = (l->getHeight() + k->getHeight()) / 2;
		float capD = (l->getDepth() + k->getDepth()) / 2;

		if (std::abs(distanceX) <= abs(capW))
			if (std::abs(distanceZ) <= abs(capD))
				if (std::abs(distanceY) <= abs(capH))
					return true;

		return false;
	}

	//instance key is pressed
	void keyInputPressed(int key, int modifier)
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
	void keyInputReleased(int key, int modifier)
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
		if (key == GLFW_KEY_KP_6)
			GAME::setFPSLimit(GAME::getFPSLimit() + 1);
		if (key == GLFW_KEY_KP_4)
			GAME::setFPSLimit(GAME::getFPSLimit() - 1);

		if (key == GLFW_KEY_F) //Toggles Fullscreen
		{
			static bool full;
			GAME::getWindow()->setFullScreen(full = !full);
			printf("Full Screen: %s\n", full ? "true" : "false");
		}

		if (key == GLFW_KEY_SPACE) //changes the model that is being moved
		{
			static CAMERA_TYPE type = PERSPECTIVE;
			GAME::setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
		}

		if (key == GLFW_KEY_TAB)
			movePlayer = !movePlayer;

		if (key == GLFW_KEY_F5) //resets the camera
		{
			GAME::m_modelShader->refresh();
			//			GAME::m_grayScalePost->refresh();
						//GAME::setCameraAngle(0, { 1, 1, 1 });
						//	GAME::setCameraPosition({0,0,0});
		}

		if (key == 'R')
			GAME::setCameraAngle(0, { 1,1,1 });

		printf("key RELEASED code: %d\n\n", key);
	}

	void mouseButtonReleased(int button, int _mod)
	{
		_mod;
		if (button == LEFT_BUTTON)
			leftM = InputManager::getMouseCursorPosition();
		if (button == RIGHT_BUTTON)
			rightM = InputManager::getMouseCursorPosition();
	}

	void playerTypes(vector<Player*> & playerType)
	{
		mod.insert(mod.begin(), playerType.begin(), playerType.end());
	}

	// Set game screen
	void init()
	{
		/// - Set Camera  - ///
		GAME::setCameraType(PERSPECTIVE);
		GAME::setCameraPosition({ 0,15.5f,-5 });
		GAME::setCameraAngle(-45, { 1,0,0 });


		//GAME::setFPSLimit(60);
		/// - Load mod into Scene - ///

		for (auto& a : mod)
			GAME::addModel(a);

		static Animation walk[4], idle[4];

		for (int a = 0; a < 4; a++)
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
		mod.push_back(new Model("Models/Buildings/CashCorp/CashcorpBuildingWIP.obj"));
		GAME::addModel(mod.back());//4
		mod.push_back(new Model(*mod[4]));
		GAME::addModel(mod.back());//5
		mod.push_back(new Model(*mod[4]));
		GAME::addModel(mod.back());//6

		mod[6]->setToRender(false);

		//Project Nebula Sign
		mod.push_back(new Model("Models/Neon Signs/Project Nebula/signn.obj"));
		GAME::addModel(mod.back()); //7

		//Boss
		mod.push_back(new Boss("Models/BOSS/robotBOI.obj"));
		GAME::addModel(mod.back()); //8
		//mod[8]->enableBoundingBox(true);

		//Floor
		mod.push_back(new Model("Models/Floor/Floor.obj"));
		GAME::addModel(mod.back()); //9

		//Light Posts
		mod.push_back(new Model("Models/Lamp/lampPost.obj"));
		GAME::addModel(mod.back()); //10
		mod.push_back(new Model(*mod[10]));
		GAME::addModel(mod.back());//11
		mod.push_back(new Model(*mod[10]));
		GAME::addModel(mod.back());//12
		mod.push_back(new Model(*mod[10]));
		GAME::addModel(mod.back());//13
		mod.push_back(new Model(*mod[10]));
		GAME::addModel(mod.back());//14
		mod.push_back(new Model(*mod[10]));
		GAME::addModel(mod.back());//15

		//Bench
		mod.push_back(new Model("Models/Bench/Bench.obj"));
		GAME::addModel(mod.back()); //16
		mod.push_back(new Model(*mod[16]));
		GAME::addModel(mod.back());//17

		//Planet
		mod.push_back(new Model("Models/Planet/planet.obj"));
		GAME::addModel(mod.back()); //18
		mod[18]->setToRender(false);

		//Building 2s
		mod.push_back(new Model("Models/Buildings/Tunnel/tunnelWIP.obj"));
		GAME::addModel(mod.back());//19
		mod.push_back(new Model(*mod[19]));
		GAME::addModel(mod.back());//20
		mod.push_back(new Model(*mod[19]));
		GAME::addModel(mod.back());//21
		mod[21]->setToRender(false);

		//GraveStones
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj")); //22
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//23
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//24
		mod.push_back(new Model("Models/RIP/Rip Ani/RIP1.obj"));//25

		//Coloured ring "IDs"
		mod.push_back(new Model("Models/ID/Identifier.obj"));//26
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//27
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//28
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//29
		GAME::addModel(mod.back());

		//Building 3s
		mod.push_back(new Model("Models/Buildings/Building3/House.obj"));
		GAME::addModel(mod.back());//30
		mod.push_back(new Model(*mod[30]));
		GAME::addModel(mod.back());//31
		mod.push_back(new Model(*mod[30]));
		GAME::addModel(mod.back());//32

		//Building 4s
		mod.push_back(new Model("Models/Buildings/Building4/tallBuilding.obj"));
		GAME::addModel(mod.back());//33
		mod.push_back(new Model(*mod[33]));
		GAME::addModel(mod.back());//34

		//Trees
		mod.push_back(new Model("Models/DiedTree/tree.obj"));
		GAME::addModel(mod.back());//35
		mod.push_back(new Model(*mod[35]));
		GAME::addModel(mod.back());//36
		mod.push_back(new Model(*mod[35]));
		GAME::addModel(mod.back());//37
		mod.push_back(new Model(*mod[35]));
		GAME::addModel(mod.back());//38

		//Building 5s
		mod.push_back(new Model("Models/Buildings/Building5/smallShop.obj"));
		GAME::addModel(mod.back());//39
		mod.push_back(new Model(*mod[39]));
		GAME::addModel(mod.back());//40
		mod.push_back(new Model(*mod[39]));
		GAME::addModel(mod.back());//41

		mod[41]->setToRender(false);

		mod.push_back(new Model("Models/Buildings/Building6/Building6.obj"));
		GAME::addModel(mod.back());//42
		mod.push_back(new Model(*mod[42]));
		GAME::addModel(mod.back());//43

		mod[42]->setToRender(false);
		mod[43]->setToRender(false);

		mod.push_back(new Model("Models/Missile/BossMissile.obj"));
		GAME::addModel(mod.back());//44
		mod.push_back(new Model(*mod[44]));
		GAME::addModel(mod.back());//45
		mod.push_back(new Model(*mod[44]));
		GAME::addModel(mod.back());//46
		mod.push_back(new Model(*mod[44]));
		GAME::addModel(mod.back());//47

		mod.push_back(new Model("Models/Bullet/bullet.obj"));//48

		mod.push_back(new Model("Models/Trash/TrashCan.obj"));
		GAME::addModel(mod.back()); //49
		mod.push_back(new Model(*mod[49]));
		GAME::addModel(mod.back());//50

		mod[49]->setToRender(false);
		mod[50]->setToRender(false);

		mod.push_back(new Model("Models/Picnic/PicnicTable.obj"));
		GAME::addModel(mod.back()); //51
		mod.push_back(new Model(*mod[51]));
		GAME::addModel(mod.back());//52

		mod[51]->setToRender(false);
		mod[52]->setToRender(false);

		mod.push_back(new Model("Models/PizzaSign/PIZZA.obj"));
		GAME::addModel(mod.back()); //53

		mod.push_back(new Model("Models/AssaultModel/Weapon/AssaultClassGun.obj"));
		GAME::addModel(mod.back()); //54
		mod.push_back(new Model(*mod[54]));
		GAME::addModel(mod.back()); //55
		mod.push_back(new Model(*mod[54]));
		GAME::addModel(mod.back()); //56
		mod.push_back(new Model(*mod[54]));
		GAME::addModel(mod.back()); //57

		mod.push_back(new Model("Models/Planet/Planet2/planet.obj"));
		GAME::addModel(mod.back()); //58

		mod[58]->setToRender(false);
		//Pause Menu
		//mod.push_back(new Model("Models/Pause Menu/Pause Menu.obj"));//33

		//collision floor
		mod.push_back(new Model("Models/Floor/Floor2.obj"));//59
		GAME::addModel(mod[59]);

		mod[59]->setToRender(false);
		mod[59]->getTransformer().setScale(1.f, 1.0f, 1.5f), mod[59]->getTransformer().setPosition(0.0f, 0.15f, 5.0f);

		//missile hit box
		mod.push_back(new Model(*mod[44]));//60

		GAME::addModel(mod.back()); //
		mod[60]->setToRender(false);
		mod[60]->getTransformer().setScale(6, 1, 1);
		mod.push_back(new Model(*mod[60]));//61
		GAME::addModel(mod.back());//
		mod.push_back(new Model(*mod[60]));//62
		GAME::addModel(mod.back());//
		mod.push_back(new Model(*mod[60]));//63
		GAME::addModel(mod.back());//

		mod[44]->addChild(mod[60]);
		mod[45]->addChild(mod[61]);
		mod[46]->addChild(mod[62]);
		mod[47]->addChild(mod[63]);

		//players's blood bar
		mod.push_back(new Model("Models/BloodBar/RedBar/blood.obj"));//64
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/BlueBar/blood.obj"));//65
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/GreenBar/blood.obj"));//66
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/YellowBar/blood.obj"));//67
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/RedBarLighter/blood.obj"));//68
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/BlueBarLighter/blood.obj"));//69
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/GreenBarLighter/blood.obj"));//70
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/YellowBarLighter/blood.obj"));//71
		GAME::addModel(mod.back());

		//boss's blood bar
		mod.push_back(new Model("Models/BloodBar/PinkBar/blood.obj"));//72
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/BloodBar/PinkBarLighter/blood.obj"));//73
		GAME::addModel(mod.back());

		//bullet's circle
		mod.push_back(new Model("Models/BulletCircle/BulletCircle.obj"));//74
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//75
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//76
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//77
		GAME::addModel(mod.back());

		//Minions
		mod.push_back(new Minion("Models/Minion/SmallRobot/SmallRobot.obj"));//78
		GAME::addModel(mod[78]);
		mod[78]->setToRender(false);

		//TRAIN
		mod.push_back(new Model("Models/Train/Head/trainhead.obj"));//79
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/Train/Body/trainbodyblend.obj"));//80
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//81
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//82
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//83
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//84
		GAME::addModel(mod.back());
		mod.push_back(new Model("Models/Train/Head/trainhead.obj"));//85
		GAME::addModel(mod.back());
		mod[79]->getTransformer().setPosition(-14.45f, 0.3f, 8.0f);
		mod[80]->getTransformer().setPosition(-9.2f, 0.3f, 8.0f);
		mod[81]->getTransformer().setPosition(-4.6f, 0.3f, 8.0f);
		mod[82]->getTransformer().setPosition(0.0f, 0.3f, 8.0f);
		mod[83]->getTransformer().setPosition(4.6f, 0.3f, 8.0f);
		mod[84]->getTransformer().setPosition(9.2f, 0.3f, 8.0f);
		mod[85]->getTransformer().setPosition(14.45f, 0.3f, 8.0f), mod[85]->getTransformer().setRotation(Coord3D(0, 180, 0));

		//RAIL
		mod.push_back(new Model("Models/Rail/rail.obj"));//86
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//87
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//88
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//89
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//90
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//91
		GAME::addModel(mod.back());
		mod.push_back(new Model(*mod.back()));//92
		GAME::addModel(mod.back());

		mod[86]->getTransformer().setScale(0.7f), mod[86]->getTransformer().setPosition(-18.0f, 0.0f, 8.0f), mod[86]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[87]->getTransformer().setScale(0.7f), mod[87]->getTransformer().setPosition(-12.0f, 0.0f, 8.0f), mod[87]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[88]->getTransformer().setScale(0.7f), mod[88]->getTransformer().setPosition(-6.0f, 0.0f, 8.0f), mod[88]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[89]->getTransformer().setScale(0.7f), mod[89]->getTransformer().setPosition(0.0f, 0.0f, 8.0f), mod[89]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[90]->getTransformer().setScale(0.7f), mod[90]->getTransformer().setPosition(6.0f, 0.0f, 8.0f), mod[90]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[91]->getTransformer().setScale(0.7f), mod[91]->getTransformer().setPosition(12.0f, 0.0f, 8.0f), mod[91]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[92]->getTransformer().setScale(0.7f), mod[92]->getTransformer().setPosition(18.0f, 0.0f, 8.0f), mod[92]->getTransformer().setRotation(Coord3D(0, 90, 0));

		//Medic Healing Ring
		mod.push_back(new Model("Models/BulletCircle/BulletCircle.obj"));//93
		GAME::addModel(mod.back());
		mod[93]->setToRender(false);
		mod[93]->getTransformer().setScale(2,1,2);


		/// - Set Model Transforms - ///
		//Player Transforms
		mod[0]->getTransformer().setScale(1.35f), mod[0]->getTransformer().setPosition(1.0f, 0.0f, -5.0f);
		mod[1]->getTransformer().setScale(1.35f), mod[1]->getTransformer().setPosition(-1.0f, 0.0f, -5.0f);
		mod[2]->getTransformer().setScale(1.35f), mod[2]->getTransformer().setPosition(2.0f, 0.0f, -5.0f);
		mod[3]->getTransformer().setScale(1.35f), mod[3]->getTransformer().setPosition(-2.0f, 0.0f, -5.0f);
		mod[0]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[1]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[2]->getTransformer().setRotation(Coord3D(0, 180, 0));
		mod[3]->getTransformer().setRotation(Coord3D(0, 180, 0));

		//Building Transforms
		//Building 1s
		mod[4]->getTransformer().setScale(1), mod[4]->getTransformer().setPosition(-15.175f, 0.0f, -2.0f), mod[4]->getTransformer().setRotation({ 0.0f,90.0f,0.0f });;
		mod[5]->getTransformer().setScale(1), mod[5]->getTransformer().setPosition(6.0f, 0.0f, 29.0f), mod[5]->getTransformer().setRotation({ 0.0f,-90.0f,0.0f });
		//mod[6]->getTransformer().setScale(2), mod[6]->getTransformer().setPosition(-4.0f, 0.0f, 22.75f), mod[6]->getTransformer().setRotation({0.0f,-90.0f,0.0f});

		//Building 2s
		mod[19]->getTransformer().setScale(0.85f), mod[19]->getTransformer().setPosition(-16.8f, 0.0f, 7.8f), mod[19]->getTransformer().setRotation({ 0.0f, 90.0f,0.0f });
		mod[20]->getTransformer().setScale(0.85f), mod[20]->getTransformer().setPosition(16.8f, 0.0f, 8.0f), mod[20]->getTransformer().setRotation({ 0.0f, -90.0f, 0.0f });;
		//mod[21]->getTransformer().setScale(1.75f), mod[21]->getTransformer().setPosition(13.5f, 0.0f, 22.4f), mod[21]->getTransformer().setRotation({0.0f, -90.0f, 0.0f});

		//Buildings 3s
		mod[30]->getTransformer().setPosition(10.5f, 0.0f, 23.6f);
		mod[31]->getTransformer().setPosition(19.5f, 0.0f, 3.75f), mod[31]->getTransformer().setRotation({ 0,180,0 });
		mod[32]->getTransformer().setPosition(-9.3f, 0.0f, 25.35f), mod[32]->getTransformer().setRotation({ 0,-90,0 });
		//Building 4s
		mod[33]->getTransformer().setPosition(17.35f, 0.0f, 20.9f), mod[33]->getTransformer().setRotation({ 0,-90,0 });
		mod[34]->getTransformer().setPosition(-14.25f, 0.0f, 24.30f), mod[34]->getTransformer().setRotation({ 0,180,0 });
		//Building 5s
		mod[39]->getTransformer().setScale(1.0f, 1.0f, 1.05f), mod[39]->getTransformer().setPosition(19.6f, 0.0f, 16.5f), mod[39]->getTransformer().setRotation({ 0,180,0 });
		mod[40]->getTransformer().setScale(1.25f, 1.0f, 1.0f), mod[40]->getTransformer().setPosition(-16.9f, 0.0f, 16.35f), mod[40]->getTransformer().setRotation({ 0,90,0 });
		//mod[41]->getTransformer().setScale(1.0f, 1.3f, 1.6f), mod[41]->getTransformer().setPosition(1.0f, 0.0f, 25.5f), mod[41]->getTransformer().setRotation({0,-90,0});
		//Building 6s
		//mod[42]->getTransformer().setScale(1.0f, 1.5f, 1.8f), mod[42]->getTransformer().setPosition(-14.2f, 0.0f, 22.9f), mod[42]->getTransformer().setRotation({0,-90,0});
		//mod[43]->getTransformer().setPosition(16.65f, 0.0f, -3.15f), mod[43]->getTransformer().setRotation({0,180,0});

		//Project Nebula Sign Transforms
		mod[7]->getTransformer().setScale(3), mod[7]->getTransformer().setPosition(9.5f, 5.34f, 22.5f);

		//Boss Trarrnsforms
		mod[8]->getTransformer().setScale(1.3f), mod[8]->getTransformer().setPosition(0.0f, 0.0f, 23.0f), mod[8]->getTransformer().setRotation({ 0.0f, 0.0f, 0.0f });

		//Floor Transforms
		mod[9]->getTransformer().setScale(2.25f, 1.0f, 5.0f), mod[9]->getTransformer().setPosition(0.0f, 0.0f, 5.0f);

		//Street Light Transforms
		mod[10]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[10]->getTransformer().setPosition(13.0f, 0.0f, -1.0f);
		mod[11]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[11]->getTransformer().setPosition(13.0f, 0.0f, 7.0f);
		mod[12]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[12]->getTransformer().setPosition(13.0f, 0.0f, 15.0f);
		mod[13]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[13]->getTransformer().setPosition(-13.0f, 0.0f, -1.0f), mod[13]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });
		mod[14]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[14]->getTransformer().setPosition(-13.0f, 0.0f, 7.0f), mod[14]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });
		mod[15]->getTransformer().setScale(0.5f, 0.8f, 0.5f), mod[15]->getTransformer().setPosition(-13.0f, 0.0f, 15.0f), mod[15]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });

		//Bench Transforms
		mod[16]->getTransformer().setPosition(-13.0f, 0.0f, 3.0f);
		mod[17]->getTransformer().setPosition(13.0f, 0.0f, 3.0f), mod[17]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });

		//Planet Transforms
		mod[18]->getTransformer().setPosition(9.0f, 17.0f, 36.0f);
		//mod[58]->getTransformer().setPosition(-10.0f, 11.0f, 25.0f);

		//ID rings?
		mod[26]->setColour({ 255,0,0 });
		mod[26]->getTransformer().setScale(0.65f), mod[26]->getTransformer().setPosition(0.0f, 0.1f, 0.0f), mod[26]->getTransformer().setRotation({ 0,-90,0 });

		mod[27]->setColour({ 0,0,255 });
		mod[27]->getTransformer().setScale(0.65f), mod[27]->getTransformer().setPosition(0.0f, 0.1f, 0.0f), mod[27]->getTransformer().setRotation({ 0,-90,0 });

		mod[28]->setColour({ 0,255,0 });
		mod[28]->getTransformer().setScale(0.65f), mod[28]->getTransformer().setPosition(0.0f, 0.1f, 0.0f), mod[28]->getTransformer().setRotation({ 0,-90,0 });

		mod[29]->setColour({ 255,255,0 });
		mod[29]->getTransformer().setScale(0.65f), mod[29]->getTransformer().setPosition(0.0f, 0.1f, 0.0f), mod[29]->getTransformer().setRotation({ 0,-90,0 });

		//Trees
		mod[35]->getTransformer().setScale(0.3f), mod[35]->getTransformer().setPosition(13.0f, 0.0f, -3.0f), mod[35]->getTransformer().setRotation({ 0,-0,0 });
		mod[36]->getTransformer().setScale(0.3f), mod[36]->getTransformer().setPosition(-13.0f, 0.0f, -3.0f), mod[36]->getTransformer().setRotation({ 0,-0,0 });
		mod[37]->getTransformer().setScale(0.3f), mod[37]->getTransformer().setPosition(13.0f, 0.0f, 11.0f), mod[37]->getTransformer().setRotation({ 0,-0,0 });
		mod[38]->getTransformer().setScale(0.3f), mod[38]->getTransformer().setPosition(-13.0f, 0.0f, 11.0f), mod[38]->getTransformer().setRotation({ 0,-0,0 });

		//Missiles
		mod[44]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[45]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[46]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);
		mod[47]->getTransformer().setPosition(0.0f, 2.0f, 17.0f);

		//Bullet Size
		mod[48]->getTransformer().setScale(50.f);
		//Pizza Sign
		mod[53]->getTransformer().setScale(1.5f), mod[53]->getTransformer().setPosition(-10.25f, 5.4f, 22.3f);

		//Assault Weapons
		mod[54]->getTransformer().setScale(0.075f), mod[54]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[54]->getTransformer().setRotation({ 0.0f,0.0f,0.0f });
		mod[55]->getTransformer().setScale(0.075f), mod[55]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[55]->getTransformer().setRotation({ 0.0f,0.0f,0.0f });
		mod[56]->getTransformer().setScale(0.075f), mod[56]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[56]->getTransformer().setRotation({ 0.0f,0.0f,0.0f });
		mod[57]->getTransformer().setScale(0.075f), mod[57]->getTransformer().setPosition(-0.1f, 0.65f, -0.15f), mod[57]->getTransformer().setRotation({ 0.0f,0.0f,0.0f });

		//Player Blood Bar
		for (int i = 0; i < 4; i++)
		{
			mod[i + 64]->getTransformer().setPosition(mod[i]->getTransformer().getPosition() + Coord3D{ 0.35f,1.4f,0.0f });
			mod[i + 64]->getTransformer().setRotation(Coord3D(0, 90, 0));
			mod[i + 64]->getTransformer().setScale(0.08f, 0.08f, 0.065f);
			mod[i + 68]->getTransformer().setPosition(mod[i + 64]->getTransformer().getPosition());
			mod[i + 68]->getTransformer().setRotation(Coord3D(0, 90, 0));
			mod[i + 68]->getTransformer().setScale(0.08f, 0.08f, 0.065f);
		}

		//Boss Blood Bar
		mod[72]->getTransformer().setPosition(mod[8]->getTransformer().getPosition() + Coord3D{ 13.0f,18.5f,0.0f });
		mod[72]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[72]->getTransformer().setScale(0.8f, 0.8f, 2.5f);
		mod[73]->getTransformer().setPosition(mod[72]->getTransformer().getPosition());
		mod[73]->getTransformer().setRotation(Coord3D(0, 90, 0));
		mod[73]->getTransformer().setScale(0.8f, 0.8f, 2.5f);

		//Bullet Circle
		mod[74]->setColour({ 255,0,0,150 });
		mod[74]->getTransformer().setScale(0.65f), mod[74]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[74]->getTransformer().setRotation({ 0,-90,0 });

		mod[75]->setColour({ 0,0,255,150 });
		mod[75]->getTransformer().setScale(0.65f), mod[75]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[75]->getTransformer().setRotation({ 0,-90,0 });

		mod[76]->setColour({ 0,255,0,150 });
		mod[76]->getTransformer().setScale(0.65f), mod[76]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[76]->getTransformer().setRotation({ 0,-90,0 });

		mod[77]->setColour({ 255,255,0,150 });
		mod[77]->getTransformer().setScale(0.65f), mod[77]->getTransformer().setPosition(0.0f, 0.05f, 0.0f), mod[77]->getTransformer().setRotation({ 0,-90,0 });



		/// - Set Model Colour - ///
		//Players colors and children
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
		mod[0]->addChild(mod[74]);
		mod[1]->addChild(mod[75]);
		mod[2]->addChild(mod[76]);
		mod[3]->addChild(mod[77]);

		LightSource::setLightAmount(14);
		for (int a = 0; a < 6; a++)
		{
			//mod[10 + a]->boundingBoxUpdate();
			LightSource::setLightType(LIGHT_TYPE::DIRECTIONAL, a);
			LightSource::setParent(mod[10 + a], a);
			LightSource::setPosition({ -5.0f,4.5,0.0f }, a);
			LightSource::setDirection({ 0.0f,-1.0f,0.0f }, a);
			//LightSource::setDiffuse({ 255,100,0,100 }, 6);
			//LightSource::setAttenuationQuadratic(0.06f, 6);
		}

		LightSource::setLightType(LIGHT_TYPE::POINT, 6);
		LightSource::setParent(mod[44], 6);
		LightSource::setDiffuse({ 255,100,0,100 }, 6);
		LightSource::setAttenuationQuadratic(0.06f, 6);

		LightSource::setLightType(LIGHT_TYPE::POINT, 7);
		LightSource::setParent(mod[0], 7);
		LightSource::setPosition({ 0, -0.75f, 0 }, 7);
		LightSource::setDiffuse({ 255,0,0,100 }, 7);
		LightSource::setAttenuationQuadratic(1.f, 7);

		LightSource::setLightType(LIGHT_TYPE::POINT, 8);
		LightSource::setParent(mod[1], 8);
		LightSource::setPosition({ 0, -0.75f, 0 }, 8);
		LightSource::setDiffuse({ 0,0,255,100 }, 8);
		LightSource::setAttenuationQuadratic(1.f, 8);

		LightSource::setLightType(LIGHT_TYPE::POINT, 9);
		LightSource::setParent(mod[2], 9);
		LightSource::setPosition({ 0, -0.75f, 0 }, 9);
		LightSource::setDiffuse({ 0,255,0,100 }, 9);
		LightSource::setAttenuationQuadratic(1.f, 9);

		LightSource::setLightType(LIGHT_TYPE::POINT, 10);
		LightSource::setParent(mod[3], 10);
		LightSource::setPosition({ 0, -0.75f, 0 }, 10);
		LightSource::setDiffuse({ 255,255,0,100 }, 10);
		LightSource::setAttenuationQuadratic(1.f, 10);

		LightSource::setLightType(LIGHT_TYPE::POINT, 11);
		LightSource::setParent(mod[45], 11);
		LightSource::setDiffuse({ 255,100,0,100 }, 11);
		LightSource::setAttenuationQuadratic(0.06f, 11);

		LightSource::setLightType(LIGHT_TYPE::POINT, 12);
		LightSource::setParent(mod[46], 12);
		LightSource::setDiffuse({ 255,100,0,100 }, 12);
		LightSource::setAttenuationQuadratic(0.06f, 12);

		LightSource::setLightType(LIGHT_TYPE::POINT, 13);
		LightSource::setParent(mod[47], 13);
		LightSource::setDiffuse({ 255,100,0,100 }, 13);
		LightSource::setAttenuationQuadratic(0.06f, 13);

		LightSource::setSceneAmbient({ 255,255,255,255 });

		/// - Set Camera  - ///

		GAME::setCameraPosition({ 0,15.5f,-17.5 });
		GAME::setCameraAngle(-25, { 1,0,0 });

		/// not needed ///
		keyPressed = [=](int a, int b) {keyInputPressed(a, b); };
		keyReleased = [=](int a, int b) {keyInputReleased(a, b); };
		mouseReleased = [=](int a, int b) {mouseButtonReleased(a, b); };

		AudioPlayer::init();

		audio.createAudioStream("Game Jam(Full).wav");

		audio.play(true);
	}


	/// - The Update Loop - ///

	void update(double dt)
	{
		mod[48]->getTransformer().setScale(1);
		//Time
		static float  time = 0;
		time += (float)dt; //Add Delta Time to Time

		//Player Ability Variables
		static float circleTime;
		static bool healingCircle = false;

		//static float coolDown = 0;
		static float duration = 0;
		static bool f = true;

		float move = .1f;

		static float pointSize = 50.0f;
		static Player* players;

		// Boss Variables
		static Boss* CandyMan;
		CandyMan = (Boss*)mod[8]; //Set model 8 as Boss called "CandyMan"
		static bool bossActive = true; //

		//Minion Variables
		static vector<Minion*>minions;//Vector of minions
		static int minionCounter = 0;
		static float minionTimer;
		static float minionSpeed = .05f;
		static int maxMinionsOnScreen = 20;

		//Train Variables
		static float trainTimer = 0; //Determines when train comes and goes

		static vector<float> timer[4];
		static vector<Model*> pMissiles[4];
		static vector<Coord3D> missileVelocity[4];
		static vector<Model*> bullets[4];
		static vector<Coord3D> velocity[4];
		static bool gunControlLaw[4], dashControl[4];//stops the creation of bullets when trigger is healed down
		static float  curveroni[4] = { 0 ,0,0,0 };
		static bool hasTarget[4] = { 0 ,0,0,0 };

		static bool dead[4];

		static float lastTime = (float)clock() / CLOCKS_PER_SEC;

		static Animation squash[4];

		static bool init = false;

		static float angle[4] = { 180,180,180,180 };

		static vector<Boss*> nimimis;


		/// - Math for the Catmull curves for the Boss - ///

		for (int a = 0; a < 4; a++)
		{
			if (!dead[a])
			{
				curveroni[a] += .01f;
				if (curveroni[a] >= 1)
				{
					hasTarget[a] = false;
				}
				curveroni[a] = fmodf(curveroni[a], 1);

				static Coord3D bossTarget[4];

				//gets a   target for missile (players 1,2,3 or 4) randomly
				if (!hasTarget[a])
				{

					bossTarget[a] = mod[a]->getTransformer().getPosition();

					hasTarget[a] = true;
				}


				if (mod[8])
					if (hasTarget[a])
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
							c1[a] = p1[a] - Coord3D{ 0,100,100 },//control point
							c2[a] = p2[a] - Coord3D{ 0,150,100 };//control point

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


		/// - Tombstone Animations - ///

		if (!init)
		{

			for (int a = 0; a < 4; a++)
			{
				squash[a].addDir("Models/RIP/Rip Ani/");
				squash[a].setAnimationSpeed(.2f);
				//	squash[a].repeat(true);
			}
			init = true;
		}

		bool youDead = true;
		static float deathCounter = 0;
		if (init)

			/// - If game mode (NOT CAMERA MODE) - ///
			if (movePlayer)
			{
				for (int a = 0; a < 4; a++)
				{
					if (GAME::isControllerConnected(a))
					{
						Xinput p1 = GAME::getController(a);
						if (p1.buttonPressed(p1.buttons.SELECT))
						{
							for (int b = 0; b < 4; b++)
								dead[b] = 0;
							GAME::setScene(new Game);
						}
						//Start button quits game
						if (p1.buttonPressed(p1.buttons.START))
						{
							puts("\nExiting Game\n");
							GAME::exit();
						}

						if (!dead[a])
						{
							deathCounter = 0;
							youDead = !true;
							players = (Player*)mod[a];


							if (p1.sticks[RS].x || p1.sticks[RS].y)
							{

								angle[a] = acosf(p1.sticks[RS].x /
									sqrtf(p1.sticks[RS].x * p1.sticks[RS].x
										+ p1.sticks[RS].y * p1.sticks[RS].y)) * (180 / (float)M_PI);
								angle[a] += (p1.sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
								angle[a] = fmodf(angle[a], 360);
							}

							/// - Missile Collisions with Player - ///
							for (int b = 0; b < 4; b++)
							{
								bool collision = collision3D(players, mod[60 + b]);
								if (collision)
								{
									curveroni[a] = 1;
									mod[44 + b]->getTransformer().setPosition(mod[8]->getCenter());
									players->setHealth(players->getHealth() - 35);
									//Coord3D test = ;

								}
							}
							//Player comes near Boss, gets teleported backwards
							if (collision(players, CandyMan))
							{
								players->getTransformer().setPosition(players->getTransformer().getPosition().x, players->getTransformer().getPosition().y, players->getTransformer().getPosition().z - 15);
								players->setHealth(players->getHealth() - 35);
							}
							//If players dies
							if (players->getHealth() <= 0)
							{
								dead[a] = true;
								mod[22 + a]->setColour(players->getColour());
								mod[22 + a]->getTransformer().setScale(0.75f * 2, 1 * 2, 0.5 * 2), mod[22 + a]->getTransformer().setPosition(players->getTransformer().getPosition()), mod[22 + a]->getTransformer().setRotation({ 0.0f,270.0f,0.0f });
								GAME::addModel(mod[22 + a]);
								mod[22 + a]->addAnimation("squash", &squash[a]);

								mod[22 + a]->setAnimation("squash");
								GAME::removeModel(players);
								GAME::removeModel(mod[44] + a);
							}
							/// - Player Shooting - ///
							if (p1.triggers[RT] >= .95 && !gunControlLaw[a])
							{
								if (players->getBulletCount() > 0)
								{
									gunControlLaw[a] = true; //gun Control Law makes it so the guns function "manualy" instead of "fully automatic"

									bullets[a].push_back(new Model(*mod[48]));
									GAME::addModel(bullets[a].back());
									//bullets[a].back()->getTransformer().reset();
									//bullets[a].back()->getTransformer().setScale(5000);
									bullets[a].back()->setColour(players->getColour());//bullet color = players color
									Coord3D pos = mod[a]->getTransformer().getPosition();
									bullets[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
									bullets[a].back()->getTransformer().setScale(.25f);
									bullets[a].back()->getTransformer().setRotation({ 90 , angle[a] ,0 });

									float cosVal = cos((float)(fmodf(angle[a] - 90, 360) * (M_PI / 180)));
									float sinVal = sin((float)(fmodf(angle[a] - 90, 360) * (M_PI / 180)));

									velocity[a].push_back(Coord3D());
									velocity[a].back() = Coord3D(cosVal * move * 3, 0, sinVal * move * 3);

									timer[a].push_back(0);
									audio.createAudioStream("pew.wav");
									audio.play();
									players->setBulletCount(players->getBulletCount() - 1);
								}
							}
							else if (p1.triggers[RT] < .95&& gunControlLaw[a])
								gunControlLaw[a] = false;

							/// - Button Presses on controller - ///
							if (p1.buttonPressed(p1.buttons.X))
							{
								players->setBulletCount(30);
								puts("RELOADING!!!\n");
							}
							///- Medic Secial Ability Active - ///
							if (healingCircle == true)
							{
								//Healing
								if (collision3D(players, mod[93]))
								{
									players->setHealth(200);
								}
								//Makes medics Circle disappear
								if ((time - circleTime) >= 2.5f)
								{
									mod[93]->setToRender(false);
									((Medic*)players)->setTimeSinceLastHeal(time);
									healingCircle = false;
								}
							}
							if (p1.buttonPressed(p1.buttons.Y))
							{	
								/// - Assault Special Ability - ///
								if (players->type == assault)
								{
									if (time - ((Assault*)players)->getTimeSinceLastMissile() >= 3)
									{
										pMissiles[a].push_back(nullptr);
										GAME::addModel(pMissiles[a].back() = new Model(*mod[44]));
										pMissiles[a].back()->getTransformer().reset();
										pMissiles[a].back()->setColour(players->getColour());
										Coord3D pos = mod[a]->getTransformer().getPosition();
										pMissiles[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
										pMissiles[a].back()->getTransformer().setScale(0.4f);

										pMissiles[a].back()->getTransformer().setRotation({ 0 , angle[a] ,0 });

										float cosVal = cos((float)(fmodf(angle[a] - 90, 360) * (M_PI / 180)));
										float sinVal = sin((float)(fmodf(angle[a] - 90, 360) * (M_PI / 180)));

										missileVelocity[a].push_back(Coord3D());
										missileVelocity[a].back() = Coord3D(cosVal * move * 6, 0, sinVal * move * 6);
										((Assault*)players)->setTimeSinceLastMissile(time);

										timer[a].push_back(0);
										audio.createAudioStream("pew.wav");
										audio.play();
										puts("Special Ability ASSAULT\n");
									}
								}
								///- Medic Special Ability Inactive - ///
								if (players->type == medic)
								{
									if (healingCircle == false)
									{
										if (time - ((Medic*)players)->getTimeSinceLastHeal() >= 5)
										{
											mod[93]->getTransformer().setPosition(players->getTransformer().getPosition() + (Coord3D({ 0.0f, 0.1f, 0.0f })));
											mod[93]->setToRender(true);
											circleTime = time;
											puts("Special Ability MEDIC");
											healingCircle = true;
										}
									}
								}
								if (players->type == tank)
								{
									if (time - ((Tank*)players)->getTimeSinceLastShield() >= 3)
									{
										
									}
								}
								if (players->type == specialist)
								{
									if (time - ((Specialist*)players)->getTimeSinceLastTurret() >= 3)
									{
									
									}
								}

							}

							/// - Boss Spawns Minions - ///
							//TODO: More Minions, Have boss spawn minions, Make minions have unifrom move speed. Lerp between colours??
							if (minionCounter < 10)
							{

								minions.push_back(nullptr);
								GAME::addModel(minions.back() = new Minion(*mod[78]));
								minions.back()->setToRender(true);
								minions.back()->getTransformer().reset();
								minions.back()->setColour(200, 100, 50);
								minions.back()->getTransformer().getPosition();
								minions.back()->getTransformer().setPosition(float(rand() % 4 + 7), float(rand() % 1 + 2), -float(rand() % 3) + 2); // Random spawns in bottom right of screen
								minions.back()->getTransformer().setScale(0.4f, 0.6f, 0.4f);

								minionCounter += 1;
							}

							/// - Minions Movement Towards Players - ///
							for (unsigned int m = 0; m < minions.size(); m++)
							{
								Coord3D norm = players->getTransformer().getPosition() - minions[m]->getTransformer().getPosition();
								norm.normalize();

								minions[m]->getTransformer().translateBy(norm * .001f);
								if (collision(minions[m], minions[m]))// Might have to change one of the m values??
								{
									// TODO:  Minions collide with each other so they arent a blob.
								}
							}

							/// - Left Trigger to Dash - ///

							if (p1.triggers[LT] >= .95)
							{
								static float coolDown[4];

								//get deltaTime put into duraction variable

								if (time - coolDown[a] >= 3)
								{
									if (f == true)
									{
										duration = time;
										f = false;
									}
									move = 0.5f;
									if (time - 0.1f >= duration)
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
								if (move <= .1f)
									move = .1f;
								//f = false;
								dashControl[a] = false;
							}

							mod[a]->getTransformer().setRotation({ 0,angle[a], 0 });
							mod[a]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move players
							float speed = p1.sticks[LS].x * p1.sticks[LS].x + p1.sticks[LS].y * p1.sticks[LS].y;


							if (!collision(mod[a], mod[59]))
								mod[a]->getTransformer().setPosition(
									abs(mod[a]->getTransformer().getPosition().x) > mod[59]->getWidth() / 2 ? mod[a]->getTransformer().getPosition().x < 0 ? -mod[59]->getWidth() / 2 : mod[59]->getWidth() / 2 : mod[a]->getTransformer().getPosition().x,
									0,
									abs(mod[a]->getTransformer().getPosition().z) > mod[59]->getDepth() / 2 ? mod[a]->getTransformer().getPosition().z < 0 ? -mod[59]->getDepth() / 2 : mod[59]->getDepth() / 2 : mod[a]->getTransformer().getPosition().z);

							//Player Animations
							if (!speed)
								mod[a]->getAnimation("walk")->pause();
							else
							{
								mod[a]->getAnimation("walk")->play();
								mod[a]->setAnimation("walk");
								mod[a]->getAnimation("walk")->setAnimationSpeed(.25f / speed);
							}

							//Update each players's Blood Bar
							mod[a + 64]->getTransformer().setPosition(mod[a]->getTransformer().getPosition() + Coord3D{ 0.35f,1.6f,0.0f });
							mod[a + 64]->getTransformer().setScale(0.08f, 0.08f, 0.065f * (players->getHealth() / players->getInitialHealth()));
							mod[a + 68]->getTransformer().setPosition(mod[a + 64]->getTransformer().getPosition());
							if (dead[a] == true)
							{
								GAME::removeModel(mod[a + 64]);
								GAME::removeModel(mod[a + 68]);

								GAME::removeModel(mod[a + 26]);
								GAME::removeModel(mod[a + 54]);
								GAME::removeModel(mod[a + 74]);
							}

							//Update each players's Bullet Circle
							mod[a + 74]->getTransformer().setScale(0.65f * (players->getBulletCount() / 30.0f));
						}

						///~ Bullet Collisions ~///
						for (unsigned b = 0; b < bullets[a].size(); b++)
							if (bullets[a][b])
							{
								timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
								bullets[a][b]->getTransformer().translateBy(velocity[a][b].x, velocity[a][b].y, velocity[a][b].z);

								if (timer[a][b] >= 1)
								{
									GAME::removeModel(bullets[a][b]);
									bullets[a].erase(bullets[a].begin() + b);
									velocity[a].erase(velocity[a].begin() + b);
									timer[a].erase(timer[a].begin() + b);
									break;
								}

								bool bulletHit = false;
								/// Bullet Collisions with Train
								for (int t = 0; t < 7; t++)
								{
									if (collision(bullets[a][b], mod[79 + t]))
									{
										GAME::removeModel(bullets[a][b]);
										bulletHit = true;
									}
								}
								if (mod[8])
									if (collision(bullets[a][b], mod[8]))
									{
										GAME::removeModel(bullets[a][b]);
										bullets[a].erase(bullets[a].begin() + b);
										velocity[a].erase(velocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										//Boss a.k.a model 8, is now called "CandyMan" for the purposes of functions.
										CandyMan->setHealth(CandyMan->getHealth() - 10);// When hit takes damage
										if (CandyMan->getHealth() <= 0)
										{
											GAME::removeModel(CandyMan); // If health = 0 then boss dead
											//	mod[8] = nullptr;
											bossActive = false;
											puts("Killed The BOSS\n");
										}
										puts("Hit The BOSS\n");
										break;
									}

								for (auto& minion : minions)
								{
									if (collision(bullets[a][b], minion))
									{
										minion->setHealth(minion->getHealth() - 10);
										GAME::removeModel(bullets[a][b]);
										bulletHit = true;
										if (minion->getHealth() <= 0)
										{
											GAME::removeModel(minion);
											minions.erase(std::find(minions.begin(), minions.end(), minion));
											puts("Killed a boi\n");
											minionCounter -= 1;
											break;
										}

									}
									//TODO: Make Minions collide with players and do damage. Make Minions move at set speed
									//if (collision(minion, players))
									//{
									//	players->setHealth(players->getHealth() - 10);
									//}
								}

								if (bulletHit)
									bullets[a].erase(bullets[a].begin() + b),
									timer[a].erase(timer[a].begin() + b),
									velocity[a].erase(velocity[a].begin() + b);

							}

						for (unsigned b = 0; b < pMissiles[a].size(); b++)
							if (pMissiles[a][b])
							{
								timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
								pMissiles[a][b]->getTransformer().translateBy(missileVelocity[a][b].x, missileVelocity[a][b].y, missileVelocity[a][b].z);

								if (timer[a][b] >= 1)
								{
									GAME::removeModel(pMissiles[a][b]);
									pMissiles[a].erase(pMissiles[a].begin() + b);
									missileVelocity[a].erase(missileVelocity[a].begin() + b);
									timer[a].erase(timer[a].begin() + b);
									break;
								}

								/// - Missile Collisions with Train - ///
								for (int t = 0; t < 7; t++)
								{
									if (collision(pMissiles[a][b], mod[79 + t]))
									{
										GAME::removeModel(pMissiles[a][b]);
										pMissiles[a].erase(pMissiles[a].begin() + b);
									}
								}

								/// - If Boss Alive - ///
								if (mod[8])
									/// - If Missiles collide with Boss -///
									if (collision(pMissiles[a][b], mod[8]))
									{
										GAME::removeModel(pMissiles[a][b]);
										pMissiles[a].erase(pMissiles[a].begin() + b);
										missileVelocity[a].erase(missileVelocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										//Boss a.k.a model 8, is now called CandyMan for the purposes of functions.
										CandyMan->setHealth(CandyMan->getHealth() - 50);// When hit takes damage
										/// - If Boss Dies - ///
										if (CandyMan->getHealth() <= 0)
										{
											GAME::removeModel(CandyMan); // If health = 0 then boss dead
											bossActive = false;
											puts("Killed The BOSS\n");
										}
										puts("Hit The BOSS\n");
										break;
									}
							}

						if (bossActive == false)
						{
							GAME::removeModel(mod[72]);
							GAME::removeModel(mod[73]);
							mod[44]->getTransformer().setPosition(1000.0f, 1000.0f, 1000.0f);
							mod[45]->getTransformer().setPosition(1000.0f, 1000.0f, 1000.0f);
							mod[46]->getTransformer().setPosition(1000.0f, 1000.0f, 1000.0f);
							mod[47]->getTransformer().setPosition(1000.0f, 1000.0f, 1000.0f);
						}
					}
					mod[a]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
				}
			}
			else
			{
				//	mod[a]->getAnimation("squash")->update(mod[a]->getShader(),mod[a]->getMesh());
			}


		/// - Train Car Movement - ///
		for (int a = 0; a < 4; a++)
		{
			players = (Player*)mod[a];

			//Train Sits in middle of map
			if (0 <= (time - trainTimer) && 10 > (time - trainTimer))
			{
				for (int t = 0; t < 7; t++)
				{
					if (collision(mod[79 + t], players))
					{
						if (players->getTransformer().getPosition().z < mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, -0.1f));
						if (players->getTransformer().getPosition().z > mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, 0.1f));
					}
				}

			}
			//Train Moves off map
			if (10 <= (time - trainTimer) && 20 > (time - trainTimer))
			{
				for (int t = 0; t < 7; t++)
				{
					mod[79 + t]->getTransformer().translateBy(Coord3D{ 0.05f, 0.f, 0.f });//Move train cars right
					if (collision(mod[79 + t], players))
					{
						players->setHealth(players->getHealth() - 10);
						if (players->getTransformer().getPosition().z < mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, -0.8f));
						if (players->getTransformer().getPosition().z > mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, 0.8f));
					}
				}
			}
			//Train stops
			else if (20 <= (time - trainTimer) && 30 > (time - trainTimer))
			{
				for (int t = 0; t < 7; t++)
				{
					mod[79 + t]->getTransformer().translateBy(Coord3D{ 0.0f, 0.f, 0.f });//Stop Train cars
				}
			}
			//Train moves back onto map
			else if (30 <= (time - trainTimer) && 40 > (time - trainTimer))
			{
				for (int t = 0; t < 7; t++)
				{
					mod[79 + t]->getTransformer().translateBy(Coord3D{ -0.05f, 0.f, 0.f });//Move train cars back to the right
					if (collision(mod[79 + t], players))
					{
						players->setHealth(players->getHealth() - 10);
						if (players->getTransformer().getPosition().z < mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, -0.8f));
						if (players->getTransformer().getPosition().z > mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, 0.8f));
						//if (players->getTransformer().getPosition().x < mod[85]->getTransformer().getPosition().x)
						//	players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.8f, 0.f, 0.0f));
					}
				}
			}
			//Train stops on map
			else if (40 <= (time - trainTimer) && 50 > (time - trainTimer))
			{
				for (int t = 0; t < 7; t++)
				{
					mod[79 + t]->getTransformer().setPosition(mod[79 + t]->getTransformer().getPosition() + Coord3D{ 0.00f, 0.f, 0.f });//Stop Train cars on map
					if (collision(mod[79 + t], players))
					{
						if (players->getTransformer().getPosition().z < mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, -0.1f));
						if (players->getTransformer().getPosition().z > mod[79 + t]->getTransformer().getPosition().z)
							players->getTransformer().setPosition(players->getTransformer().getPosition() + Coord3D(0.0f, 0.f, 0.1f));
					}
				}
				trainTimer += time; //Reset Train timer so it all starts again.
			}
		}

		//Boss health bar calculation
		mod[72]->getTransformer().setScale(0.8f, 0.8f, 2.5f* (CandyMan->getHealth() / 1000.0f));

		if (youDead)
		{
			//TODO: do something when the party is dead, game over screen with "Main Menu" "Quit" options

		}
		else
		{

		}

		lastTime = (float)clock() / CLOCKS_PER_SEC;

		/// - If game not active and Camera is active (Move camera mode) - ///
		if (!movePlayer)
			if (GAME::isControllerConnected(0))
			{
				Xinput p1 = GAME::getController(0);
				deathCounter = 0;

				//move camera
				move *= 2;

				GAME::moveCameraPositionBy({ p1.sticks[LS].x * move , 0 * move, p1.sticks[LS].y * move });//move camera
				GAME::moveCameraAngleBy(ang * (abs(p1.sticks[RS].x) + abs(p1.sticks[RS].y)), { p1.sticks[RS].y  ,p1.sticks[RS].x, 0 });//rotate camera
				//GAME::getMainCamera()->getTransformer().rotateBy({ ang *p1.sticks[RS].y ,ang *p1.sticks[RS].x ,0}, { p1.sticks[RS].y  ,p1.sticks[RS].x, 0 });
				GAME::moveCameraPositionBy({ 0 ,p1.triggers[LT] * -move,0 });//move out
				GAME::moveCameraPositionBy({ 0 ,p1.triggers[RT] * move,0 });//move out
				move /= 2;
			}

		GAME::m_grayScalePost->enable();
		glUniform1f(GAME::m_grayScalePost->getUniformLocation("uTime"), deathCounter);
		GAME::m_grayScalePost->disable();

		deathCounter += .007f;
		deathCounter = deathCounter <= 1 ? deathCounter : 1;
	}

private:
	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;

	float ang = 2;
	int numModel = 0;
	bool m_left = 0, m_right = 0, m_in = 0, m_out = 0, m_up = 0, m_down = 0,
		rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0,
		movePlayer = true;
	Coord2D leftM, rightM;
	AudioPlayer audio;
	bool pause = true;
};
