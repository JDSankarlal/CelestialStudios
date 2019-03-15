#include "Specialist.h"
Model* Specialist::turret;

void Specialist::init()
{
	type = specialist;
	m_initialHealth = 75;
	setHealth(75);
	if(!turret)
		turret = new Model("Models/Turret/turret.obj");
}

Specialist::Specialist():Player()
{
	init();
}

Specialist::Specialist(Player& model) : Player(model)
{
	init();
}

Specialist::Specialist(const char* path) : Player(path)
{
	init();
}


Specialist::~Specialist()
{
}

float Specialist::getTimeSinceLastTurret()
{
	return timeSinceLastTurret;
}

void Specialist::setTimeSinceLastTurret(float v)
{
	timeSinceLastTurret = v;
}

bool Specialist::hitTurret(Model* mod)
{
	bool hit = false;
	for(auto& pTurret : pTurrets)
	{
		//If pTurret touched by minion
		if(collision2D(mod, pTurret))
		{
			hit = true;
			GAME::removeModel(pTurret);
			pTurrets.erase(std::find(pTurrets.begin(), pTurrets.end(), pTurret));
			continue;
		}
	}
	return hit;
}


void Specialist::update(float dt)
{
	dt;
	XinputController* p1 = (XinputController*)GAME::getController(m_index);
	/// - Turret Active - ///
	if(turretActive == true)
	{

		for(auto& pTurret : pTurrets)
		{

			///// - Turret targeting and shooting logic - ///
			////Get pTurret position
			////Coord3D ;
			//turretPos = pTurret->getTransformer().getPosition().x + pTurret->getTransformer().getPosition().y;
			//float lowestDistance = (turretPos - minions[0]->getTransformer().getPosition()).distance();
			//
			//for (unsigned int m = 0; m < minions.size(); m++)
			//{
			//	//get minion positions
			//	
			//	if((turretPos - minions[m]->getTransformer().getPosition()).distance() < lowestDistance)
			//	{
			//		lowestDistance = (turretPos - minions[m]->getTransformer().getPosition()).distance();
			//		minionPos =  minions[m]->getTransformer().getPosition();
			//
			//	}
			//
			//}
			//pow1 = pow((double)X, 2); //type casting from int to double
			//pow2 = pow((double)Y, 2);
			//
			//sq = abs(sqrt((double)pow1 + (double)pow2));
			//
			//angleInRadian = atan2((double)Y, (double)X); //angle in radian
			//
			//angleInDegree = angleInRadian * 180 / 3.14159; //angle in degree
			//
			//bullets.push_back(new Model(*mod[48]));
			//GAME::addModel(bullets.back());
			//
			//Coord3D pos = pTurret->getTransformer().getPosition();
			//bullets.back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
			//bullets.back()->getTransformer().setScale(.25f);
			//
			////Insert code for finding angle between pTurret and minion
			//
			//
			//float cosVal = cos((float)(fmodf((float)angleInDegree - 90, 360) * (M_PI / 180)));
			//float sinVal = sin((float)(fmodf((float)angleInDegree - 90, 360) * (M_PI / 180)));
			//
			//velocity[a].push_back(Coord3D());
			//velocity[a].back() = Coord3D(cosVal * move * 3, 0, sinVal * move * 3);
			//
			////timer[a].push_back(0);
			//audio.createAudioStream("pew.wav");
			//audio.play();
			//
			//

			/// - Turret targeting and shooting logic - ///
				//Get pTurret position
				//turretPos = pTurret->getTransformer().getPosition().x + pTurret->getTransformer().getPosition().y;
				//for (int m = 0; m < minions.size(); m++)
				//{
				//	//get minion positions
				//	minionPos = minions[m]->getTransformer().getPosition().x + minions[m]->getTransformer().getPosition().y;
				//	
				//}


				/// - Cases for deleting pTurret - ///
				//If pTurret time runs out
			if((time - turretTime) >= 5)
			{
				GAME::removeModel(pTurret);
				pTurrets.erase(std::find(pTurrets.begin(), pTurrets.end(), pTurret));
				continue;
			}
		}
	}

	if(p1->isButtonPressed(CONTROLLER_Y))
	{
		if(time - getTimeSinceLastTurret() >= 8)
		{
			pTurrets.push_back(new Model(*turret));
			GAME::addModel(pTurrets.back());
			pTurrets.back()->getTransformer().reset();
			//pTurrets.back()->setColour(getColour());
			Coord3D pos = getTransformer().getPosition();
			pTurrets.back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
			pTurrets.back()->getTransformer().setRotation({0.0f, 90.0f, 0.0f});
			//pTurrets.back()->getTransformer().setScale(0.4f);
			pTurrets.back()->setToRender(true);
			puts("Special Ability SPECIALIST");

			setTimeSinceLastTurret(time);
			turretTime = time;
			turretActive = true;
		}
	}
	Player::update(dt);
}