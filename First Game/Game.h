#pragma once
#include <EmGineAudioPlayer.h>
#include <GameEmGine.h>
#include <SceneManager.h> 
#include <vector>
#include <list>
#include "Player.h"
#include "Boss.h"
#include "Minion.h"
#include "Assault.h"
#include "Specialist.h"
#include "Medic.h"
#include "Tank.h"

using std::vector;
using std::list;

class Game:public Scene
{
public:
	void onSceneExit() {}

	//instance key is pressed
	void keyInputPressed(int key, int modifier)
	{
		modifier;
		m_left = (key == 'A' ? true : m_left);
		m_right = (key == 'D' ? true : m_right);
		m_up = (key == 'E' ? true : m_up);
		m_down = (key == 'Q' ? true : m_down);
		m_fwd = (key == 'W' ? true : m_fwd);
		m_back = (key == 'S' ? true : m_back);

		rotLeft = (key == GLFW_KEY_LEFT ? true : rotLeft);
		rotRight = (key == GLFW_KEY_RIGHT ? true : rotRight);
		rotUp = (key == GLFW_KEY_UP ? true : rotUp);
		rotDown = (key == GLFW_KEY_DOWN ? true : rotDown);

		//printf("key PRESSED code: %d\n\n", key);
	}

	//instance key is released
	void keyInputReleased(int key, int modifier)
	{
		modifier;
		m_left = (key == 'A' ? false : m_left);
		m_right = (key == 'D' ? false : m_right);
		m_fwd = (key == 'W' ? false : m_fwd);
		m_back = (key == 'S' ? false : m_back);
		m_up = (key == 'E' ? false : m_up);
		m_down = (key == 'Q' ? false : m_down);

		rotLeft = (key == GLFW_KEY_LEFT ? false : rotLeft);
		rotRight = (key == GLFW_KEY_RIGHT ? false : rotRight);
		rotUp = (key == GLFW_KEY_UP ? false : rotUp);
		rotDown = (key == GLFW_KEY_DOWN ? false : rotDown);

		if(key == GLFW_KEY_TAB)
			++(*(char*)&currentState) %= 2;//don't ask

		if(key == 'S' && modifier == GLFW_MOD_CONTROL)
		{
			vector<Model*>tmp;
			for(auto& a : GameEmGine::getObjectList())
				tmp.push_back(a.second);
			SceneManager::saveScene("GameScene.scene", tmp);
		}

		//changes fps limit
		if(key == GLFW_KEY_KP_6)
			GameEmGine::setFPSLimit(GameEmGine::getFPSLimit() + 5);
		if(key == GLFW_KEY_KP_4)
			GameEmGine::setFPSLimit(GameEmGine::getFPSLimit() - 5);

		if(key == GLFW_KEY_F) //Toggles Full-screen
		{
			static bool full;
			GameEmGine::getWindow()->setFullScreen(full = !full);
			printf("Full Screen: %s\n", full ? "true" : "false");
		}

		if(key == GLFW_KEY_SPACE) //Changes the camera mode
		{
			static Camera::CAM_TYPE type = Camera::FRUSTUM;
			GameEmGine::setCameraType(type = type == Camera::ORTHOGRAPHIC ? Camera::FRUSTUM : Camera::ORTHOGRAPHIC);
		}

		if(key == GLFW_KEY_F5) //refreshes the shaders
			Shader::refresh();


		//if(key == GLFW_KEY_Q)
		//	GameEmGine::lutActive = (GameEmGine::lutActive == false) ? true : false;
		//if(key == GLFW_KEY_T)
		//	GameEmGine::toonActive = (GameEmGine::toonActive == false) ? true : false;
		//if(key == 'R')
		//	GameEmGine::setCameraRotation({0,0,0});

		//printf("key RELEASED code: %d\n\n", key);
	}


	void mouseButtonReleased(int button, int a_mod)
	{
		a_mod;
		//if(button == MOUSE_LEFT_BUTTON)
		//	leftM = InputManager::getMousePosition();
		//if(button == MOUSE_RIGHT_BUTTON)
		//	rightM = InputManager::getMousePosition();
		static ColourRGBA lastColour;

		if(button == MOUSE_LEFT_BUTTON)
		{
			if(curModel)
				curModel->setColour(lastColour);

			curModel = GameEmGine::getMouseCollisionObject();

			if(curModel)
			{
				if(curModel)
					lastColour = curModel->getColour();

				curModel->setColour(0, .65f, 0);
			}
		}
		if(button == MOUSE_RIGHT_BUTTON)
		{
			if(curModel)
				curModel->setColour(lastColour);
			curModel = nullptr;
		}
	}

	void playerTypes(vector<Player*>& playerType)
	{
		for(Player* a : playerType)
			mod.push_back(*a);
	}

