#include "Minion.h"
std::vector<Player*> Minion::targets;


Minion::Minion():Model()
{

}

Minion::Minion(Model& model) : Model(model)
{

}

Minion::Minion(const char* path) : Model(path)
{

}


Minion::~Minion()
{
}

void Minion::setHealth(float v)
{
	m_health = v;
}

float Minion::getTimeSinceLastSpawn()
{
	return m_timeSinceLastSpawn;
}

void Minion::setTimeSinceLastSpawn(float v)
{
	m_timeSinceLastSpawn = v;
}

void Minion::addTarget(Player* mod)
{
	targets.push_back(mod);
}

bool Minion::hitByEnemy(Model* enemy, float damage)
{
	if(enemy)
	{
		if(collision2D(enemy, {false,true,true}))
		{
			setHealth(getHealth() - damage);
			return true;
		}
	}
	return false;
}

void Minion::update(float dt)
{
	dt;
	/// - Minions Movement Towards Players - ///
	if(m_move)
	{
		float distance, lastDistance = 0;
		moveTo = targets[0]->getPosition();

		if(!targets.empty())
			lastDistance = (getPosition() - targets[0]->getPosition()).distance();

		for(auto& a : targets)
		{
			distance = (getPosition() - a->getPosition()).distance();
			if(distance < lastDistance)
				moveTo = a->getPosition() - getPosition(),
				lastDistance = (getPosition() - a->getPosition()).distance();
		}

		moveTo.normalize();
		translateBy(moveTo * m_moveSpeed);
	}
	
}

void Minion::move(bool move)
{
	m_move = move;
}

float Minion::getHealth()
{
	return m_health;
}

bool operator>(Minion a, Minion b)
{
	return 1;
}
