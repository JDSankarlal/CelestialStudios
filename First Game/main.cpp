#define _USE_MATH_DEFINES 
//#define 

#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <vector>

using namespace std;

float ang = 2;
int numModel = 0;
bool m_left = 0, m_right = 0, m_in = 0, m_out = 0, m_up = 0, m_down = 0,
rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0,
movePlayer = true;
Coord2D leftM, rightM;
EmGineAudioPlayer audio;

#define modSize 20 //Number of Models that can exist
GameEmGine game("The Real Game", 1000, 800, 0, 0, 0, false);
GLSLCompiler colourProgram, colourProgram2;
Model *mod[modSize];

//shader initialization
void shaderInit()
{
	//shader initialization
}

//instance key is pressed
void keyInputPressed(int key, int _mod)
{
	m_left = (key == 'A' ? true : m_left);
	m_right = (key == 'D' ? true : m_right);
	m_up = (key == 'E' ? true : m_up);
	m_down = (key == 'Q' ? true : m_down);
	m_in = (key == 'W' ? true : m_in);
	m_out = (key == 'S' ? true : m_out);

	rotLeft = (key == GLFW_KEY_LEFT ? true : rotLeft);
	rotRight = (key == GLFW_KEY_RIGHT ? true : rotRight);
	rotUp = (key == GLFW_KEY_UP ? true : rotUp);
	rotDown = (key == GLFW_KEY_DOWN ? true : rotDown);

	printf("key PRESED code: %d\n\n", key);
}

//instace key is released
void keyInputReleased(int key, int _mod)
{
	m_left = (key == 'A' ? false : m_left);
	m_right = (key == 'D' ? false : m_right);
	m_in = (key == 'W' ? false : m_in);
	m_out = (key == 'S' ? false : m_out);
	m_up = (key == 'E' ? false : m_up);
	m_down = (key == 'Q' ? false : m_down);

	rotLeft = (key == GLFW_KEY_LEFT ? false : rotLeft);
	rotRight = (key == GLFW_KEY_RIGHT ? false : rotRight);
	rotUp = (key == GLFW_KEY_UP ? false : rotUp);
	rotDown = (key == GLFW_KEY_DOWN ? false : rotDown);

	//changes fps limit
	if(key == GLFW_KEY_KP_6)
		game.setFPSLimit(game.getFPSLimit() + 1);
	if(key == GLFW_KEY_KP_4)
		game.setFPSLimit(game.getFPSLimit() - 1);

	if(key == GLFW_KEY_F) //Toggles Fullscreen
	{
		static bool full;
		game.getWindow()->setFullScreen(full = !full);
		printf("Full Screen: %s\n", full ? "true" : "false");
	}

	if(key == GLFW_KEY_SPACE) //changes the model that is being moved
	{
		static CAMERA_TYPE type = PERSPECTIVE;
		game.setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
	}

	if(key == GLFW_KEY_TAB)
		movePlayer = !movePlayer;

	if(key == GLFW_KEY_F5) //resets the camera
	{
		GameEmGine::m_modelShader->refresh();

		//game.setCameraAngle(0, { 1, 1, 1 });
		//	game.setCameraPosition({0,0,0});
	}

	if(key == 'R')
		game.setCameraAngle(0, { 1,1,1 });

	printf("key RELEASED code: %d\n\n", key);
}

/// - Collision Class - ///
bool collisions(Model *l, Model *k)
{
	//if distance between models in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
	Coord3D thing = l->getCenter() - k->getCenter();

	float distanceX = abs(thing.coordX);
	float distanceZ = abs(thing.coordZ);

	float capW = (l->getWidth() + k->getWidth()) / 2;
	float capD = (l->getDepth() + k->getDepth()) / 2;

	if(std::abs(distanceX) <= capW)
		if(std::abs(distanceZ) <= capD)
			return true;

	return false;
}

