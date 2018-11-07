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

#define modSize 5
GameEmGine game("The Real Game", 1000, 800, 0, 0, 0, false);
GLSLCompiler colourProgram, colourProgram2;
Logger tlog = Logger("New Log:>");
Model *mod[modSize];

void shaderInit()
{
	//shader initialization
	colourProgram.compileShaders("Shaders/Colour Shading.vtsh", "Shaders/Colour Shading.fmsh");
	colourProgram.linkShaders();
	colourProgram2.compileShaders("Shaders/Colour Shading 2.vtsh", "Shaders/Colour Shading 2.fmsh");
	colourProgram2.linkShaders();
}

void keyInputPressed(int key, int mod)
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

void keyInputReleased(int key, int mod)
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
		//Coord2D tmp = rightM - leftM;
		//float length = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
		//printf("%f\n\n", length);
		static CAMERA_TYPE type = PERSPECTIVE;
		game.setCameraType(type = type == ORTHOGRAPHIC ? PERSPECTIVE : ORTHOGRAPHIC);
	}

	if (key == 'R') //resets the camera
	{
		GameEmGine::m_modelShader->refresh();

		//game.setCameraAngle(0, { 1, 1, 1 });
		//	game.setCameraPosition({0,0,0});
	}
	printf("key RELEASED code: %d\n\n", key);
}

