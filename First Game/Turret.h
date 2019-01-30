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
	float lifeTime;
	int health;
};

