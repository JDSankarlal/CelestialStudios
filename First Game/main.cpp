#define _USE_MATH_DEFINES 
//#define 

#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <vector>
#include "Boss.h"
#include "Player.h"

using namespace std;

float ang = 2;
int numModel = 0;
bool m_left = 0, m_right = 0, m_in = 0, m_out = 0, m_up = 0, m_down = 0,
rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0,
movePlayer = true;
Coord2D leftM, rightM;
EmGineAudioPlayer audio;

#define modSize 20 //Number of mod that can exist
GameEmGine game("The Real Game", 1920, 1080, 0, 0, 0, false);
Shader colourProgram, colourProgram2;
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

	printf("key PRESSED code: %d\n\n", key);
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
	if (key == GLFW_KEY_KP_6)
		game.setFPSLimit(game.getFPSLimit() + 1);
	if (key == GLFW_KEY_KP_4)
		game.setFPSLimit(game.getFPSLimit() - 1);

	if (key == GLFW_KEY_F) //Toggles Fullscreen
	{
		static bool full;
		game.getWindow()->setFullScreen(full = !full);
		printf("Full Screen: %s\n", full ? "true" : "false");
	}

	if (key == GLFW_KEY_SPACE) //changes the model that is being moved
	{
		static CAMERA_TYPE type = PERSPECTIVE;
		game.setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
	}

	if (key == GLFW_KEY_TAB)
		movePlayer = !movePlayer;

	if (key == GLFW_KEY_F5) //resets the camera
	{
		GameEmGine::m_modelShader->refresh();

		//game.setCameraAngle(0, { 1, 1, 1 });
		//	game.setCameraPosition({0,0,0});
	}

	if (key == 'R')
		game.setCameraAngle(0, { 1,1,1 });

	printf("key RELEASED code: %d\n\n", key);
}
Path path;
int lineCurveDetail = 10;
void drawLineCatmull()
{
	if (path.pointHandles.size() < 4)
		return;

	for (int p0 = 0; p0 < path.pointHandles.size() - !path.connectedEndpoints * 3; ++p0)
	{
		int p1, p2, p3;
		p1 = vectorWrap(p0 + 1, path.pointHandles.size());
		p2 = vectorWrap(p0 + 2, path.pointHandles.size());
		p3 = vectorWrap(p0 + 3, path.pointHandles.size());

		glm::vec3 lastPosition = path.pointHandles[p1].position;
		for (int j = 0; j < lineCurveDetail; ++j)
		{

			glm::vec3 pointPosition = catmull
			(
				path.pointHandles[p0].position,
				path.pointHandles[p1].position,
				path.pointHandles[p2].position,
				path.pointHandles[p3].position, (j + 1.0f) / lineCurveDetail);

			//glLineWidth(10);
			//glColor3f(1.0, 0.0, 0.0);
			//glBegin(GL_LINES);
			//glVertex3f(0.0f, 4.0f, 10.0f);
			//glVertex3f(0, 0, 0);
			//	
			////Path path;
			//TTK::Graphics::DrawLine(lastPosition, pointPosition);
			//lastPosition = pointPosition;
			//glEnd();
		}
	}
}

/// - Collision Class - ///
bool collisions(Model *l, Model *k)
{
	//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
	Coord3D thing = l->getCenter() - k->getCenter();

	float distanceX = abs(thing.coordX);
	float distanceZ = abs(thing.coordZ);

	float capW = (l->getWidth() + k->getWidth()) / 2;
	float capD = (l->getDepth() + k->getDepth()) / 2;

	if (std::abs(distanceX) <= capW)
		if (std::abs(distanceZ) <= capD)
			return true;

	return false;
}

