#pragma once
class Turret
{
public:
	Turret();
	~Turret();

	void setLifeTime(float t);
	float getLifeTime();

	void setHealth(int v);
	int getHealth();

private:
	float m_lifeTime;
	int m_health;
};