//updates within game loop
void update(double dt)
{
	float move = .1f;
	//printf("%f\n", dt);

	static Model* bullets[4];
	static Coord3D velocity[4];
	static bool makeShitLessCancer[4];
	if(movePlayer)
		for(int a = 0; a < 4; a++)
			if(game.isControllerConnected(a))
			{
				Xinput p1 = game.getController(a);


				static float angle[4] = { 0,0,0,0 };
				if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
				{

					angle[a] = acosf(p1.Coord2D_sticks[RS].x /
									 sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
									 + p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
					angle[a] += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
					angle[a] = fmodf(angle[a], 360);
				}


				if(p1.triggers[RT] >= .95 && !makeShitLessCancer[a])
				{
					makeShitLessCancer[a] = true;

					if(bullets[a])
					{
						game.removeModel(bullets[a]);
						//	delete bullets[0];
					}

					game.addModel(bullets[a] = new Model(*mod[a]));
					bullets[a]->getTransformer().reset();
					Coord3D pos = mod[a]->getTransformer().getPosition();
					bullets[a]->getTransformer().setPosition(pos.coordX, pos.coordY + .1, pos.coordZ);
					bullets[a]->getTransformer().setScale(0.25);

					bullets[a]->getTransformer().setRotation({ 90 , angle[a] ,0 });


					float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
					float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

					velocity[a] = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
					audio.createStream("pew.wav");
					audio.play();
				} else if(p1.triggers[RT] < .95 && makeShitLessCancer[a])
					makeShitLessCancer[a] = false;

				/// - Button Presses on controller - ///

				if(p1.buttonPressed(p1.buttons.START))
				{
					printf("\nExiting Game\n");
					exit(0);
				}
				if(p1.buttonPressed(p1.buttons.X))
				{
					printf("RELOADING!!!\n");
				}
				if(p1.buttonPressed(p1.buttons.Y))
				{
					printf("SPECIAL ABILITY\n");
				}
				if(p1.triggers[LT] >= .95)
				{
					//get deltaTime put into duraction variable
					//if(deltaTime - coolDown >= 2)
					//{
						//float duration = deltaTime;
					move = 0.5f;
					//if (deltaTime >= duration + an amount)
					//{
					//		move = 0.1f;
					//		float coolDown = deltaTime;
					//}
				//}


				//Do the same with the LT button, have it so will only work every X seconds.
				}

				/// - Bullet Collisions - ///

				if(bullets[a])
				{
					bullets[a]->getTransformer().translateBy(velocity[a].coordX, velocity[a].coordY, velocity[a].coordZ);

					if(collisions(bullets[a], mod[8]))
					{
						game.removeModel(bullets[a]);
						bullets[a] = 0;
						printf("Hit BOSS\n\n");
					}

					if(bullets[a])
						for(int i = 5; i < 8; i++)
						{
							bullets[a]->getTransformer().translateBy(velocity[a].coordX, velocity[a].coordY, velocity[a].coordZ);

							if(collisions(bullets[a], mod[i]))
							{
								game.removeModel(bullets[a]);
								bullets[a] = 0;
								printf("Hit Wall??\n\n");
								break;
							}
						}
				}
				mod[a]->getTransformer().setRotation({ 0,angle[a]	,0 });
				mod[a]->getTransformer().translateBy(p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move); //move player
				//game.moveCameraPositionBy({ p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move });
				//	mod[0]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
				//	mod[0]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
			}

	if(!movePlayer)
		if(game.isControllerConnected(0))
		{
			Xinput p1 = game.getController(0);

			p1.numButtons;
			p1.numSticks;
			float angle;
			if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
			{

				angle = acosf(p1.Coord2D_sticks[RS].x /
							  sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
							  + p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
				angle += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
				angle = fmodf(angle, 360);
			}

			if(Xinput::buttonPressed(p1.buttons.A))
				printf("%d\n", p1.buttons.A);

			game.moveCameraPositionBy({ p1.Coord2D_sticks[LS].x * move , 0 * move, p1.Coord2D_sticks[LS].y * move });//move camera
			game.moveCameraAngleBy(ang * (abs(p1.Coord2D_sticks[RS].x) + abs(p1.Coord2D_sticks[RS].y)), { p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0 });//rotate camera
			game.moveCameraPositionBy({ 0 ,p1.triggers[LT] * -move,0 });//move out
			game.moveCameraPositionBy({ 0 ,p1.triggers[RT] * move,0 });//move out
		}
}

void mouseButtonReleased(int button, int _mod)
{
	if(button == LEFT_BUTTON)
		leftM = InputManager::getMouseCursorPosition();
	if(button == RIGHT_BUTTON)
		rightM = InputManager::getMouseCursorPosition();
}

void render()
{}

//template<class T>
//bool sorted(T* sort, unsigned size)
//{
//	for(unsigned a = 1; a < size; a++)
//		if(sort[a] < sort[a - 1])
//			return false;
//	return true;
//}
//
//template<class T>
//void randomize(T* sort, unsigned size)
//{
//	for(int a = 0; a < size; a++)
//		swap(sort[a], sort[(rand() % size]);
//}
//
//template<class T>
//void bogo(T* sort, unsigned size)
//{
//	if(sorted(sort, size))
//		return;
//
//	randomize(sort, size);
//	bogo(sort, size);
//}
//
//template<class T>
//void bogobogo(T* sort, unsigned size)
//{
//	T *sort2 = new T[size];
//	memcpy_s(sort2, sizeof(T)*size, sort, sizeof(T)*size);
//	unsigned n = size - 1;
//
//	while(n)
//	{
//		bogo(sort2, n);
//		if()
//	}
//}

int main()
{
	/// - Load Models into Scene - ///

	//game.addModel(mod[0] = new Model("Models/crysis-nano-suit-2(OBJ)/scene.obj")); //Crysis Guy
	game.addModel(mod[0] = new Model("Models/AssaultModel/Model_AssaultClass.obj"));//Rowans Character
	game.addModel(mod[5] = new Model("Models/PlaceholderWalls/PlaceholderBox.obj")); //Wall
	game.addModel(mod[8] = new Model("Models/BOSS/roughBOSS.obj")); //Boss
	game.addModel(mod[9] = new Model("Models/Floor/Floor.obj")); //Floor
	game.addModel(mod[10] = new Model("Models/Lamp/lampPost.obj"));//Street Light

	mod[5]->setColour(0.65f, 0.65f, 0.7f);

	/// - Make New Models From Existing Models - ///
	//Players
	mod[3] = new Model(*mod[0]);
	mod[2] = new Model(*mod[0]);
	mod[1] = new Model(*mod[0]);

	//Placeholder Walls
	mod[6] = new Model(*mod[5]);
	mod[7] = new Model(*mod[5]);

	//Street Lights
	mod[11] = new Model(*mod[10]);
	mod[12] = new Model(*mod[10]);
	mod[13] = new Model(*mod[10]);
	mod[14] = new Model(*mod[10]);
	mod[15] = new Model(*mod[10]);

	/// - Set Model Transforms - ///
	//Player Transforms
	mod[0]->getTransformer().setScale(1), mod[0]->getTransformer().setPosition(1, 1, 0),
		mod[1]->getTransformer().setScale(1), mod[1]->getTransformer().setPosition(-1, 1, 0),
		mod[2]->getTransformer().setScale(1), mod[2]->getTransformer().setPosition(2, 1, 0),
		mod[3]->getTransformer().setScale(1), mod[3]->getTransformer().setPosition(-2, 1, 0);

	//Wall Transforms
	mod[5]->getTransformer().setRotation({ 0.0f, 90.0f, 0.0f }), mod[5]->getTransformer().setPosition(15.0f, 1.8f, 7.0f), mod[5]->getTransformer().setScale(3.0f, 1.0f, 1.0f),
		mod[6]->getTransformer().setRotation({ 0.0f, 90.0f, 0.0f }), mod[6]->getTransformer().setPosition(-15.0f, 1.8f, 7.0f), mod[6]->getTransformer().setScale(3.0f, 1.0f, 1.0f),
		mod[7]->getTransformer().setRotation({ 0.0f, 0.0f, 0.0f }), mod[7]->getTransformer().setPosition(0.0f, 1.8f, 20.5f), mod[7]->getTransformer().setScale(3.0f, 1.0f, 1.0f);

	//Boss Transforms
	mod[8]->getTransformer().setRotation({ 0, 90, 0 }), mod[8]->getTransformer().setPosition(0, 0, 17), mod[8]->getTransformer().setScale(3);

	//Floor Transforms
	mod[9]->getTransformer().setScale(1.3f, 1.0f, 1.3f);

	//Street Light Transforms
	mod[10]->getTransformer().setScale(0.5), mod[10]->getTransformer().setPosition(13, 1, -1),
		mod[11]->getTransformer().setScale(0.5), mod[11]->getTransformer().setPosition(13, 1, 7),
		mod[12]->getTransformer().setScale(0.5), mod[12]->getTransformer().setPosition(13, 1, 15),
		mod[13]->getTransformer().setScale(0.5), mod[13]->getTransformer().setPosition(-13, 1, -1), mod[13]->getTransformer().setRotation({ 0.0f,180.0f,0.0f }),
		mod[14]->getTransformer().setScale(0.5), mod[14]->getTransformer().setPosition(-13, 1, 7), mod[14]->getTransformer().setRotation({ 0.0f,180.0f,0.0f }),
		mod[15]->getTransformer().setScale(0.5), mod[15]->getTransformer().setPosition(-13, 1, 15), mod[15]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });

	/// - Set Model Colour - ///
	//Players
	mod[0]->setColour(1, 0, 0);
	mod[1]->setColour(0, 0, 1);
	mod[2]->setColour(0, 1, 0);
	mod[3]->setColour(1, 1, 0);

	//Floor
	mod[9]->setColour(196.0f / 255, 167.0f / 255, 113.0f / 255);

	/// - Add Duplicate Models - ///

	game.addModel(mod[1]);
	game.addModel(mod[2]);
	game.addModel(mod[3]);
	game.addModel(mod[6]);
	game.addModel(mod[7]);
	game.addModel(mod[11]);
	game.addModel(mod[12]);
	game.addModel(mod[13]);
	game.addModel(mod[14]);
	game.addModel(mod[15]);

	/// - Set Camera - ///

	game.setCameraPosition({ 0,15,-10 });
	game.setCameraAngle(-45, { 1,0,0 });


	audio.createStream("Game Jam(Full).wav");

	audio.play(true);

	//engine stuff
	//game.setFPSLimit(60);
	game.keyPressed(keyInputPressed);
	game.keyReleased(keyInputReleased);
	game.mouseButtonReleased(mouseButtonReleased);
	game.gameLoopUpdate(update);
	game.run();//this one is pretty important

	//the game ended... why are you here?... leave
	//Or run it again... ;)
	return 0;
}