//updates within game loop
void update(double dt)
{
	static float  time = 0;
	time += dt;
	static float coolDown = 0;
	static float duration = 0;
	static bool f = true;
	static bool bossActive = true;

	float move = .1f;

	static float pointSize = 50.0f;
	//printf("%f\n", dt);


	static vector<Player*> bullets[4];
	static vector<Coord3D> velocity[4];
	static bool makeShitLessCancer[4], makeShitLessCancer2[4];//stops the creation of bullets when trigger is healed down
	static float  curveroni = 0;
	curveroni += .01;
	curveroni = fmodf(curveroni, 1);

	static bool hasTarget = false;
	static Coord3D bossTarget;

	//gets a   target for missile (player 1,2,3 or 4) randomly
	if (!hasTarget)
	{
		bossTarget = mod[rand() % 4]->getTransformer().getPosition();
		hasTarget = true;
	}

	if (hasTarget)
	{
		Coord3D
			p1 = mod[8]->getTransformer().getPosition(),//start point
			p2 = bossTarget,//end point 
			c1 = p1 - Coord3D{ 0,100,0 },//controle point
			c2 = p2 - Coord3D{ 0,100,0 };//controle point

		Coord3D cat = catmull
		(
			c1,
			p1,
			p2,
			c2,
			curveroni
		);

		Coord3D  pointPosition = cat;
		mod[18]->getTransformer().setPosition(pointPosition.coordX, pointPosition.coordY, pointPosition.coordZ);
	}

	//some function
	//{

	//when function plays, get player locations
	//call 4 missles to shoot at those locations
	//maybe call missles to shoot randomly
	//}


	if (movePlayer)
		for (int a = 0; a < 4; a++)
			if (game.isControllerConnected(a))
			{
				Xinput p1 = game.getController(a);




				static float angle[4] = { 180,180,180,180 };
				if (p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
				{

					angle[a] = acosf(p1.Coord2D_sticks[RS].x /
						sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
							+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
					angle[a] += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
					angle[a] = fmodf(angle[a], 360);
				}


				if (p1.triggers[RT] >= .95 && !makeShitLessCancer[a])
				{
					makeShitLessCancer[a] = true;

					//for(unsigned b=0;b<bullets[a].size();b++)
					//if(bullets[a][b])
					//{
					//	game.removeModel(bullets[a][b]);
					//	bullets[a].erase(bullets[a].begin() + b);
					//}

					bullets[a].push_back(nullptr);
					game.addModel(bullets[a].back() = new Player(*mod[a]));
					bullets[a].back()->getTransformer().reset();
					Coord3D pos = mod[a]->getTransformer().getPosition();
					bullets[a].back()->getTransformer().setPosition(pos.coordX, pos.coordY + .1, pos.coordZ);
					bullets[a].back()->getTransformer().setScale(0.25);

					bullets[a].back()->getTransformer().setRotation({ 90 , angle[a] ,0 });


					float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
					float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

					velocity[a].push_back(Coord3D());
					velocity[a].back() = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
					audio.createStream("pew.wav");
					audio.play();
				}
				else if (p1.triggers[RT] < .95 && makeShitLessCancer[a])
					makeShitLessCancer[a] = false;

				/// - Button Presses on controller - ///

				if (p1.buttonPressed(p1.buttons.START))
				{
					puts("\nExiting Game\n");
					game.exit();
				}
				if (p1.buttonPressed(p1.buttons.X))
				{
					puts("RELOADING!!!\n");
				}
				if (p1.buttonPressed(p1.buttons.Y))
				{
					puts("SPECIAL ABILITY\n");
				}
				/// - Left Trigger to Dash - ///

				if (p1.triggers[LT] >= .95)
				{
					static float coolDown[4];

					//get deltaTime put into duraction variable

					if (time - coolDown[a] >= 3)
					{
						if (f == true)
						{
							duration = time;
							f = false;
						}
						move = 0.5f;
						if (time - 0.1f >= duration)
						{
							move = 0.1f;
							//If triggers up then coolDown = time;
							coolDown[a] = time;
							f = true;
						}
					}

				}

				//Do the same with the LT button, have it so will only work every X seconds.
				else
				{
					move -= .001;
					if (move < .1)
						move = .1;
					makeShitLessCancer2[a] = false;
				}

				/// - Bullet Collisions - ///
				for (unsigned b = 0; b < bullets[a].size(); b++)
					if (bullets[a][b])
					{
						bullets[a][b]->getTransformer().translateBy(velocity[a][b].coordX, velocity[a][b].coordY, velocity[a][b].coordZ);

						if (mod[8])
							if (collisions(bullets[a][b], mod[8]))
							{
								game.removeModel(bullets[a][b]);
								bullets[a].erase(bullets[a].begin() + b);
								velocity[a].erase(velocity[a].begin() + b);
								Boss*CandyMan = (Boss*)mod[8];//Boss a.k.a model 8, is now called CandyMan for teh purposes of functions.
								CandyMan->setHealth(CandyMan->getHealth() - 100);// When hit takes damage
								if (CandyMan->getHealth() <= 0)
								{
									game.removeModel(CandyMan); // If health = 0 then boss dead
									mod[8] = nullptr;
									bossActive = false;
									puts("Killed The BOSS\n");
								}
								puts("Hit The BOSS\n");
								break;
							}

						if (bullets[a][b])
							for (int i = 5; i < 8; i++)
							{
								bullets[a][b]->getTransformer().translateBy(velocity[a][b].coordX, velocity[a][b].coordY, velocity[a][b].coordZ);

								if (collisions(bullets[a][b], mod[i]))
								{
									game.removeModel(bullets[a][b]);
									bullets[a].erase(bullets[a].begin() + b);
									velocity[a].erase(velocity[a].begin() + b);
									printf("Hit Wall\n\n");
									break;
								}
							}
					}

				//bool  hitWall = false;
				//for(int i = 5; i < 8; i++)
				//{
				//
				//	if(collisions(mod[a], mod[i]))
				//	{
				//		hitWall = true;
				//		printf("player has hit a wall");
				//		break;
				//	}
				//}
				mod[a]->getTransformer().setRotation({ 0,angle[a], 0 });
				//if(hitWall)
				//	continue;
				mod[a]->getTransformer().translateBy(p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move); //move player
				//game.moveCameraPositionBy({ p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move });
				//	mod[0]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
				//	mod[0]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
			}

	if (!movePlayer)
		if (game.isControllerConnected(0))
		{
			Xinput p1 = game.getController(0);

			//if(Player.getTransformer())

			p1.numButtons;
			p1.numSticks;
			float angle = 0;
			if (p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
			{

				angle = acosf(p1.Coord2D_sticks[RS].x /
					sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
						+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
				angle += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
				angle = fmodf(angle, 360);
			}

			if (Xinput::buttonPressed(p1.buttons.A))
				printf("%d\n", p1.buttons.A);

			////rotate left wall
			//mod[6]->getTransformer().setRotation({ 0, angle, 0 });

			//move canera
			move *= 2;
			game.moveCameraPositionBy({ p1.Coord2D_sticks[LS].x * move , 0 * move, p1.Coord2D_sticks[LS].y * move });//move camera
			game.moveCameraAngleBy(ang * (abs(p1.Coord2D_sticks[RS].x) + abs(p1.Coord2D_sticks[RS].y)), { p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0 });//rotate camera
			game.moveCameraPositionBy({ 0 ,p1.triggers[LT] * -move,0 });//move out
			game.moveCameraPositionBy({ 0 ,p1.triggers[RT] * move,0 });//move out
			move /= 2;
		}
	if (bossActive == true)
	{
		//glm::vec3 pi(0.0f,4.0f,10.0f);
		//glm::vec3 pp1(0, 0, 0);
		//glm::vec3 ppu1(0, -3, -1);
		//glm::vec3 ppu2(0, -3, 11);
		//
		//path.pointHandles.push_back(PointHandle(pointSize, pi, std::to_string(path.pointHandles.size())));
		//path.pointHandles.push_back(PointHandle(pointSize, pp1, std::to_string(path.pointHandles.size())));
		//path.pointHandles.push_back(PointHandle(pointSize, ppu1, std::to_string(path.pointHandles.size())));
		//path.pointHandles.push_back(PointHandle(pointSize, ppu2, std::to_string(path.pointHandles.size())));

		//glLineWidth(50);
		//glColor3f(1.0, 0.0, 0.0);
		//glBegin(GL_LINES);
		//glVertex3f(0.0f, 0.0f, 0.0f);
		//glVertex3f(0, 0, 5);
		//glEnd();
		//Path path;
	}
}

void mouseButtonReleased(int button, int _mod)
{
	if (button == LEFT_BUTTON)
		leftM = InputManager::getMouseCursorPosition();
	if (button == RIGHT_BUTTON)
		rightM = InputManager::getMouseCursorPosition();
}

void render()
{}

//// We need BogoBogo sort in our game right?
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
	/// - Load mod into Scene - ///

	//game.addModel(mod[0] = new Model("mod/crysis-nano-suit-2(OBJ)/scene.obj")); //Crysis Guy
	game.addModel(mod[0] = new Player("Models/AssaultModel/Model_AssaultClass.obj"));//Rowans Character
	game.addModel(mod[9] = new Model("Models/Floor/Floor.obj")); //Floor
	game.addModel(mod[5] = new Model("Models/PlaceholderWalls/PlaceholderBox.obj")); //Wall
	game.addModel(mod[8] = new Boss("Models/BOSS/roughBOSS.obj")); //Boss
	//Boss *CandyMan = mod[8];
	game.addModel(mod[10] = new Model("Models/Lamp/lampPost.obj"));//Street Light
	game.addModel(mod[16] = new Model("Models/Bench/Bench.obj"));//Bench
	game.addModel(mod[17] = new Model("Models/Neon Signs/Project Nebula/signn.obj"));
	game.addModel(mod[18] = new Model("Models/Missile/untitled.obj"));



	mod[5]->setColour(0.65f, 0.65f, 0.7f);

	/// - Make New mod From Existing mod - ///
	//Players
	mod[3] = new Player(*mod[0]);
	mod[2] = new Player(*mod[0]);
	mod[1] = new Player(*mod[0]);

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
	mod[0]->getTransformer().setScale(1.2f), mod[0]->getTransformer().setPosition(1, 0, -5),
		mod[1]->getTransformer().setScale(1.2f), mod[1]->getTransformer().setPosition(-1, 0, -5),
		mod[2]->getTransformer().setScale(1.2f), mod[2]->getTransformer().setPosition(2, 0, -5),
		mod[3]->getTransformer().setScale(1.2f), mod[3]->getTransformer().setPosition(-2, 0, -5);

	//Wall Transforms
	mod[5]->getTransformer().setRotation({ 0.0f, 90.0f, 0.0f }), mod[5]->getTransformer().setPosition(15.0f, 0.0f, 21.0f), mod[5]->getTransformer().setScale(4.0f, 1.0f, 1.0f),
		mod[6]->getTransformer().setRotation({ 0.0f, 270.0f, 0.0f }), mod[6]->getTransformer().setPosition(-15.0f, 0.0f, -5.0f), mod[6]->getTransformer().setScale(4.0f, 1.0f, 1.0f),
		mod[7]->getTransformer().setRotation({ 0.0f, 0.0f, 0.0f }), mod[7]->getTransformer().setPosition(15.0f, 0.0f, 21.0f), mod[7]->getTransformer().setScale(4.52f, 1.0f, 1.0f);

	//Boss Transforms
	mod[8]->getTransformer().setRotation({ 0, 90, 0 }), mod[8]->getTransformer().setPosition(0, -1.65f, 17), mod[8]->getTransformer().setScale(4);
	mod[18]->getTransformer().setPosition(0, 2.f, 17);

	//Floor Transforms
	mod[9]->getTransformer().setScale(1.875f, 1.0f, 1.5f), mod[9]->getTransformer().setPosition(0, 0.0f, 5);

	//Street Light Transforms
	mod[10]->getTransformer().setScale(0.5), mod[10]->getTransformer().setPosition(13, 0, -1),
		mod[11]->getTransformer().setScale(0.5), mod[11]->getTransformer().setPosition(13, 0, 7),
		mod[12]->getTransformer().setScale(0.5), mod[12]->getTransformer().setPosition(13, 0, 15),
		mod[13]->getTransformer().setScale(0.5), mod[13]->getTransformer().setPosition(-13, 0, -1), mod[13]->getTransformer().setRotation({ 0.0f,180.0f,0.0f }),
		mod[14]->getTransformer().setScale(0.5), mod[14]->getTransformer().setPosition(-13, 0, 7), mod[14]->getTransformer().setRotation({ 0.0f,180.0f,0.0f }),
		mod[15]->getTransformer().setScale(0.5), mod[15]->getTransformer().setPosition(-13, 0, 15), mod[15]->getTransformer().setRotation({ 0.0f,180.0f,0.0f });

	//Bench Transforms
	mod[16]->getTransformer().setPosition(-13, 0, 3);

	//Sign Transformations
	//Project Nebula Sign
	mod[17]->getTransformer().setPosition(9, 1.1f, 21), mod[17]->getTransformer().setScale(3);

	/// - Set Model Colour - ///
	//Players
	mod[0]->setColour(1, 0, 0);
	mod[1]->setColour(0, 0, 1);
	mod[2]->setColour(0, 1, 0);
	mod[3]->setColour(1, 1, 0);

	//Floor
	mod[9]->setColour(196.0f / 255, 167.0f / 255, 113.0f / 255);

	/// - Add Duplicate mod - ///

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
	game.backgroundColour(0.05f, 0.0f, 0.1f);
	game.keyPressed(keyInputPressed);
	game.keyReleased(keyInputReleased);
	game.mouseButtonReleased(mouseButtonReleased);
	game.gameLoopUpdate(update);
	game.run();//this one is pretty important
	//the game ended... why are you here?... leave
	//Or run it again... ;)
	return 0;
}