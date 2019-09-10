#include "Medic.h"


void Medic::init()
{
	type = medic;
	m_initialHealth = 150;
	setHealth(150);
	//puts("MEDIC CIRCLE SHOULD APPEAR WTF");
	healingCircle = new Model("Models/MedicCircle/BETTERHealingCircle.obj");
	healingCircle->setToRender(true);
	healingCircle->setTransparent(true);
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
	if(collision2D(healingCircle,player, {0,0,1}))
	{
		static float healAmount = .95f;
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
	XinputController* p1 = (XinputController*)GameEmGine::getController(m_index);
	p1->setStickDeadZone(.2f);

	///- Medic Special Ability Inactive - ///
	if(p1->isButtonStroked(CONTROLLER_Y))
		if(isHealing == false)
		{
			if(time - getTimeSinceLastHeal() >= 8)
			{
				healingCircle->translate(this->getPosition() + (Coord3D<>({0.0f, 0.1f, 0.0f})));
				healingCircle->setScale(4);
				healingCircle->setToRender(true);
				circleTime = time;
				//puts("Special Ability MEDIC");
				AudioPlayer::createAudioStream("Audio/medicAbility.wav","Medic Ability");
				AudioPlayer::play();
				isHealing = true;
				GameEmGine::addModel(healingCircle);
				if (m_index == 0)
				{
					healingCircle->setColour({ 255,0,0 });
				}
				if (m_index == 1)
				{
					healingCircle->setColour({ 0,0,255 });
				}
				if (m_index == 2)
				{
					healingCircle->setColour({ 0,255,0 });
				}
				if (m_index == 3)
				{
					healingCircle->setColour({ 255,255,0 });
				}
			}
		}

	///- Medic Secial Ability Active - ///
	if (isHealing == true)
	{	//Makes medics Circle disappear
		//healingCircle->setToRender(true);
		if ((time - circleTime) >= 3)
		{
			healingCircle->setToRender(false);
			setTimeSinceLastHeal(time);
			isHealing = false;
			GameEmGine::removeModel(healingCircle);

		}
	}

}
