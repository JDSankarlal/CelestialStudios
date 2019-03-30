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
		if(collision2D(enemy))
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
		Coord3D moveTo = targets[0]->getTransformer().getPosition();

		if(!targets.empty())
			lastDistance = (getTransformer().getPosition() - targets[0]->getTransformer().getPosition()).distance();

		for(auto& a : targets)
		{
			distance = (getTransformer().getPosition() - a->getTransformer().getPosition()).distance();
			if(distance < lastDistance)
				moveTo = a->getTransformer().getPosition() - getTransformer().getPosition(),
				lastDistance = (getTransformer().getPosition() - a->getTransformer().getPosition()).distance();
		}

		moveTo.normalize();
		getTransformer().translateBy(moveTo * m_moveSpeed);
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