	// Set game screen
	void init()
	{

		setSkyBox("skyboxes/space2/");
		enableSkyBox(true);

		//GameEmGine::m_modelShader->sendUniform("darken", 1);
		mod.resize(132);//sets the initial size of the vector (if u add any more models, increase this number)

		/// - Set Camera  - ///
		GameEmGine::setCameraType(Camera::FRUSTUM);
		GameEmGine::getMainCamera()->translate({0,15.5f,-5});
		GameEmGine::getMainCamera()->rotate({-45,0,0});


		//GAME::setFPSLimit(60);
		/// - Load mod into Scene - ///

		for(auto& a : mod)
			if(a.getMesh(0))
				GameEmGine::addModel(&a),
				Minion::addTarget((Player*)&a);
			else
				break;

		static Transformer uniScaler;
		uniScaler.scale(.5);

	#if FALSE
	#pragma region BULL SHIT!!!!!

		//Building 1s
		*std::next(mod.begin(), 4) = (Model("Models/Buildings/CashCorp/CashcorpBuildingWIP.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 4));//4
		*std::next(mod.begin(), 5) = (Model(*std::next(mod.begin(), 4)));
		GameEmGine::addModel(&*std::next(mod.begin(), 5));//5
		*std::next(mod.begin(), 6) = (Model(*std::next(mod.begin(), 4)));
		GameEmGine::addModel(&*std::next(mod.begin(), 6));//6

		(&*std::next(mod.begin(), 6))->setToRender(false);

		//Project Nebula Sign
		*std::next(mod.begin(), 7) = Model("Models/Neon Signs/Project Nebula/signn.obj");
		GameEmGine::addModel(&*std::next(mod.begin(), 7)); //7

		//Boss		
		*std::next(mod.begin(), 8) = Boss("Models/BOSS/missleShoot/BMS1.obj");
		GameEmGine::addModel(&*std::next(mod.begin(), 8)); //8



		//Floor
		*std::next(mod.begin(), 9) = (Model("Models/Floor/Floor.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 9)); //9

		//Light Posts
		*std::next(mod.begin(), 10) = (Model("Models/Lamp/lampPost.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 10)); //10
		*std::next(mod.begin(), 11) = (Model(*std::next(mod.begin(), 10)));
		GameEmGine::addModel(&*std::next(mod.begin(), 11));//11
		*std::next(mod.begin(), 12) = (Model(*std::next(mod.begin(), 10)));
		GameEmGine::addModel(&*std::next(mod.begin(), 12));//12
		*std::next(mod.begin(), 13) = (Model(*std::next(mod.begin(), 10)));
		GameEmGine::addModel(&*std::next(mod.begin(), 13));//13
		*std::next(mod.begin(), 14) = (Model(*std::next(mod.begin(), 10)));
		GameEmGine::addModel(&*std::next(mod.begin(), 14));//14
		*std::next(mod.begin(), 15) = (Model(*std::next(mod.begin(), 10)));
		GameEmGine::addModel(&*std::next(mod.begin(), 15));//15

		//Bench
		*std::next(mod.begin(), 16) = (Model("Models/Bench/Bench.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 16)); //16
		*std::next(mod.begin(), 17) = (Model(*std::next(mod.begin(), 16)));
		GameEmGine::addModel(&*std::next(mod.begin(), 17));//17

		//Planet
		*std::next(mod.begin(), 18) = (Model("Models/Planet/planet.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 18)); //18
		(&*std::next(mod.begin(), 18))->setToRender(false);

		//Building 2s
		*std::next(mod.begin(), 19) = (Model("Models/Buildings/Tunnel/Tunnel_Back_Final.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 19));//19
		*std::next(mod.begin(), 20) = (Model(*std::next(mod.begin(), 19)));
		GameEmGine::addModel(&*std::next(mod.begin(), 20));//20
		*std::next(mod.begin(), 21) = (Model(*std::next(mod.begin(), 19)));
		GameEmGine::addModel(&*std::next(mod.begin(), 21));//21
		(&*std::next(mod.begin(), 21))->setToRender(false);

		//GraveStones
		*std::next(mod.begin(), 22) = (Model("Models/RIP/Rip Ani/RIP1.obj")); //22
		*std::next(mod.begin(), 23) = (Model("Models/RIP/Rip Ani/RIP1.obj"));//23
		*std::next(mod.begin(), 24) = (Model("Models/RIP/Rip Ani/RIP1.obj"));//24
		*std::next(mod.begin(), 25) = (Model("Models/RIP/Rip Ani/RIP1.obj"));//25

		//Building 3s
		*std::next(mod.begin(), 30) = (Model("Models/Buildings/Building3/House.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 30));//30
		*std::next(mod.begin(), 31) = (Model(*std::next(mod.begin(), 30)));
		GameEmGine::addModel(&*std::next(mod.begin(), 31));//31
		*std::next(mod.begin(), 32) = (Model(*std::next(mod.begin(), 30)));
		GameEmGine::addModel(&*std::next(mod.begin(), 32));//32

		//Building 4s
		*std::next(mod.begin(), 33) = (Model("Models/Buildings/Building4/tallBuilding.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 33));//33
		*std::next(mod.begin(), 34) = (Model(*std::next(mod.begin(), 33)));
		GameEmGine::addModel(&*std::next(mod.begin(), 34));//34

		//Trees
		*std::next(mod.begin(), 35) = (Model("Models/DiedTree/tree.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 35));//35
		*std::next(mod.begin(), 36) = (Model(*std::next(mod.begin(), 35)));
		GameEmGine::addModel(&*std::next(mod.begin(), 36));//36
		*std::next(mod.begin(), 37) = (Model(*std::next(mod.begin(), 35)));
		GameEmGine::addModel(&*std::next(mod.begin(), 37));//37
		*std::next(mod.begin(), 38) = (Model(*std::next(mod.begin(), 35)));
		GameEmGine::addModel(&*std::next(mod.begin(), 38));//38

		//Building 5s
		*std::next(mod.begin(), 39) = (Model("Models/Buildings/Building5/smallShop.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 39));//39
		*std::next(mod.begin(), 40) = (Model(*std::next(mod.begin(), 39)));
		GameEmGine::addModel(&*std::next(mod.begin(), 40));//40
		*std::next(mod.begin(), 41) = (Model(*std::next(mod.begin(), 39)));
		GameEmGine::addModel(&*std::next(mod.begin(), 41));//41

		(&*std::next(mod.begin(), 41))->setToRender(false);

		*std::next(mod.begin(), 42) = (Model("Models/Buildings/Building6/Building6.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 42));//42
		*std::next(mod.begin(), 43) = (Model(*std::next(mod.begin(), 42)));
		GameEmGine::addModel(&*std::next(mod.begin(), 43));//43

		(&*std::next(mod.begin(), 42))->setToRender(false);
		(&*std::next(mod.begin(), 43))->setToRender(false);

		*std::next(mod.begin(), 44) = Model("Models/missile/candyMissile.obj");

		*std::next(mod.begin(), 48) = (Model("Models/Bullet/bullet.obj"));//48

		*std::next(mod.begin(), 49) = (Model("Models/Trash/TrashCan.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 49)); //49
		*std::next(mod.begin(), 50) = (Model(*std::next(mod.begin(), 49)));
		GameEmGine::addModel(&*std::next(mod.begin(), 50));//50

		(&*std::next(mod.begin(), 49))->setToRender(false);
		(&*std::next(mod.begin(), 50))->setToRender(false);

		*std::next(mod.begin(), 51) = (Model("Models/Picnic/PicnicTable.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 51)); //51
		*std::next(mod.begin(), 52) = (Model(*std::next(mod.begin(), 51)));
		GameEmGine::addModel(&*std::next(mod.begin(), 52));//52

		(&*std::next(mod.begin(), 51))->setToRender(false);
		(&*std::next(mod.begin(), 52))->setToRender(false);

		*std::next(mod.begin(), 53) = (Model("Models/PizzaSign/PIZZA.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 53)); //53

		*std::next(mod.begin(), 54) = (Model("Models/AssaultModel/Weapon/AssaultClassGun.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 54)); //54
		*std::next(mod.begin(), 55) = (Model(*std::next(mod.begin(), 54)));
		GameEmGine::addModel(&*std::next(mod.begin(), 55)); //55
		*std::next(mod.begin(), 56) = (Model(*std::next(mod.begin(), 54)));
		GameEmGine::addModel(&*std::next(mod.begin(), 56)); //56
		*std::next(mod.begin(), 57) = (Model(*std::next(mod.begin(), 54)));
		GameEmGine::addModel(&*std::next(mod.begin(), 57)); //57
		(&*std::next(mod.begin(), 54))->setToRender(false);
		(&*std::next(mod.begin(), 55))->setToRender(false);
		(&*std::next(mod.begin(), 56))->setToRender(false);
		(&*std::next(mod.begin(), 57))->setToRender(false);

		*std::next(mod.begin(), 58) = (Model("Models/Planet/Planet2/planet.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 58)); //58

		(&*std::next(mod.begin(), 58))->setToRender(false);
		//Pause Menu
		//&*std::next(mod.begin(),) = (Model("Models/Pause Menu/Pause Menu.obj"));//33

		//collision2D floor
		*std::next(mod.begin(), 59) = (Model("Models/Floor/Floor2.obj"));//59
		GameEmGine::addModel(&*std::next(mod.begin(), 59));

		(&*std::next(mod.begin(), 59))->setToRender(false);
		(&*std::next(mod.begin(), 59))->scale(1.f, 1.0f, 1.f), (&*std::next(mod.begin(), 59))->translate(0.0f, 0.15f, 5.0f);

		//TRAIN
		*std::next(mod.begin(), 79) = (Model("Models/Train/Head/trainhead.obj"));//79
		GameEmGine::addModel(&*std::next(mod.begin(), 79));
		*std::next(mod.begin(), 80) = (Model("Models/Train/Body/trainbodyblend.obj"));//80
		GameEmGine::addModel(&*std::next(mod.begin(), 80));
		*std::next(mod.begin(), 81) = (Model(*std::next(mod.begin(), 80)));//81
		GameEmGine::addModel(&*std::next(mod.begin(), 81));
		*std::next(mod.begin(), 82) = (Model(*std::next(mod.begin(), 81)));//82
		GameEmGine::addModel(&*std::next(mod.begin(), 82));
		*std::next(mod.begin(), 83) = (Model(*std::next(mod.begin(), 82)));//83
		GameEmGine::addModel(&*std::next(mod.begin(), 83));
		*std::next(mod.begin(), 84) = (Model(*std::next(mod.begin(), 83)));//84
		GameEmGine::addModel(&*std::next(mod.begin(), 84));
		*std::next(mod.begin(), 85) = (Model("Models/Train/Head/trainhead.obj"));//85
		GameEmGine::addModel(&*std::next(mod.begin(), 85));
		(&*std::next(mod.begin(), 79))->translate(-14.45f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 80))->translate(-9.2f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 81))->translate(-4.6f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 82))->translate(0.0f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 83))->translate(4.6f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 84))->translate(9.2f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 85))->translate(14.45f, 0.3f, 8.0f), (&*std::next(mod.begin(), 85))->rotate(Coord3D<>(0, 180, 0));

		//Rail
		*std::next(mod.begin(), 86) = (Model("Models/Rail/rail.obj"));//86
		GameEmGine::addModel(&*std::next(mod.begin(), 86));
		*std::next(mod.begin(), 87) = (Model(*std::next(mod.begin(), 86)));//87
		GameEmGine::addModel(&*std::next(mod.begin(), 87));
		*std::next(mod.begin(), 88) = (Model(*std::next(mod.begin(), 87)));//88
		GameEmGine::addModel(&*std::next(mod.begin(), 88));
		*std::next(mod.begin(), 89) = (Model(*std::next(mod.begin(), 88)));//89
		GameEmGine::addModel(&*std::next(mod.begin(), 89));
		*std::next(mod.begin(), 90) = (Model(*std::next(mod.begin(), 89)));//90
		GameEmGine::addModel(&*std::next(mod.begin(), 90));
		*std::next(mod.begin(), 91) = (Model(*std::next(mod.begin(), 90)));//91
		GameEmGine::addModel(&*std::next(mod.begin(), 91));
		*std::next(mod.begin(), 92) = (Model(*std::next(mod.begin(), 91)));//92
		GameEmGine::addModel(&*std::next(mod.begin(), 92));

		(&*std::next(mod.begin(), 86))->scale(0.7f), (&*std::next(mod.begin(), 86))->translate(-18.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 86))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 87))->scale(0.7f), (&*std::next(mod.begin(), 87))->translate(-12.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 87))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 88))->scale(0.7f), (&*std::next(mod.begin(), 88))->translate(-6.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 88))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 89))->scale(0.7f), (&*std::next(mod.begin(), 89))->translate(0.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 89))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 90))->scale(0.7f), (&*std::next(mod.begin(), 90))->translate(6.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 90))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 91))->scale(0.7f), (&*std::next(mod.begin(), 91))->translate(12.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 91))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 92))->scale(0.7f), (&*std::next(mod.begin(), 92))->translate(18.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 92))->rotate(Coord3D<>(0, 90, 0));


		//Escape pods
		*std::next(mod.begin(), 94) = (Model("Models/TrainGrayBox.obj"));//94
		GameEmGine::addModel(&*std::next(mod.begin(), 94));
		*std::next(mod.begin(), 95) = (Model(*std::next(mod.begin(), 94)));//95
		GameEmGine::addModel(&*std::next(mod.begin(), 95));
		*std::next(mod.begin(), 96) = (Model(*std::next(mod.begin(), 94)));//96
		GameEmGine::addModel(&*std::next(mod.begin(), 96));
		*std::next(mod.begin(), 97) = (Model(*std::next(mod.begin(), 94)));//97
		GameEmGine::addModel(&*std::next(mod.begin(), 97));

		//yeah we arent using these anytime soon so :) 
		(&*std::next(mod.begin(), 94))->setToRender(false);
		(&*std::next(mod.begin(), 95))->setToRender(false);
		(&*std::next(mod.begin(), 96))->setToRender(false);
		(&*std::next(mod.begin(), 97))->setToRender(false);

		//RailLight
		*std::next(mod.begin(), 99) = (Model("Models/Rail/railLight.obj"));//99
		GameEmGine::addModel(&*std::next(mod.begin(), 99));
		*std::next(mod.begin(), 100) = (Model(*std::next(mod.begin(), 99)));//100
		GameEmGine::addModel(&*std::next(mod.begin(), 100));
		*std::next(mod.begin(), 101) = (Model(*std::next(mod.begin(), 100)));//101
		GameEmGine::addModel(&*std::next(mod.begin(), 101));
		*std::next(mod.begin(), 102) = (Model(*std::next(mod.begin(), 101)));//102
		GameEmGine::addModel(&*std::next(mod.begin(), 102));
		*std::next(mod.begin(), 103) = (Model(*std::next(mod.begin(), 102)));//103
		GameEmGine::addModel(&*std::next(mod.begin(), 103));
		*std::next(mod.begin(), 104) = (Model(*std::next(mod.begin(), 103)));//104
		GameEmGine::addModel(&*std::next(mod.begin(), 104));
		*std::next(mod.begin(), 105) = (Model(*std::next(mod.begin(), 104)));//105
		GameEmGine::addModel(&*std::next(mod.begin(), 105));

		////Background
		//*std::next(mod.begin(), 106) = (Model("Models/BackgroundSky/sky.obj"));//106
		//GameEmGine::addModel(&*std::next(mod.begin(), 106));
		//(&*std::next(mod.begin(), 106))->scale(8.0f, 8.0f, 5.0f), (&*std::next(mod.begin(), 106))->translate(1.0f, 4.0f, 40.0f), (&*std::next(mod.begin(), 106))->rotate({90.0f,0.0f,0.0f});

		//Add more buildings in the back
		*std::next(mod.begin(), 107) = (Model("Models/Buildings/Building7/PharmacureBuilding.obj"));//107
		GameEmGine::addModel(&*std::next(mod.begin(), 107));

		(&*std::next(mod.begin(), 107))->scale(1.5f, 1.5f, 1.0f), (&*std::next(mod.begin(), 107))->translate(2.5f, 0.0f, 30.0f);

		*std::next(mod.begin(), 108) = (Model("Models/Buildings/Building1/building1.obj"));//108
		GameEmGine::addModel(&*std::next(mod.begin(), 108));
		*std::next(mod.begin(), 109) = (Model(*std::next(mod.begin(), 108)));//109
		GameEmGine::addModel(&*std::next(mod.begin(), 109));

		(&*std::next(mod.begin(), 108))->scale(2.0f, 3.5f, 2.5f), (&*std::next(mod.begin(), 108))->translate(-6.0f, 0.0f, 37.0f), (&*std::next(mod.begin(), 108))->rotate({0.0f, -90.0f, 0.0f});
		(&*std::next(mod.begin(), 109))->scale(2.0f, 3.5f, 2.5f), (&*std::next(mod.begin(), 109))->translate(25.2f, 0.0f, 18.0f), (&*std::next(mod.begin(), 109))->rotate({0.0f, 180.0f, 0.0f});

		*std::next(mod.begin(), 110) = (Model("Models/Buildings/Building2/building2.obj"));//110
		GameEmGine::addModel(&*std::next(mod.begin(), 110));
		*std::next(mod.begin(), 111) = (Model(*std::next(mod.begin(), 110)));//111
		GameEmGine::addModel(&*std::next(mod.begin(), 111));

		(&*std::next(mod.begin(), 110))->scale(2.0f, 3.5f, 2.5f), (&*std::next(mod.begin(), 110))->translate(-22.0f, 0.0f, 15.0f), (&*std::next(mod.begin(), 110))->rotate({0.0f, 0.0f, 0.0f});
		(&*std::next(mod.begin(), 111))->scale(1.0f, 3.5f, 2.5f), (&*std::next(mod.begin(), 111))->translate(5.0f, 0.0f, 37.0f), (&*std::next(mod.begin(), 111))->rotate({0.0f, -90.0f, 0.0f});

		*std::next(mod.begin(), 112) = (Model("Models/Buildings/Building8/Pharmacure_Model.obj"));//112
		GameEmGine::addModel(&*std::next(mod.begin(), 112));
		*std::next(mod.begin(), 113) = (Model(*std::next(mod.begin(), 112)));//113
		GameEmGine::addModel(&*std::next(mod.begin(), 113));

		(&*std::next(mod.begin(), 112))->scale(1.0f, 1.0f, 1.0f), (&*std::next(mod.begin(), 112))->translate(17.0f, 0.0f, 22.0f), (&*std::next(mod.begin(), 112))->rotate({0.0f, -90.0f, 0.0f});
		(&*std::next(mod.begin(), 113))->scale(2.0f, 2.0f, 2.0f), (&*std::next(mod.begin(), 113))->translate(-25.0f, 0.0f, 25.0f), (&*std::next(mod.begin(), 113))->rotate({0.0f, 90.0f, 0.0f});

		*std::next(mod.begin(), 114) = (Model("Models/Buildings/Building3/House.obj"));//114
		GameEmGine::addModel(&*std::next(mod.begin(), 114));

		(&*std::next(mod.begin(), 114))->scale(2.0f, 2.0f, 2.0f), (&*std::next(mod.begin(), 114))->translate(25.0f, 0.0f, 10.0f), (&*std::next(mod.begin(), 114))->rotate({0.0f, 90.0f, -90.0f});

		*std::next(mod.begin(), 115) = (Model("Models/Buildings/Building9/cyber1.obj"));//115
		GameEmGine::addModel(&*std::next(mod.begin(), 115));
		*std::next(mod.begin(), 116) = (Model(*std::next(mod.begin(), 115)));//116
		GameEmGine::addModel(&*std::next(mod.begin(), 116));

		(&*std::next(mod.begin(), 115))->scale(3.0f, 3.0f, 3.0f), (&*std::next(mod.begin(), 115))->translate(-22.0f, 0.0f, 35.0f), (&*std::next(mod.begin(), 115))->rotate({0.0f, 45.0f, 0.0f});
		(&*std::next(mod.begin(), 116))->scale(3.0f, 3.0f, 3.0f), (&*std::next(mod.begin(), 116))->translate(13.5f, 0.0f, 35.0f), (&*std::next(mod.begin(), 116))->rotate({0.0f, 0.0f, 0.0f});

		*std::next(mod.begin(), 117) = (Model("Models/Buildings/Building10/cyber2.obj"));//117
		GameEmGine::addModel(&*std::next(mod.begin(), 117));
		*std::next(mod.begin(), 118) = (Model(*std::next(mod.begin(), 117)));//118
		GameEmGine::addModel(&*std::next(mod.begin(), 118));

		(&*std::next(mod.begin(), 117))->scale(3.0f, 3.0f, 3.0f), (&*std::next(mod.begin(), 117))->translate(21.0f, 0.0f, 27.0f), (&*std::next(mod.begin(), 117))->rotate({0.0f, 90.0f, 0.0f});
		(&*std::next(mod.begin(), 118))->scale(3.0f, 3.0f, 3.0f), (&*std::next(mod.begin(), 118))->translate(-0.5f, 2.0f, 36.0f), (&*std::next(mod.begin(), 118))->rotate({0.0f, 90.0f, 0.0f});

		*std::next(mod.begin(), 119) = (Model("Models/Buildings/Building7/PharmacureBuilding.obj"));//119
		GameEmGine::addModel(&*std::next(mod.begin(), 119));
		(&*std::next(mod.begin(), 119))->scale(1.0f, 3.0f, 1.0f), (&*std::next(mod.begin(), 119))->translate(17.f, 0.0f, 30.0f), (&*std::next(mod.begin(), 119))->rotate({0.0f, -90.0f, 0.0f});

		*std::next(mod.begin(), 120) = (Model("Models/Buildings/Building11/cyber3.obj"));//120
		GameEmGine::addModel(&*std::next(mod.begin(), 120));
		*std::next(mod.begin(), 121) = (Model(*std::next(mod.begin(), 120)));//121
		GameEmGine::addModel(&*std::next(mod.begin(), 121));
		(&*std::next(mod.begin(), 120))->scale(2.0f, 3.0f, 3.0f), (&*std::next(mod.begin(), 120))->translate(-17.f, -5.0f, 24.0f), (&*std::next(mod.begin(), 120))->rotate({0.0f, 0.0f, 0.0f});
		(&*std::next(mod.begin(), 121))->scale(2.0f, 2.0f, 2.0f), (&*std::next(mod.begin(), 121))->translate(-4.2f, -5.0f, 29.7f), (&*std::next(mod.begin(), 121))->rotate({0.0f, -90.0f, 0.0f});

		*std::next(mod.begin(), 122) = (Model("Models/Buildings/Building5/smallShop.obj"));//122
		GameEmGine::addModel(&*std::next(mod.begin(), 122));
		(&*std::next(mod.begin(), 122))->scale(1.2f, 1.2f, 1.2f), (&*std::next(mod.begin(), 122))->translate(-8.0f, 0.0f, 27.0f), (&*std::next(mod.begin(), 122))->rotate({0.0f, -90.0f, 0.0f});

		//Building 2s
		*std::next(mod.begin(), 123) = (Model("Models/Buildings/Tunnel/Tunnel_Front_Blue.obj")); //123
		GameEmGine::addModel(&*std::next(mod.begin(), 123));
		*std::next(mod.begin(), 124) = (Model(*std::next(mod.begin(), 123)));
		GameEmGine::addModel(&*std::next(mod.begin(), 124));//124
		(&*std::next(mod.begin(), 123))->setColour({0,255,255});
		(&*std::next(mod.begin(), 124))->setColour({0,255,255});

		//boss portrait beside its health bar 
		(*std::next(mod.begin(), 125)) = (Model("Models/BOSS/bossPORTRAIT.obj")); //125
		(&*std::next(mod.begin(), 125))->translate(10.2f, 18.2f, 19.2f);
		(&*std::next(mod.begin(), 125))->scale(2.0f, 2.0f, 2.0f);
		(&*std::next(mod.begin(), 125))->rotate({0, 0, 0});
		(&*std::next(mod.begin(), 125))->setTransparent(true);
		GameEmGine::addModel(&*std::next(mod.begin(), 125));

		//pause screen
		(*std::next(mod.begin(), 126)) = (Model("Models/Scene/Pause/pausedScreen.obj")); //125
		(&*std::next(mod.begin(), 126))->translate(-0.8f, 10.0f, -8.0f);
		(&*std::next(mod.begin(), 126))->scale(0.25f, 0.45f, 0.25f);
		GameEmGine::addModel(&*std::next(mod.begin(), 126));
		(&*std::next(mod.begin(), 126))->setToRender(false);
		(&*std::next(mod.begin(), 126))->setTransparent(true);

		//Tutorial Sign
		*std::next(mod.begin(), 127) = (Model("Models/Sign/tallSign/tallSign.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 127));
		(&*std::next(mod.begin(), 127))->translate(-14.0f, 0.0f, 36.0f), (&*std::next(mod.begin(), 127))->scale(1.5f, 1.5f, 1.5f);
		(&*std::next(mod.begin(), 127))->setColour({0,255,255});

		*std::next(mod.begin(), 128) = (Model("Models/Sign/shortSign/shortSign.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 128));
		(&*std::next(mod.begin(), 128))->translate(-16.9f, 0.0f, 17.0f), (&*std::next(mod.begin(), 128))->rotate({0, 90, 0}), (&*std::next(mod.begin(), 128))->scale(0.8f, 1.0f, 1.0f);
		(&*std::next(mod.begin(), 128))->setColour({0,255,255});

		*std::next(mod.begin(), 129) = (Model("Models/Sign/sideSign/sideSign.obj"));
		GameEmGine::addModel(&*std::next(mod.begin(), 129));
		(&*std::next(mod.begin(), 129))->scale(1.0f, 3.0f, 1.0f), (&*std::next(mod.begin(), 129))->translate(16.8f, 0.0f, 29.5f), (&*std::next(mod.begin(), 129))->rotate({0.0f, -90.0f, 0.0f});

		(*std::next(mod.begin(), 130)) = (Model("Models/Scene/GameOver/gameOver.obj")); //125
		(&*std::next(mod.begin(), 130))->translate(-0.8f, 10.0f, -8.0f);
		(&*std::next(mod.begin(), 130))->scale(0.25f, 0.45f, 0.25f);
		GameEmGine::addModel(&*std::next(mod.begin(), 130));
		(&*std::next(mod.begin(), 130))->setToRender(false);
		(&*std::next(mod.begin(), 130))->setTransparent(true);

		(*std::next(mod.begin(), 131)) = (Model("Models/Scene/Win/win.obj")); //125
		(&*std::next(mod.begin(), 131))->translate(-0.8f, 10.0f, -8.0f);
		(&*std::next(mod.begin(), 131))->scale(0.25f, 0.45f, 0.25f);
		GameEmGine::addModel(&*std::next(mod.begin(), 131));
		(&*std::next(mod.begin(), 131))->setToRender(false);
		(&*std::next(mod.begin(), 131))->setTransparent(true);

		/// - Set Model Transforms - ///
		//Player Transforms
		(&*std::next(mod.begin(), 0))->scale(1.2f, 1.4f, 1.2f), (&*std::next(mod.begin(), 0))->translate(1.0f, 0.0f, -5.0f);
		(&*std::next(mod.begin(), 1))->scale(1.2f, 1.4f, 1.2f), (&*std::next(mod.begin(), 1))->translate(-1.0f, 0.0f, -5.0f);
		(&*std::next(mod.begin(), 2))->scale(1.2f, 1.4f, 1.2f), (&*std::next(mod.begin(), 2))->translate(2.0f, 0.0f, -5.0f);
		(&*std::next(mod.begin(), 3))->scale(1.2f, 1.4f, 1.2f), (&*std::next(mod.begin(), 3))->translate(-2.0f, 0.0f, -5.0f);
		(&*std::next(mod.begin(), 0))->rotate(Coord3D<>(0, 180, 0));
		(&*std::next(mod.begin(), 1))->rotate(Coord3D<>(0, 180, 0));
		(&*std::next(mod.begin(), 2))->rotate(Coord3D<>(0, 180, 0));
		(&*std::next(mod.begin(), 3))->rotate(Coord3D<>(0, 180, 0));

		(&*std::next(mod.begin(), 19))->addChild(&*std::next(mod.begin(), 123));
		(&*std::next(mod.begin(), 20))->addChild(&*std::next(mod.begin(), 124));

		//Building Transforms
		//Building 1s
		(&*std::next(mod.begin(), 4))->scale(1), (&*std::next(mod.begin(), 4))->translate(-15.175f, 0.0f, -2.0f), (&*std::next(mod.begin(), 4))->rotate({0.0f,90.0f,0.0f});;
		(&*std::next(mod.begin(), 5))->scale(1), (&*std::next(mod.begin(), 5))->translate(6.0f, 0.0f, 29.0f), (&*std::next(mod.begin(), 5))->rotate({0.0f,-90.0f,0.0f});
		//&*std::next(mod.begin(),6)->scale(2), &*std::next(mod.begin(),6)->translate(-4.0f, 0.0f, 22.75f), &*std::next(mod.begin(),6)->rotate({0.0f,-90.0f,0.0f});

		//Building 2s
		(&*std::next(mod.begin(), 19))->scale(0.85f), (&*std::next(mod.begin(), 19))->translate(-18.0f, 0.0f, 6.4f), (&*std::next(mod.begin(), 19))->rotate({0.0f, 90.0f,0.0f}); //left 
		(&*std::next(mod.begin(), 20))->scale(0.85f), (&*std::next(mod.begin(), 20))->translate(18.0f, 0.0f, 9.5f), (&*std::next(mod.begin(), 20))->rotate({0.0f, -90.0f, 0.0f}); //right 

		//Buildings 3s
		(&*std::next(mod.begin(), 30))->translate(10.5f, 0.0f, 23.6f);
		(&*std::next(mod.begin(), 31))->translate(19.5f, 0.0f, 3.75f), (&*std::next(mod.begin(), 31))->rotate({0,180,0});
		(&*std::next(mod.begin(), 32))->translate(-12.0f, 0.0f, 25.35f), (&*std::next(mod.begin(), 32))->rotate({0,-90,0});

		//Building 4s //Lillian's building, moved back
		(&*std::next(mod.begin(), 33))->translate(27.0f, 0.0f, 26.0f), (&*std::next(mod.begin(), 33))->rotate({0,45,0}); //right
		(&*std::next(mod.begin(), 34))->translate(-14.0f, 0.0f, 36.0f), (&*std::next(mod.begin(), 34))->scale(1.5f, 1.5f, 1.5f), (&*std::next(mod.begin(), 34))->rotate({0,180,0}); //left
			//Building 5s
		(&*std::next(mod.begin(), 39))->scale(1.0f, 1.0f, 1.05f), (&*std::next(mod.begin(), 39))->translate(19.6f, 0.0f, 16.5f), (&*std::next(mod.begin(), 39))->rotate({0,180,0});
		(&*std::next(mod.begin(), 40))->scale(1.25f, 1.0f, 1.0f), (&*std::next(mod.begin(), 40))->translate(-16.9f, 0.0f, 16.35f), (&*std::next(mod.begin(), 40))->rotate({0,90,0});

		//Project Nebula Sign Transforms
		(&*std::next(mod.begin(), 7))->scale(3), (&*std::next(mod.begin(), 7))->translate(9.5f, 5.34f, 22.5f);

		//Boss Trarrnsforms
		(&*std::next(mod.begin(), 8))->scale(2.0f), (&*std::next(mod.begin(), 8))->translate(0.0f, 0.0f, 23.0f), (&*std::next(mod.begin(), 8))->rotate({0.0f, 0.0f, 0.0f});

		//Floor Transforms
		(&*std::next(mod.begin(), 9))->scale(2.25f, 1.0f, 5.0f), (&*std::next(mod.begin(), 9))->translate(0.0f, 0.0f, 5.0f);

		//Street Light Transforms
		(&*std::next(mod.begin(), 10))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 10))->translate(13.0f, 0.0f, -1.0f);
		(&*std::next(mod.begin(), 11))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 11))->translate(13.0f, 0.0f, 6.0f);
		(&*std::next(mod.begin(), 12))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 12))->translate(13.0f, 0.0f, 15.0f);
		(&*std::next(mod.begin(), 13))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 13))->translate(-13.0f, 0.0f, -1.0f), (&*std::next(mod.begin(), 13))->rotate({0.0f,180.0f,0.0f});
		(&*std::next(mod.begin(), 14))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 14))->translate(-13.0f, 0.0f, 6.0f), (&*std::next(mod.begin(), 14))->rotate({0.0f,180.0f,0.0f});
		(&*std::next(mod.begin(), 15))->scale(0.5f, 0.8f, 0.5f), (&*std::next(mod.begin(), 15))->translate(-13.0f, 0.0f, 15.0f), (&*std::next(mod.begin(), 15))->rotate({0.0f,180.0f,0.0f});

		//Bench Transforms
		(&*std::next(mod.begin(), 16))->translate(-13.0f, 0.0f, 3.0f);
		(&*std::next(mod.begin(), 17))->translate(13.0f, 0.0f, 3.0f), (&*std::next(mod.begin(), 17))->rotate({0.0f,180.0f,0.0f});

		//Planet Transforms
		(&*std::next(mod.begin(), 18))->translate(9.0f, 17.0f, 36.0f);
		//&*std::next(mod.begin(),58)->translate(-10.0f, 11.0f, 25.0f);

		//Trees
		(&*std::next(mod.begin(), 35))->scale(0.3f), (&*std::next(mod.begin(), 35))->translate(13.0f, 0.0f, -3.0f), (&*std::next(mod.begin(), 35))->rotate({0,-0,0});
		(&*std::next(mod.begin(), 36))->scale(0.3f), (&*std::next(mod.begin(), 36))->translate(-13.0f, 0.0f, -3.0f), (&*std::next(mod.begin(), 36))->rotate({0,-0,0});
		(&*std::next(mod.begin(), 37))->scale(0.3f), (&*std::next(mod.begin(), 37))->translate(13.0f, 0.0f, 11.0f), (&*std::next(mod.begin(), 37))->rotate({0,-0,0});
		(&*std::next(mod.begin(), 38))->scale(0.3f), (&*std::next(mod.begin(), 38))->translate(-13.0f, 0.0f, 11.0f), (&*std::next(mod.begin(), 38))->rotate({0,-0,0});

		//Pizza Sign
		(&*std::next(mod.begin(), 53))->scale(1.5f), (&*std::next(mod.begin(), 53))->translate(-13.0f, 5.4f, 22.3f);

		//Train
		(&*std::next(mod.begin(), 79))->translate(-14.45f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 80))->translate(-9.2f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 81))->translate(-4.6f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 82))->translate(0.0f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 83))->translate(4.6f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 84))->translate(9.2f, 0.3f, 8.0f);
		(&*std::next(mod.begin(), 85))->translate(14.45f, 0.3f, 8.0f), (&*std::next(mod.begin(), 85))->rotate(Coord3D<>(0, 180, 0));

		//Rail
		(&*std::next(mod.begin(), 86))->scale(0.7f), (&*std::next(mod.begin(), 86))->translate(-18.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 86))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 87))->scale(0.7f), (&*std::next(mod.begin(), 87))->translate(-12.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 87))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 88))->scale(0.7f), (&*std::next(mod.begin(), 88))->translate(-6.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 88))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 89))->scale(0.7f), (&*std::next(mod.begin(), 89))->translate(0.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 89))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 90))->scale(0.7f), (&*std::next(mod.begin(), 90))->translate(6.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 90))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 91))->scale(0.7f), (&*std::next(mod.begin(), 91))->translate(12.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 91))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 92))->scale(0.7f), (&*std::next(mod.begin(), 92))->translate(18.0f, 0.0f, 8.0f), (&*std::next(mod.begin(), 92))->rotate(Coord3D<>(0, 90, 0));

		//RialLight
		(&*std::next(mod.begin(), 99))->scale(0.7f), (&*std::next(mod.begin(), 99))->translate(-18.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 99))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 100))->scale(0.7f), (&*std::next(mod.begin(), 100))->translate(-12.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 100))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 101))->scale(0.7f), (&*std::next(mod.begin(), 101))->translate(-6.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 101))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 102))->scale(0.7f), (&*std::next(mod.begin(), 102))->translate(0.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 102))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 103))->scale(0.7f), (&*std::next(mod.begin(), 103))->translate(6.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 103))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 104))->scale(0.7f), (&*std::next(mod.begin(), 104))->translate(12.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 104))->rotate(Coord3D<>(0, 90, 0));
		(&*std::next(mod.begin(), 105))->scale(0.7f), (&*std::next(mod.begin(), 105))->translate(18.0f, 0.03f, 8.0f), (&*std::next(mod.begin(), 105))->rotate(Coord3D<>(0, 90, 0));
	#pragma endregion
	#endif // FALSE

		SceneManager::loadScene("gamescene.scene", testSceneLoad);

		GameEmGine::clearObjectList();
		for(auto& a : testSceneLoad)
			GameEmGine::addModel(&a);

		//apply uniform transform
		for(auto& a : mod)
			uniScaler.addChild(&a);

		//lights.resize(14);
		for(int a = 0; a < 6; a++)
		{
			lights.push_back(Light());
			//&*std::next(mod.begin(),10 + a)->boundingBoxUpdate();
			(*std::next(lights.begin(), a)).setLightType(Light::DIRECTIONAL);
			(*std::next(lights.begin(), a)).setParent(&*std::next(mod.begin(), 10 + a));
			(*std::next(lights.begin(), a)).translate({-0.5f,4.5,0.0f});
			//(&*std::next(lights.begin(),a).setDiffuse({ 255,100,0,100 }, 6);
			//(&*std::next(lights.begin(),a).setAttenuationQuadratic(0.06f, 6);
		}

		for(int a = 0; a < 4; a++)
		{
			lights.push_back(Light());
			(*std::next(lights.begin(), a + 6)).setLightType(Light::POINT);
			(*std::next(lights.begin(), a + 6)).setParent(&*std::next(mod.begin(), a));
			(*std::next(lights.begin(), a + 6)).translate({0, -0.75f, 0});
			(*std::next(lights.begin(), a + 6)).setAttenuationQuadratic(1.f);
		}

		(*std::next(lights.begin(), 6)).setDiffuse({255,0,0,100});
		(*std::next(lights.begin(), 7)).setDiffuse({0,0,255,100});
		(*std::next(lights.begin(), 8)).setDiffuse({0,255,0,100});
		(*std::next(lights.begin(), 9)).setDiffuse({255,255,0});



		//(&*std::next(lights.begin(),a).setSceneAmbient({255,255,255,255});

		/// - Set Camera  - ///
		//*std::next(mod.begin(),130)->translate(-0.8f, 10.0f, -8.0f);
		GameEmGine::getMainCamera()->translate({0,15.5f,-17.5});
		GameEmGine::getMainCamera()->rotate({-25,0,0});

		/// key/mouse input ///
		keyPressed = [&](int a, int b) {keyInputPressed(a, b); };
		keyReleased = [&](int a, int b) {keyInputReleased(a, b); };
		mouseReleased = [&](int a, int b) {mouseButtonReleased(a, b); };

		AudioPlayer::init();

		audio.createAudioStream("Audio/potential mix (with beat).wav", "BG Music");

		audio.setVolume(0.6f);
		audio.play(true);
	}

	void insertionSort(std::list<Minion*>& arr, Model* checker)
	{
		int i, j;
		float key;
		for(i = 1; i < (int)arr.size(); i++)
		{
			key = Coord3D<>::distance((*std::next(arr.begin(), i))->getLocalPosition(), checker->getLocalPosition());
			Minion* tmp = (*std::next(arr.begin(), i));
			j = i - 1;

			/*
				Move elements of (*std::next(arr.begin(),0..i-1], that are
				greater than key, to one position ahead
				of their current position
			*/
			while(j >= 0 && Coord3D<>::distance((*std::next(arr.begin(), j))->getLocalPosition(), checker->getLocalPosition()) > key)
			{
				(*std::next(arr.begin(), j + 1)) = (*std::next(arr.begin(), j));
				j = j - 1;
			}
			(*std::next(arr.begin(), j + 1)) = tmp;
		}
	}

	/// - The Update Loop - ///

	void update(double dt)
	{
		switch(currentState)
		{
		case CONTROL_STATE::CAMERA:
		case CONTROL_STATE::LEVEL:

			objectMovement(dt);
			break;
		case CONTROL_STATE::GAME:
			//gameUpdate(dt);
			break;
		}
	}



	void objectMovement(double dt)
	{
		float moveSpd = 15 * (float)dt;
		moveSpd;
		if(m_left)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(-moveSpd, 0, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(-moveSpd, 0, 0);
				break;
			}
		if(m_right)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(moveSpd, 0, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(moveSpd, 0, 0);
				break;
			}
		if(m_fwd)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(0, 0, moveSpd);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(0, 0, moveSpd);
				break;
			}
		if(m_back)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(0, 0, -moveSpd);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(0, 0, -moveSpd);
				break;
			}
		if(m_up)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(0, moveSpd, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(0, moveSpd, 0);
				break;
			}
		if(m_down)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->translateBy(0, -moveSpd, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->translateBy(0, -moveSpd, 0);
				break;
			}

		if(rotLeft)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->rotateBy(0, -moveSpd, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->rotateBy(0, -moveSpd, 0);
				break;
			}
		if(rotRight)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->rotateBy(0, moveSpd, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->rotateBy(0, moveSpd, 0);
				break;
			}
		if(rotUp)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				//this is correct
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->rotateBy(moveSpd, 0, 0);
				break;
			case CAMERA:
				//this is correct
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->rotateBy(moveSpd, 0, 0);
				break;
			}
		if(rotDown)
			switch(currentState)
			{
			case GAME:
				break;
			case LEVEL:
				GameEmGine::getMainCamera()->enableFPSMode(false); if(!curModel)return;
				curModel->rotateBy(-moveSpd, 0, 0);
				break;
			case CAMERA:
				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::getMainCamera()->rotateBy(-moveSpd, 0, 0);
				break;
			}

	}

	void gameUpdate(double dt)
	{
		//(*std::next(lights.begin(),a].setParent(((Boss*)&(*std::next(mod.begin(),8]).getMissials()[0], 10);
		//(*std::next(lights.begin(),a].setParent(((Boss*)&(*std::next(mod.begin(),8]).getMissials()[1], 11);
		//(*std::next(lights.begin(),a].setParent(((Boss*)&(*std::next(mod.begin(),8]).getMissials()[2], 12);
		//(*std::next(lights.begin(),a].setParent(((Boss*)&(*std::next(mod.begin(),8]).getMissials()[3], 13);


		time += (float)dt; //Add Delta Time to Time

		float move = .1f;
		Player* player;

		// Boss Variables
		Boss* CandyMan = (Boss*)&(*std::next(mod.begin(), 8)); //Set model 8 as Boss called "CandyMan"


		Model* count;


		///~ countdown stuff ~///
		if(countdown)
		{
			CandyMan->setActive(false);
			for(int a = 0; a < 4; a++)
			{
				player = (Player*)&(*std::next(mod.begin(), a));
				player->setActive(false);
			}

			count = &nums[countdownCounter];

			if(!m_init)
			{
				countdownTimer = time;
				count->rotate({-25, 0, 0});
				count->setTransparent(true);
				GameEmGine::addModel(count);
				m_init = true;
			}

			count->translate(lerp(GameEmGine::getMainCamera()->getLocalPosition() + Coord3D<>{0, -5, 7}, GameEmGine::getMainCamera()->getLocalPosition() - Coord3D<>{0, 0, 0}, (time - countdownTimer) / 3.5f));

			if(int((time - countdownTimer) / 2))
			{
				GameEmGine::removeModel(count);
				m_init = false;
				countdownCounter++;
				if(countdownCounter == 3)
				{
					countdown = false;
					CandyMan->setActive(true);
					for(int a = 0; a < 4; a++)
					{
						player = (Player*)&(*std::next(mod.begin(), a));
						player->setActive(true);
					}
				}
			}
		}

		static std::list<Player*> players;
		if(players.size() != 4)
			for(int a = 0; a < 4; ++a)
				players.push_back((Player*)&(*std::next(mod.begin(), a)));

		CandyMan->setPlayers(players);
		CandyMan->update((float)dt);

		//add mod for pause screen here but set render to false 
		for(int a = 0; a < 4; a++)
		{

			player = (Player*)&(*std::next(mod.begin(), a));
			player->setPlayerIndex(a);

			static bool pausedAgain[4] = {0,0,0,0};

			if(GameEmGine::isControllerConnected(a))
			{
				/// - Game Over - ///
				if(gameOver)
				{
					//pausedAgain[a] = true;
					for(int b = 0; b < 4; b++)
						(*(Player*)&(*std::next(mod.begin(), b))).setActive(pause);

					//rn the music gets quieter during the pause screen 
					if(!pause)
						EmGineAudioPlayer::setVolume(.3f, 0);

					else
						EmGineAudioPlayer::setVolume(0.6f, 0);

					(*std::next(mod.begin(), 130)).rotate(GameEmGine::getMainCamera()->getLocalRotation() * Coord3D { -1.f, 1.f, 1.f }); //should be parallel to camera hopefully 
					(*std::next(mod.begin(), 130)).translate(GameEmGine::getMainCamera()->getLocalPosition() + Coord3D{-0.8f, -5.5f, 8.5f}); //should be parallel to camera hopefully 

					(*std::next(mod.begin(), 130)).setToRender(!pause);
					CandyMan->setActive(pause);

					if(fadeout)
					{
						splashT += 0.01f;
						splashT = splashT > 1 ? 1 : splashT;
						splashAmbient = (GLubyte)lerp(255, 0, splashT);
						(*std::next(lights.begin(), a)).setAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
						if(splashAmbient <= 2)
						{
							fadein = true;
							fadeout = false;
							splashT = 0;
							splashAmbient = 255;

							GameEmGine::setScene((getParent()->getParent()->getParent())); // Can't set new scene from game for some reason?
						}
					}
					if(GameEmGine::getController(a)->isButtonStroked(CONTROLLER_A))
					{
						fadeout = true;
					}
					if(GameEmGine::getController(a)->isButtonStroked(CONTROLLER_B))
					{
						GameEmGine::exit();
					}
				}

				/// - GAme Win - ///
				if(gameWin)
				{
					//pausedAgain[a] = true;
					for(int b = 0; b < 4; b++)
						(*(Player*)&(*std::next(mod.begin(), b))).setActive(pause);

					//rn the music gets quieter during the pause screen 
					if(!pause)
						EmGineAudioPlayer::setVolume(.3f, 0);

					else
						EmGineAudioPlayer::setVolume(0.6f, 0);

					(*std::next(mod.begin(), 131)).rotate(GameEmGine::getMainCamera()->getLocalRotation()); //should be parallel to camera hopefully 
					(*std::next(mod.begin(), 131)).setToRender(!pause);
					CandyMan->setActive(pause);

					if(GameEmGine::getController(a)->isButtonStroked(CONTROLLER_A))
					{
						splashT += 0.01f;
						splashT = splashT > 1 ? 1 : splashT;
						splashAmbient = (GLubyte)lerp(255, 0, splashT);
						(*std::next(lights.begin(), a)).setAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
						if(splashAmbient <= 5)
						{
							fadein = true;
							fadeout = false;
							splashT = 0;
							splashAmbient = 255;

							//GamePlayInit();
							//GameEmGine::setScene(new Controls); // Can't set new scene from game for some reason?
						}
					}
					if(GameEmGine::getController(a)->isButtonStroked(CONTROLLER_B))
					{
						GameEmGine::exit();
					}
				}

				//music should slow down in the pause menu!!!!
				if(GameEmGine::getController(a)->isButtonPressed(CONTROLLER_START) && !pausedAgain[a])
				{
					pausedAgain[a] = true;
					for(int b = 0; b < 4; b++)
						(*(Player*)&(*std::next(mod.begin(), b))).setActive(pause);

					//rn the music gets quieter during the pause screen 
					if(!pause)
						EmGineAudioPlayer::setVolume(.3f, 0);
					else
						EmGineAudioPlayer::setVolume(0.6f, 0);

					(*std::next(mod.begin(), 126)).rotate(GameEmGine::getMainCamera()->getLocalRotation()); //should be parallel to camera hopefully 
					(*std::next(mod.begin(), 126)).setToRender(!pause);
					CandyMan->setActive(pause);
					//music should slow down in the pause menu!!!!
					pause = !pause;
					screenPause = !screenPause;

				}
				else if(GameEmGine::getController(a)->isButtonReleased(CONTROLLER_START))
				{
					pausedAgain[a] = false;
					//pauseScreen[a] = false; 
					//(*std::next(mod.begin(),126].setToRender(false);
				}

				if(!player->dead)
					deathCounter = 0;
			}
			else
			{
				(*(Player*)&(*std::next(mod.begin(), a))).setActive(false);
			}
			player->update((float)dt);

			player->onPlayArea(&(*std::next(mod.begin(), 59)));

			//bullet collisions with boss
			if(player->bulletCollisions(CandyMan))
			{
				CandyMan->setHealth(CandyMan->getHealth() - 10);

				CandyMan->bossFlash = true;

			}
			if(CandyMan->getHealth() <= 0)
			{
				gameWin = true;
			}

			//bullet collision with minions
			for(auto& minion : CandyMan->minions)
			{
				if(player->bulletCollisions(minion))
					minion->setHealth(minion->getHealth() - 10);
				if(player->collision2D(minion, {0,1,0}))
				{
					player->hitByEnemy(minion, 5);
					player->translateBy(minion->moveTo * 3);
				}
			}

			if(player->type == specialist)
				if(!((Specialist*)player)->pTurrets.empty())
					insertionSort(CandyMan->minions, ((Specialist*)player)->pTurrets[0]);



			for(int b = 0; b < 7; b++)
			{
				player->bulletCollisions(&(*std::next(mod.begin(), 79 + b)));
				if(player->collision2D(&(*std::next(mod.begin(), 79 + b)), {0,1,0}))
				{

					player->translateBy(((XinputController*)GameEmGine::getController(a))->getSticks()[LS].x * -move * 1.1f, 0,
										((XinputController*)GameEmGine::getController(a))->getSticks()[LS].y * move *
										player->getLocalPosition().z < (*std::next(mod.begin(), 79 + b)).getLocalPosition().z ? -1 : 1 * 1.1f); //move player back

				}

			}

			switch(player->type)
			{
			case assault:


				for(int b = 0; b < (int)(*(Assault*)player).pMissiles.size(); b++)
				{
					CandyMan->hitByEnemy((*(Assault*)player).pMissiles[b], 50);
				}
				(*(Assault*)player).missileCollision(CandyMan);

				for(int t = 79; t < 79 + 7; t++)
				{
					(*(Assault*)player).missileCollision(&(*std::next(mod.begin(), t)));
				}



				break;
			case tank:
				//nothing special needed
				break;
			case medic:
				for(int b = 0; b < 4; b++)
					if((*(Medic*)player).isHealing)
						(*(Medic*)player).getHealing((Player*)&(*std::next(mod.begin(), b)));
				break;
			case specialist:
				for(auto& b : (*(Specialist*)player).pTurrets)
				{
					b->setMinions(CandyMan->minions);
				}

				if((*(Specialist*)player).hitTurret(CandyMan))
				{
					//do something?
				}

				//bullet collision with minions
				for(auto& minion : CandyMan->minions)
				{
					if((*(Specialist*)player).hitTurret(minion))
					{
						//do something?

					}
				}

				for(auto& missile : CandyMan->getMissials())
					if((*(Specialist*)player).hitTurret(missile.get()))
					{
						//do something?
					}

				break;
			}

		}




		/// - Train Car Movement - ///
		//Train Sits in middle of map
		if(0 <= (time - trainTimer) && 10 > (time - trainTimer))
		{
			(*std::next(mod.begin(), 123)).setColour({255, 0, 0});
			(*std::next(mod.begin(), 124)).setColour({255, 0, 0});
			for(int t = 0; t < 7; t++)
			{
				if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {false,true,false}))
				{

					player->translate(
						abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
						0,
						abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);

					//if(player->getLocalPosition().z < (*std::next(mod.begin(),79 + t].getLocalPosition().z)
					//	player->translate(player->getLocalPosition() + Coord3D<>(0.0f, 0.f, -0.1f));
					//if(player->getLocalPosition().z > (*std::next(mod.begin(),79 + t].getLocalPosition().z)
					//	player->translate(player->getLocalPosition() + Coord3D<>(0.0f, 0.f, 0.1f));
				}
			}
			trainInit = false;
		}
		//Train Moves off map
		else if(10 <= (time - trainTimer) && 13 > (time - trainTimer))
		{
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{0.2f, 0.f, 0.f});//Move train cars right
				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {false,true,false}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
			trainInit = false;
		}
		else if(13 <= (time - trainTimer) && 20 > (time - trainTimer))
		{
			if(!trainInit)
			{
				audio.createAudioStream("Audio/RailOff.wav", "Train Off");
				audio.play();
				trainInit = true;
			}
			(*std::next(mod.begin(), 123)).setColour({0, 255, 255});
			(*std::next(mod.begin(), 124)).setColour({0, 255, 255});
			for(int i = 99; i <= 105; i++)
			{

				(*std::next(mod.begin(), i)).translate((*std::next(mod.begin(), i)).getLocalPosition().x, -1.0f, (*std::next(mod.begin(), i)).getLocalPosition().z);
			}

			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{0.2f, 0.f, 0.f});//Move train cars right
				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {false,true,false}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}

		}
		//Train stops
		else if(20 <= (time - trainTimer) && 30 > (time - trainTimer))
		{
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{0.0f, 0.f, 0.f});//Stop Train cars
			}
			trainInit = false;
		}
		//Train moves back onto map
		else if(30 <= (time - trainTimer) && 37 > (time - trainTimer))
		{
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right


				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
			trainInit = false;
		}
		// Tunnel starts blinking 
		else if(37 <= (time - trainTimer) && 37.5f > (time - trainTimer))
		{
			if(!trainInit)
			{
				audio.createAudioStream("Audio/RailOff.wav", "Train Off");
				audio.play();
				trainInit = true;
			}

			(*std::next(mod.begin(), 123)).setColour({255, 0, 0});
			(*std::next(mod.begin(), 124)).setColour({255, 0, 0});
			for(int i = 99; i <= 105; i++)
			{
				(*std::next(mod.begin(), i)).translate((*std::next(mod.begin(), i)).getLocalPosition().x, 0.03f, (*std::next(mod.begin(), i)).getLocalPosition().z);

			}

			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right

				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
		}
		else if(37.5f <= (time - trainTimer) && 38 > (time - trainTimer))
		{
			(*std::next(mod.begin(), 123)).setColour({0, 255, 255});
			(*std::next(mod.begin(), 124)).setColour({0, 255, 255});
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right


				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
			trainInit = false;
		}
		else if(38 <= (time - trainTimer) && 38.5f > (time - trainTimer))
		{
			(*std::next(mod.begin(), 123)).setColour({255, 0, 0});
			(*std::next(mod.begin(), 124)).setColour({255, 0, 0});
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right
				if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
				{
					player->setHealth(player->getHealth() - 10);
					player->translate(
						abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
						0,
						abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
				}
			}
			trainInit = false;
		}
		else if(38.5f <= (time - trainTimer) && 39 > (time - trainTimer))
		{
			(*std::next(mod.begin(), 123)).setColour({0, 255, 255});
			(*std::next(mod.begin(), 124)).setColour({0, 255, 255});
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right

				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
			trainInit = false;
		}
		else if(39 <= (time - trainTimer) && 40 > (time - trainTimer))
		{
			(*std::next(mod.begin(), 123)).setColour({255, 0, 0});
			(*std::next(mod.begin(), 124)).setColour({255, 0, 0});
			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translateBy(Coord3D{-0.2f, 0.f, 0.f});//Move train cars back to the right

				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->setHealth(player->getHealth() - 10);
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
				}
			}
			trainInit = false;
		}
		//Train stops on map
		else if(40 <= (time - trainTimer) && 50 > (time - trainTimer))
		{

			for(int t = 0; t < 7; t++)
			{
				(*std::next(mod.begin(), 79 + t)).translate((*std::next(mod.begin(), 79 + t)).getLocalPosition() + Coord3D{0.00f, 0.f, 0.f});//Stop Train cars on map

				for(int a = 0; a < 4; a++)
				{
					player = (Player*)&(*std::next(mod.begin(), a));
					if(Model::collision2D(&(*std::next(mod.begin(), 79 + t)), player, {0,1,0}))
					{
						player->translate(
							abs(player->getLocalPosition().x) > (*std::next(mod.begin(), 79 + t)).getWidth() / 2 ? player->getLocalPosition().x < 0 ? -(*std::next(mod.begin(), 79 + t)).getWidth() / 2 : (*std::next(mod.begin(), 79 + t)).getWidth() / 2 : player->getLocalPosition().x,
							0,
							abs(player->getLocalPosition().z) >(*std::next(mod.begin(), 79 + t)).getDepth() / 2 ? player->getLocalPosition().z < 0 ? -(*std::next(mod.begin(), 79 + t)).getDepth() / 2 : (*std::next(mod.begin(), 79 + t)).getDepth() / 2 : player->getLocalPosition().z);
					}
					trainTimer += time; //Reset Train timer so it all starts again.
				}
			}
			trainTimer = time;
			trainInit = false;
		}


		if(!pause)
		{
			CandyMan->setActive(true);
			for(int a = 0; a < 4; a++)
				(*(Player*)&(*std::next(mod.begin(), a))).setActive(true);
		}

		/// - If game not m_active and Camera is m_active (Move camera mode) - ///
		if(!movePlayer)
		{
			for(int a = 0; a < 4; a++)
				(*(Player*)&(*std::next(mod.begin(), a))).setActive(false);

			CandyMan->setActive(false);
			deathCounter = 0;

			if(GameEmGine::isControllerConnected(0))
			{

				XinputController* p1 = (XinputController*)GameEmGine::getController(0);

				//move camera
				move *= 2;

				GameEmGine::getMainCamera()->enableFPSMode();
				GameEmGine::translateCameraBy({p1->getSticks()[LS].x * move , 0 * move, p1->getSticks()[LS].y * move});//move camera
				GameEmGine::rotateCameraBy(ang * (abs(p1->getSticks()[RS].x) + abs(p1->getSticks()[RS].y)) * Coord3D<>{p1->getSticks()[RS].y, p1->getSticks()[RS].x, 0});//rotate camera
				//GAME::getMainCamera().rotateBy({ ang *p1->getSticks()[RS].y ,ang *p1->getSticks()[RS].x ,0}, { p1->getSticks()[RS].y  ,p1->getSticks()[RS].x, 0 });
				GameEmGine::translateCameraBy({0 ,p1->getTriggers().LT * -move,0});//move out
				GameEmGine::translateCameraBy({0 ,p1->getTriggers().RT * move,0});//move out
				move /= 2;
			}
		}
		//GameEmGine::m_grayScalePost.enable();
		//glUniform1f(GameEmGine::m_grayScalePost.getUniformLocation("uTime"), deathCounter);
		//GameEmGine::m_grayScalePost.disable();

		if(deathCounter)
			gameOver = true;
		deathCounter += .01f;
		deathCounter = deathCounter <= 1 ? deathCounter : 1;
	}

private:
	bool m_init = false;

	enum CONTROL_STATE:char
	{
		CAMERA,
		LEVEL,
		GAME,
	}currentState;

	list<Model> testSceneLoad;
	std::list<Model> mod;
	std::list<Light> lights;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;

	float ang = 2;
	Model* curModel = nullptr;
	bool m_left = 0, m_right = 0, m_fwd = 0, m_back = 0, m_up = 0, m_down = 0,
		rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0, movePlayer = true;

	Coord2D<> leftM, rightM;
	AudioPlayer audio;

	bool gameOver = false;
	bool gameWin = false;
	bool screenPause = false;
	bool pause = false;

	//Time
	float  time = 0;
	float deathCounter = 0;

	//Train Variables
	float trainTimer = 0; //Determines when train comes and goes
	vector<float> timer[4];

	//Countdown Variables
	Model nums[3]{Model("Models/COUNTDOWN/3cd.obj"), Model("Models/COUNTDOWN/2cd.obj"), Model("Models/COUNTDOWN/1cd.obj")};
	float countdownTimer = 0;
	int countdownCounter = 0;
	bool countdown = true;

	bool trainInit = false;
};
