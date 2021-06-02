#include "Turret.h"

Turret::Turret()
{}

Turret::Turret(const char* str):Model(str)
{}

Turret::~Turret()
{}

void Turret::setLifeTime(float t)
{
	m_lifeTime = t;
}

float Turret::getLifeTime()
{
	return m_lifeTime;
}

void Turret::setHealth(int v)
{
	m_health = v;
}

int Turret::getHealth()
{
	return m_health;
}
void Turret::update(float dt)
{
	dt;
	static float Time = 0;
	static float bulletTime = 0;
	Time += dt;
	static Model* bullet = new Model("Models/Bullet/bullet.obj");
	if(Time - bulletTime >= 1)
	{
		bullets.push_back(new Model(*bullet));
		GameEmGine::addModel(bullets.back());
		bullets.back()->setColour(1, 1, 1);//bullet color = players color
		Coord3D pos = this->getLocalPosition();
		bullets.back()->translate(pos.x, pos.y + .1f, pos.z);
		bullets.back()->scale(.25f);

		velocity.push_back(Coord3D());
		velocity.back() = ((*std::next(m_minions.begin(), 0))->getLocalPosition() - getLocalPosition()).normal();
		velocity.back().y = 0;
		bulletTime = Time;
		//timer.push_back(0);
		std::string tag = "Turret Shots Fired" + std::to_string(bullets.size());
		AudioPlayer::createAudioStream("Audio/pew.wav", tag);
		AudioPlayer::play();
	}

	for(int a = 0; a < (int)bullets.size(); a++)
	{
		((*std::next(bullets.begin(), a)))->translateBy(velocity[a]);
		//if(collision bullets[a], minion)
	}


}

void Turret::setMinions(std::list<Minion*> minions)
{
	this->m_minions = minions;
}