void update()
{
	float move = .1;

	//mod[1]->getTransformer().translateBy(mod[0]->getTransformer().get);


	if (game.isControllerConnected(0))
	{
	
		Xinput p1 = game.getController(0);
	
		if (Xinput::buttonPressed(p1.buttons.A))
			printf("%d\n", p1.buttons.A);
	
		mod[0]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	
		float angle = 0;
		if (p1.sticks[RS].x) {
			angle = acos(p1.sticks[RS].x /
				sqrt(p1.sticks[RS].x*p1.sticks[RS].x
					+ p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
			angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
		}
	
		mod[0]->getTransformer().setRotation({ 0,angle	,0 });
	
	//	mod[0]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
	//	mod[0]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
	}
	//
	//if (game.isControllerConnected(1))
	//{
	//
	//	Xinput p1 = game.getController(1);
	//
	//	if (Xinput::buttonPressed(p1.buttons.A))
	//		printf("%d\n", p1.buttons.A);
	//
	//	float angle = 0;
	//	if (p1.sticks[RS].x)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	mod[1]->getTransformer().setRotation({ 0,angle	,0 });
	//	mod[1]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//
	////	mod[1]->getTransformer().translateBy(0, -p1.triggers[LT] * move, 0);
	////	mod[1]->getTransformer().translateBy(0, p1.triggers[RT] * move, 0);
	//
	//}
	//
	//if (game.isControllerConnected(2))
	//{
	//
	//	Xinput p1 = game.getController(2);
	//
	//	if (Xinput::buttonPressed(p1.buttons.A))
	//		printf("%d\n", p1.buttons.A);
	//
	//	float angle = 0;
	//	if (p1.sticks[RS].x)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	mod[2]->getTransformer().setRotation({ 0,angle	,0 });
	//	mod[2]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//}
	//
	//if (game.isControllerConnected(3))
	//{
	//
	//	Xinput p1 = game.getController(3);
	//
	//	if (Xinput::buttonPressed(p1.buttons.A))
	//		printf("%d\n", p1.buttons.A);
	//
	//	float angle = 0;
	//	if (p1.sticks[RS].x)
	//	{
	//		angle = acos(p1.sticks[RS].x /
	//					 sqrt(p1.sticks[RS].x*p1.sticks[RS].x
	//					 + p1.sticks[RS].y*p1.sticks[RS].y)) * (180 / M_PI);
	//		angle += (p1.sticks[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
	//	}
	//
	//	mod[3]->getTransformer().setRotation({ 0,angle	,0 });
	//
	//	mod[3]->getTransformer().translateBy(p1.sticks[LS].x * move, 0, p1.sticks[LS].y * move); //move camera
	//}

	////Model Movement
	//if (m_in)
	//	mod[numModel]->getTransformer().translateBy(0, 0, move);
	//else if (m_out)
	//	mod[numModel]->getTransformer().translateBy(0, 0, -move);
	//if (m_up)
	//	mod[numModel]->getTransformer().translateBy(0, move, 0);
	//else if (m_down)
	//	mod[numModel]->getTransformer().translateBy(0, -move, 0);
	//if (m_right)
	//	mod[numModel]->getTransformer().translateBy(move, 0, 0);
	//else if (m_left)
	//	mod[numModel]->getTransformer().translateBy(-move, 0, 0);
	//
	//
	//
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
	////Rotate Model
	//if (rotUp)
	//	mod[numModel]->getTransformer().rotateBy({ ang,0,0 });
	//else if (rotDown)
	//	mod[numModel]->getTransformer().rotateBy({ -ang,0,0 });
	//if (rotRight)
	//	mod[numModel]->getTransformer().rotateBy({ 0,ang,0 });
	//else if (rotLeft)
	//	mod[numModel]->getTransformer().rotateBy({ 0,-ang,0 });
}

void mouseButtonReleased(int button, int mod)
{

	if (button == LEFT_BUTTON)
		leftM = InputManager::getMouseCursorPosition();
	if (button == RIGHT_BUTTON)
		rightM = InputManager::getMouseCursorPosition();
}

void render()
{
}

SpriteInfo sp1, sp2;

void main()
{
	//Model Stuff
	//Model *floor;
	//game.addModel(floor = new Model("models/floor/placeholder_floor.obj"));
	//floor->getTransformer().setScale(500);

	/// - Load Models into Scene - ///
	game.addModel(mod[0] = new Model("Models/crysis-nano-suit-2(OBJ)/scene.obj"));
	game.addModel(mod[5] = new Model("Models/PlaceholderWalls/PlaceholderBox.obj"));
	//game.addModel(mod[4] = new Model("models/Bruce+Lee+obj/Bruce Lee.obj"));

	/// - Sets Model Transform - ///

	
	//mod[4]->getTransformer().setScale(.2);
	

	/// - Make New Models From Existing Models - ///

	//Players
	mod[3] = new Model(*mod[0]);
	mod[2] = new Model(*mod[0]);
	mod[1] = new Model(*mod[0]);

	//Placeholder Walls
	mod[6] = new Model(*mod[5]);

	mod[0]->getTransformer().setScale(.15), mod[0]->getTransformer().setPosition(1, 0, 0),
		mod[1]->getTransformer().setScale(.15), mod[1]->getTransformer().setPosition(-1, 0, 0),
		mod[2]->getTransformer().setScale(.15), mod[2]->getTransformer().setPosition(2, 0, 0),
		mod[3]->getTransformer().setScale(.15), mod[3]->getTransformer().setPosition(-2, 0, 0);

	mod[5]->getTransformer().setRotation({ 0, 60, 0 }), mod[5]->getTransformer().setPosition(7, 0, 4), mod[5]->getTransformer().setScale(2, 1, 1),
		mod[6]->getTransformer().setRotation({ 0, -60, 0 }), mod[6]->getTransformer().setPosition(-7, 0, 4), mod[6]->getTransformer().setScale(2, 1, 1);
	

	/// - Set Model Colour - ///

	mod[0]->setColour(1, 0, 0);
	mod[1]->setColour(0, 1, 0);
	mod[2]->setColour(0, 0, 1);
	mod[3]->setColour(1, 1, 0);

	game.addModel(mod[1]);

	game.addModel(mod[2]);

	game.addModel(mod[3]);

	game.addModel(mod[6]);

	game.setCameraPosition({ 0,3,-20 });
	game.setCameraAngle(-45, { 1,0,0 });

	EmGineAudioPlayer audio;

	audio.createStream("Game Jam(Full).wav");

	//audio.play(true);

	//engine stuff
	game.setFPSLimit(60);
	game.keyPressed(keyInputPressed);
	game.keyReleased(keyInputReleased);
	game.mouseButtonReleased(mouseButtonReleased);
	game.gameLoopUpdate(update);
	game.run();

	//the game ended... why are you here?... leave
}