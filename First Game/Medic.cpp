#include "Medic.h"

void Medic::init()
{
	type = medic;
	m_initialHealth = 150;
	setHealth(150);
	healingCircle = new Model("Models/MedicCircle/BETTERHealingCircle.obj");
	GameEmGine::addModel(healingCircle); 
	healingCircle->setToRender(false);
}

Medic::Medic():Player()
{
	init();
}

Medic::Medic(Player& model) : Player(model)
{
	init();
}

Medic::Medic(const char* path) : Player(path)
{
	init();
}


Medic::~Medic()
{
}

float Medic::getTimeSinceLastHeal()
{
	return timeSinceLastHeal;
}

void Medic::setTimeSinceLastHeal(float v)
{
	timeSinceLastHeal = v;
}

void Medic::getHealing(Player* player)
{
	//if(time - getTimeSinceLastMissile() >= 3)
		//Healing
	if(collision2D(healingCircle,player))
	{
		static float healAmount = .02f;
		if(player->getHealth() + healAmount < player->getInitialHealth())
			player->setHealth(player->getHealth() + healAmount);
		
	}
}

void Medic::update(float dt)
{
	if(!m_active)
	{
		getCurrentAnimation()->pause();
		return;
	}
	else
		if (!getCurrentAnimation()->checkPlay())
			getCurrentAnimation()->play();

	Player::update(dt);
	XinputController* p1 = (XinputController*)GAME::getController(m_index);
	p1->setStickDeadZone(.2f);

	///- Medic Special Ability Inactive - ///
	if(p1->isButtonStroked(CONTROLLER_Y))
		if(isHealing == false)
		{
			if(time - getTimeSinceLastHeal() >= 5)
			{
				healingCircle->getTransformer().setPosition(getTransformer().getPosition() + (Coord3D({0.0f, 0.1f, 0.0f})));
				healingCircle->getTransformer().setScale(600);
				healingCircle->setToRender(true);
				circleTime = time;
				puts("Special Ability MEDIC");
				isHealing = true;
			}
		}

	///- Medic Secial Ability Active - ///
	if(isHealing == true)
		//Makes medics Circle disappear
		if((time - circleTime) >= 2.5f)
		{
			healingCircle->setToRender(false);
			setTimeSinceLastHeal(time);
			isHealing = false;
		}


}
