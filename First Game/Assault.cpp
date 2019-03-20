#include "Assault.h"
Model* Assault::missile;

typedef EmGineAudioPlayer AudioPlayer;

void Assault::init()
{
	type = assault;
	m_initialHealth = 100;
	setHealth(100);

	if(!missile)
		Assault::missile = new Model("Models/missile/candyMissile.obj");
}

Assault::Assault():Player()
{
	init();
}

Assault::Assault(Player& model) : Player(model)
{
	init();
}

Assault::Assault(const char* path) : Player(path)
{
	init();
}


Assault::~Assault()
{
}

float Assault::getTimeSinceLastMissile()
{
	return m_timeSinceLastMissile;
}

void Assault::setTimeSinceLastMissile(float v)
{
	m_timeSinceLastMissile = v;
}

bool Assault::missileCollision(Model* mod)
{
	///- Missile Collisions -///
	bool collision = false;
	for(unsigned a = 0; a < pMissiles.size(); a++)
		if(collision3D(pMissiles[a], mod))
		{
			collision = true;
			GAME::removeModel(pMissiles[a]);
			pMissiles.erase(pMissiles.begin() + a);
			missileVelocity.erase(missileVelocity.begin() + a);
			timer.erase(timer.begin() + a);
		}
	return collision;
}

void Assault::update(float dt)
{
	if(!m_active)
		return;

	Player::update(dt);
	XinputController* p1 = (XinputController*)GAME::getController(m_index);

	/// - Assault Special Ability - ///
	if(p1->isButtonPressed(CONTROLLER_Y))
		if(time - m_timeSinceLastMissile >= 3)
		{
			pMissiles.push_back(new Model(*missile));
			GAME::addModel(pMissiles.back());
			pMissiles.back()->getTransformer().reset();
			pMissiles.back()->setColour(getColour());
			Coord3D pos = getTransformer().getPosition();

			pMissiles.back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
			pMissiles.back()->getTransformer().setScale(0.4f);
			pMissiles.back()->getTransformer().setRotation({0 , angle ,0});

			float cosVal = cos((float)(fmodf(angle - 90, 360) * (M_PI / 180)));
			float sinVal = sin((float)(fmodf(angle - 90, 360) * (M_PI / 180)));

			missileVelocity.push_back(Coord3D(cosVal * move * 6, 0, sinVal * move * 6));
			setTimeSinceLastMissile(time);

			timer.push_back(time);
			AudioPlayer::createAudioStream("pew.wav");
			AudioPlayer::play();
			puts("Special Ability ASSAULT\n");
		}


	for(unsigned a = 0; a < pMissiles.size(); a++)
		if(pMissiles[a])
		{
			timer[a] += dt;
			pMissiles[a]->getTransformer().translateBy(missileVelocity[a].x, missileVelocity[a].y, missileVelocity[a].z);

			if(timer[a] >= 1)
			{
				GAME::removeModel(pMissiles[a]);
				pMissiles.erase(pMissiles.begin() + a);
				missileVelocity.erase(missileVelocity.begin() + a);
				timer.erase(timer.begin() + a);
			}
		}
}