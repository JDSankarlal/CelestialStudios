#include "Boss.h"



Boss::Boss(): Model()
{
}

Boss::Boss(Model& model) : Model(model)

{

}

Boss::Boss(const char* path):Model(path)
{

}

Boss::~Boss()
{
}

void Boss::setPlayers(Player players[4])
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

void Boss::update()
{

	static float  curveroni[4] = {0,0,0,0};
	static bool hasTarget[4] = {0,0,0,0};
	static Coord3D cat[4];
	static Coord3D  pointPosition[4];

	for(int a = 0; a < 4; a++)
	{
		if(!targets->dead)
		{
			curveroni[a] += .01f;
			if(curveroni[a] >= 1)
			{
				hasTarget[a] = false;
			}
			curveroni[a] = fmodf(curveroni[a], 1);

			static Coord3D bossTarget[4];

			//gets a   target for model (players 1,2,3 or 4) randomly
			if(!hasTarget[a])
			{

				bossTarget[a] = getTransformer().getPosition();

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
				missles[a]->getTransformer().setPosition(pointPosition[a].x, pointPosition[a].y, pointPosition[a].z);
			}
		}
	}
}
