#define _USE_MATH_DEFINES // for C++
#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <vector>

using namespace std;

float ang = 2;
int numModel = 0;
bool m_left = 0, m_right = 0, m_in = 0, m_out = 0, m_up = 0, m_down = 0,
rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0;
Coord2D leftM, rightM;
EmGineAudioPlayer *omniPlayer;

#define modSize 20
GameEmGine game("The Real Game", 1000, 800, 0, 0, 0, false);
GLSLCompiler colourProgram, colourProgram2;
Model *mod[modSize];

//shader initialization
void shaderInit()
{
	//shader initialization
	colourProgram.compileShaders("Shaders/Colour Shading.vtsh", "Shaders/Colour Shading.fmsh");
	colourProgram.linkShaders();
	colourProgram2.compileShaders("Shaders/Colour Shading 2.vtsh", "Shaders/Colour Shading 2.fmsh");
	colourProgram2.linkShaders();
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
		//Coord2D tmp = rightM - leftM;
		//float length = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
		//printf("%f\n\n", length);
		static CAMERA_TYPE type = PERSPECTIVE;
		game.setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
	}

	if(key == 'R') //resets the camera
	{
		GameEmGine::m_modelShader->refresh();

		//game.setCameraAngle(0, { 1, 1, 1 });
		//	game.setCameraPosition({0,0,0});
	}
	printf("key RELEASED code: %d\n\n", key);
}

