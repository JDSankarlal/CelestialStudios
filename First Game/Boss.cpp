#define _USE_MATH_DEFINES
#include "Boss.h"

typedef GameEmGine GAME;

Model* Boss::missles[4];
Model *Boss::lazer;

Boss::Boss(): Model()
{
	missles[0] = (new Model("Models/Missile/BossMissile.obj"));
	GAME::addModel(missles[0]);
	missles[1] = (new Model(*missles[0]));
	GAME::addModel(missles[1]);
	missles[2] = (new Model(*missles[0]));
	GAME::addModel(missles[2]);
	missles[3] = (new Model(*missles[0]));
	GAME::addModel(missles[3]);

	lazer = new Model("Models/lazer/lazer.obj");
	lazer->setColour(1, 0, 0);
	GAME::addModel(lazer);
}

Boss::Boss(Model& model): Model(model)

{
	Boss();
}

Boss::Boss(const char* path):Model(path)
{
	Boss();
}

Boss::~Boss()
{
}

void Boss::setPlayers(Player* players[4])
{
	targets = players;
}

bool Boss::randAttacks()
{
	//Randomly Pick an attack (attacks should be other functions, this semester just model attacks)
	//Use Attack
	//If Player(s) standing too close use stomp OR flop attack instead
	//Wait x Amount of time before repeating
	return false;
}

int Boss::getHealth()
{
	return health;
}

void Boss::setHealth(int v)
{
	health = v;
}

Model* Boss::getMissial(int index)
{
	return missles[index];
}

void Boss::update()
{

	static clock_t  lastDelay[4];
	static float  curveroni[4], delay[4];
	static bool hasTarget[4];
	static Coord3D cat[4];
	static Coord3D  pointPosition[4];

	for(int a = 0; a < 4; a++)
	{
		pointPosition[a] = getTransformer().getPosition();
		if(!targets[a]->dead)
		{
			float ans;

			if((ans = (clock() - lastDelay[a]) / (float)CLOCKS_PER_SEC) >= delay[a])
				curveroni[a] += .01f;

			if(curveroni[a] >= 1)
			{
				curveroni[a] = 0;
				lastDelay[a] = clock(),
					hasTarget[a] = false;
			}

			//curveroni[a] = fmodf(curveroni[a], 1);

			static Coord3D bossTarget[4];

			//gets a target for model (players 1,2,3 or 4) randomly
			if(!hasTarget[a])
			{
				bossTarget[a] = targets[a]->getTransformer().getPosition();
				delay[a] = (rand() % 2 + 1) + (float(rand() % 100) / 100);
				hasTarget[a] = true;
			}


			if(hasTarget[a])
			{

				Coord3D
					p1[4],
					p2[4],
					c1[4],
					c2[4];

				p1[a] = getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 2.0f),//start point
					p2[a] = bossTarget[a],//end point 
					c1[a] = p1[a] - Coord3D{0, 100, 100},//control point
					c2[a] = p2[a] - Coord3D{0, 150, 100};//control point

				cat[a] = catmull
				(
					c1[a],
					p1[a],
					p2[a],
					c2[a],
					curveroni[a]
				);

				pointPosition[a] = cat[a];
			}
			missles[a]->getTransformer().setPosition(pointPosition[a].x, pointPosition[a].y, pointPosition[a].z);
		}
	}


	shootLazer(0);

}

void Boss::shootLazer(int playerIndex)
{

	 
	Coord3D start = getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 0.0f)
		, end = targets[playerIndex]->getTransformer().getPosition();

	float distance = (end - start).distance();
	static float counter,amount=.01f;

	counter += amount;

	if(counter > 1 || counter < 0)
	{
		counter =float( counter > 0 ? 1 : 0);

		amount *= -1;
	}
	float angle[3];
	angle[0] = acosf((start.x*end.x+start.y*end.y )/
		(sqrtf(start.x*start.x
			+ start.y * start.y)* sqrtf(end.x* end.x + end.y * end.y))) * (180 / (float)M_PI);
	angle[1] = acosf((start.x * end.x + start.z * end.z) /
		(sqrtf(start.x * start.x
			+ start.z * start.z) * sqrtf(end.x * end.x + end.z * end.z))) * (180 / (float)M_PI);

	angle[2] = acosf((start.y * end.y + start.z * end.z) /
		(sqrtf(start.y * start.y
			+ start.z * start.z) * sqrtf(end.y * end.y + end.z * end.z))) * (180 / (float)M_PI);

	lazer->getTransformer().setPosition(start);
	lazer->getTransformer().setScale( .5f,lerp(0.0f, distance, counter), .5f);
	lazer->getTransformer().setRotation({angle[0],angle[2],angle[1]});

}
