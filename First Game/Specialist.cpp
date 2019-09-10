#include "Specialist.h"
Turret* Specialist::turret;

void Specialist::init()
{
	type = specialist;
	m_initialHealth = 75;
	setHealth(75);
	if(!turret)
		turret = new Turret("Models/Turret/turret.obj");
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
		if(collision2D(mod, pTurret, {0,0,1}))
		{
			hit = true;
			GameEmGine::removeModel(pTurret);
			pTurrets.erase(std::find(pTurrets.begin(), pTurrets.end(), pTurret));
			continue;
		}
	}
	return hit;
}


void Specialist::update(float dt)
{
	dt;
	if(!m_active)
	{
		getCurrentAnimation()->pause();
		return;
	}
	else
		if (!getCurrentAnimation()->checkPlay())
			getCurrentAnimation()->play();

	Player::update(dt);

	XinputController* p1 = (XinputController*)GameEmGine::getController(m_index);
	
	/// - Turret Active - ///
	if(turretActive == true)
	{

		for(auto& pTurret : pTurrets)
		{
			pTurret->update(dt);
			///// - Turret targeting and shooting logic - ///
			////Get pTurret position
			////Coord3D ;
			//turretPos = pTurret->getPosition().x + pTurret->getPosition().y;
			//float lowestDistance = (turretPos - minions[0]->getPosition()).distance();
			//
			//for (unsigned int m = 0; m < minions.size(); m++)
			//{
			//	//get minion positions
			//	
			//	if((turretPos - minions[m]->getPosition()).distance() < lowestDistance)
			//	{
			//		lowestDistance = (turretPos - minions[m]->getPosition()).distance();
			//		minionPos =  minions[m]->getPosition();
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
			//Coord3D pos = pTurret->getPosition();
			//bullets.back()->setPosition(pos.x, pos.y + .1f, pos.z);
			//bullets.back()->setScale(.25f);
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
				//turretPos = pTurret->getPosition().x + pTurret->getPosition().y;
				//for (int m = 0; m < minions.size(); m++)
				//{
				//	//get minion positions
				//	minionPos = minions[m]->getPosition().x + minions[m]->getPosition().y;
				//	
				//}


				/// - Cases for deleting pTurret - ///
				//If pTurret time runs out
			if((time - turretTime) >= 5)
			{
				GameEmGine::removeModel(pTurret);
				pTurrets.erase(std::find(pTurrets.begin(), pTurrets.end(), pTurret));
				continue;
			}
		}
	}

	if(p1->isButtonStroked(CONTROLLER_Y))
	{
		if(time - getTimeSinceLastTurret() >= 8)
		{
			pTurrets.push_back(new Turret(*turret));
			GameEmGine::addModel(pTurrets.back());
			pTurrets.back()->reset();
			//pTurrets.back()->setColour(getColour());
			Coord3D pos = getPosition();
			pTurrets.back()->translate(pos.x, pos.y + .1f, pos.z);
			pTurrets.back()->rotate({0.0f, 90.0f, 0.0f});
			//pTurrets.back()->setScale(0.4f);
			pTurrets.back()->setToRender(true);
			//puts("Special Ability SPECIALIST");
			AudioPlayer::createAudioStream("Audio/specialistAbility.wav","Specialist Ability");
			EmGineAudioPlayer::setVolume(2);
			AudioPlayer::play();
			setTimeSinceLastTurret(time);
			turretTime = time;
			turretActive = true;
		}
	}
}