//updates within game loop
void update()
{
	float move = .1;

	static Model* bullets[4];
	static Coord3D velocity[4];

	for(int a = 0; a < 4; a++)
		if(game.isControllerConnected(a))
		{
			Xinput p1 = game.getController(a);


			static float angle[4] = { 0,0,0,0 };
			if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
			{

				angle[a] = acos(p1.Coord2D_sticks[RS].x /
								sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
								+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / M_PI);
				angle[a] += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
				angle[a] = fmodf(angle[a], 360);
			}

			if(p1.triggers[RT] >= .95)
			{
				if(bullets[a])
				{
					game.removeModel(bullets[a]);
					//	delete bullets[0];
				}

				game.addModel(bullets[a] = new Model(*mod[a]));
				Coord3D pos = mod[a]->getTransformer().getPosition();
				bullets[a]->getTransformer().setPosition(pos.coordX, pos.coordY + 1, pos.coordZ);
				bullets[a]->getTransformer().setScale(.05);

				bullets[a]->getTransformer().setRotation({ 90 , 0, angle[a] });
				//bullets[a]->getTransformer().rotateBy({ 90 , 0, 0 });


				float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
				float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

				velocity[a] = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
			}
			if(bullets[a])
			{

				bullets[a]->getTransformer().translateBy(velocity[a].coordX, velocity[a].coordY, velocity[a].coordZ);
			}


			mod[a]->getTransformer().setRotation({ 0,angle[a]	,0 });
			mod[a]->getTransformer().translateBy(p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move); //move camera
			//	mod[0]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
			//	mod[0]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
		}

	//if(game.isControllerConnected(1))
	//{
	//
	//	Xinput p1 = game.getController(1);
	//
	//
	//
	//	float angle = 0;
	//	if(p1.sticks[RS].x || p1.sticks[RS].y)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	if(p1.triggers[RT] >= .95)
	//	{
	//		if(bullets[1])
	//		{
	//			game.removeModel(bullets[1]);
	//			//	delete bullets[0];
	//		}
	//
	//		game.addModel(bullets[1] = new Model(*mod[1]));
	//		Coord3D pos = mod[1]->getTransformer().getPosition();
	//		bullets[1]->getTransformer().setPosition(pos.x, pos.y + 1, pos.z);
	//		bullets[1]->getTransformer().setScale(.05);
	//		bullets[1]->getTransformer().rotateBy({ 90 , 0, 0 });
	//
	//
	//		float cosVal = cos(fmodf(angle - 90, 360)*(M_PI / 180));
	//		float sinVal = sin(fmodf(angle - 90, 360)*(M_PI / 180));
	//
	//		velocity[1] = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
	//	}
	//
	//
	//
	//	mod[1]->getTransformer().setRotation({ 0,angle	,0 });
	//	mod[1]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//
	////	mod[1]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
	////	mod[1]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
	//
	//}
	//
	//if(game.isControllerConnected(2))
	//{
	//
	//	Xinput p1 = game.getController(2);
	//
	//	float angle = 0;
	//	if(p1.sticks[RS].x || p1.sticks[RS].y)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	if(p1.triggers[RT] >= .95)
	//	{
	//		if(bullets[2])
	//		{
	//			game.removeModel(bullets[2]);
	//			//	delete bullets[0];
	//		}
	//
	//		game.addModel(bullets[2] = new Model(*mod[3]));
	//		Coord3D pos = mod[2]->getTransformer().getPosition();
	//		bullets[2]->getTransformer().setPosition(pos.x, pos.y + 1, pos.z);
	//		bullets[2]->getTransformer().setScale(.05);
	//		bullets[2]->getTransformer().rotateBy({ 90 , 0, 0 });
	//
	//
	//		float cosVal = cos(fmodf(angle - 90, 360)*(M_PI / 180));
	//		float sinVal = sin(fmodf(angle - 90, 360)*(M_PI / 180));
	//
	//		velocity[2] = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
	//	}
	//
	//
	//	mod[2]->getTransformer().setRotation({ 0,angle	,0 });
	//	mod[2]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//}
	//
	//if(game.isControllerConnected(3))
	//{
	//
	//	Xinput p1 = game.getController(3);
	//
	//
	//	float angle = 0;
	//	if(p1.sticks[RS].x || p1.sticks[RS].y)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	if(p1.triggers[RT] >= .95)
	//	{
	//		if(bullets[3])
	//		{
	//			game.removeModel(bullets[3]);
	//			//	delete bullets[0];
	//		}
	//
	//		game.addModel(bullets[3] = new Model(*mod[3]));
	//		Coord3D pos = mod[3]->getTransformer().getPosition();
	//		bullets[3]->getTransformer().setPosition(pos.x, pos.y + 1, pos.z);
	//		bullets[3]->getTransformer().setScale(.05);
	//		bullets[3]->getTransformer().rotateBy({ 90 , 0, 0 });
	//
	//
	//		float cosVal = cos(fmodf(angle - 90, 360)*(M_PI / 180));
	//		float sinVal = sin(fmodf(angle - 90, 360)*(M_PI / 180));
	//
	//		velocity[3] = Coord3D(cosVal * move * 2, 0, sinVal * move * 2);
	//	}
	//
	//	mod[3]->getTransformer().setRotation({ 0,angle	,0 });
	//
	//	mod[3]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//}

	//if (game.isControllerConnected(0))
	//{
	//	Xinput p1 = game.getController(0);
	//
	//	p1.numButtons;
	//	p1.numSticks;
	//
	//	if (Xinput::buttonPressed(p1.buttons.A))
	//		printf("%d\n", p1.buttons.A);
	//
	//	game.moveCameraPositionBy({ p1.sticks[LS].x * move , 0 * move, p1.sticks[LS].y * move });//move camera
	//	game.moveCameraAngleBy(ang * (abs(p1.sticks[RS].x) + abs(p1.sticks[RS].y)), { p1.sticks[RS].y  ,p1.sticks[RS].x, 0 });//rotate camera
	//	game.moveCameraPositionBy({ 0 , 0, p1.triggers[LT] * -move });//move out
	//}
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


int main()
{
	/// - Load Models into Scene - ///

	game.addModel(mod[0] = new Model("Models/crysis-nano-suit-2(OBJ)/scene.obj")); //Crysis Guy
	game.addModel(mod[5] = new Model("Models/PlaceholderWalls/PlaceholderBox.obj")); //Wall
	game.addModel(mod[8] = new Model("Models/BOSS/roughBOSS.obj")); //Boss
	game.addModel(mod[9] = new Model("Models/Floor/Floor.obj")); //Floor

	mod[5]->setColour(0.65, 0.65, 0.7);

	/// - Make New Models From Existing Models - ///
	//Players
	mod[3] = new Model(*mod[0]);
	mod[2] = new Model(*mod[0]);
	mod[1] = new Model(*mod[0]);

	//Placeholder Walls
	mod[6] = new Model(*mod[5]);
	mod[7] = new Model(*mod[5]);

	/// - Set Model Transforms - ///
	//Player Transforms
	mod[0]->getTransformer().setScale(.15), mod[0]->getTransformer().setPosition(1, 0, 0),
		mod[1]->getTransformer().setScale(.15), mod[1]->getTransformer().setPosition(-1, 0, 0),
		mod[2]->getTransformer().setScale(.15), mod[2]->getTransformer().setPosition(2, 0, 0),
		mod[3]->getTransformer().setScale(.15), mod[3]->getTransformer().setPosition(-2, 0, 0);

	//Wall Transforms
	mod[5]->getTransformer().setRotation({ 0, 90, 0 }), mod[5]->getTransformer().setPosition(15, 0, 7), mod[5]->getTransformer().setScale(3, 1, 1),
		mod[6]->getTransformer().setRotation({ 0, 90, 0 }), mod[6]->getTransformer().setPosition(-15, 0, 7), mod[6]->getTransformer().setScale(3, 1, 1),
		mod[7]->getTransformer().setRotation({ 0, 0, 0 }), mod[7]->getTransformer().setPosition(0, 0, 20.5), mod[7]->getTransformer().setScale(3, 1, 1);

	//Boss Transforms
	mod[8]->getTransformer().setRotation({ 0,90,0 }), mod[8]->getTransformer().setPosition(0, 0, 10), mod[8]->getTransformer().setScale(2.25);


	/// - Set Model Colour - ///
	//Players
	mod[0]->setColour(1, 0, 0);
	mod[1]->setColour(0, 0, 1);
	mod[2]->setColour(0, 1, 0);
	mod[3]->setColour(1, 1, 0);

	//Floor
	mod[9]->setColour((float)196 / 255, (float)167 / 255, (float)113 / 255);



	/// - Add Duplicate Models - ///

	game.addModel(mod[1]);
	game.addModel(mod[2]);
	game.addModel(mod[3]);
	game.addModel(mod[6]);
	game.addModel(mod[7]);

	/// - Set Camera - ///

	game.setCameraPosition({ 0,7,-20 });
	game.setCameraAngle(-45, { 1,0,0 });

	EmGineAudioPlayer audio;
	audio.createStream("Game Jam(Full).wav");

	audio.play(true);

	//engine stuff
	game.setFPSLimit(60);
	game.keyPressed(keyInputPressed);
	game.keyReleased(keyInputReleased);
	game.mouseButtonReleased(mouseButtonReleased);
	game.gameLoopUpdate(update);
	game.run();//this one is pretty important

	//the game ended... why are you here?... leave
	return 0;
}