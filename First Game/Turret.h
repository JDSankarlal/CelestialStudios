#pragma once
class Turret
{
public:
	Turret();
	~Turret();

	void setLifeTime(float t);
	float getLifeTime();

	void setHealth(int v);
	float getHealth();

private:
	float lifeTime;
	int health;
};

