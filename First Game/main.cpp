#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <vector>

using namespace std;

float ang = 2;
int numModel = 0;
bool m_left = 0, m_right = 0, m_in = 0, m_out = 0, m_up = 0, m_down = 0,
rotLeft = 0, rotRight = 0, rotUp = 0, rotDown = 0;
Coord2D leftM, rightM;
EmGineAudioPlayer* omniPlayer;

#define modSize 3 
GameEmGine game("The Real Game", 1000, 800, 0, 0, 0, false);
GLSLCompiler colourProgram, colourProgram2;
Logger tlog = Logger("New Log:>");
Model* mod[modSize];

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
	if(key == GLFW_KEY_KP_6)
		game.setFPSLimit(game.getFPSLimit() + 1);
	if(key == GLFW_KEY_KP_4)
		game.setFPSLimit(game.getFPSLimit() - 1);

	if(key == GLFW_KEY_F)//Toggles Fullscreen
	{
		static bool full;
		game.getWindow()->setFullScreen(full = !full);
		printf("Full Screen: %s\n", full ? "true" : "false");
	}

	if(key == GLFW_KEY_SPACE)//changes the model that is being moved
	{
		Coord2D tmp = rightM - leftM;
		float length = sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
		printf("%f\n\n", length);
	}



	if(key == 'R')//resets the camera
	{
		game.setCameraAngle(0, {1,1,1});
	//	game.setCameraPosition({0,0,0});
	}
	printf("key RELEASED code: %d\n\n", key);
}


void update()
{
	float move = 20;

	//Model Movement
	if(m_in)
		mod[numModel]->getTransformer().translateBy(0, 0, move);
	else if(m_out)
		mod[numModel]->getTransformer().translateBy(0, 0, -move);
	if(m_up)
		mod[numModel]->getTransformer().translateBy(0, move, 0);
	else if(m_down)
		mod[numModel]->getTransformer().translateBy(0, -move, 0);
	if(m_right)
		mod[numModel]->getTransformer().translateBy(move, 0, 0);
	else if(m_left)
		mod[numModel]->getTransformer().translateBy(-move, 0, 0);



	if(game.isControllerConnected(0))
	{

		Xinput p1 = game.getController(0);

		p1.numButtons;
		p1.numSticks;

		if(Xinput::buttonPressed(p1.buttons.A))
			printf("%d\n", p1.buttons.A);

		game.moveCameraPositionBy({p1.sticks[LS].x * move , 0 * move, p1.sticks[LS].y * move});//move camera
		//game.moveCameraAngleBy(ang * (abs(p1.sticks[RS].x) + abs(p1.sticks[RS].y)), {p1.sticks[RS].y  ,p1.sticks[RS].x, 0});//rotate camera
		//game.moveCameraPositionBy({0 , 0,p1.triggers[LT] * -move});//move out
		

	}
	game.setCameraPosition({ 0,5000,0 });//move in
	game.setCameraAngle(-45,{1,0,0});
	//Rotate Model
	if(rotUp)
		mod[numModel]->getTransformer().rotateBy({ang,0,0});
	else if(rotDown)
		mod[numModel]->getTransformer().rotateBy({-ang,0,0});
	if(rotRight)
		mod[numModel]->getTransformer().rotateBy({0,ang,0});
	else if(rotLeft)
		mod[numModel]->getTransformer().rotateBy({0,-ang,0});
}



void mouseButtonReleased(int button, int mod)
{

	if(button == LEFT_BUTTON)
		leftM = InputManager::getMouseCursorPosition();
	if(button == RIGHT_BUTTON)
		rightM = InputManager::getMouseCursorPosition();


}

void render()
{}

SpriteInfo sp1, sp2;

void main()
{
	//Model Stuff
	Model *floor;
	Model *m1;
	game.addModel(floor = new Model("Models/Floor/placeholder_floor.obj"));
	game.addModel(m1 = new Model("Models/crysis_nano_suit_2/scene.gltf"));
	mod[0] = m1;
	floor->getTransformer().setScale(50);
	m1->getTransformer().setScale(.15);
	m1->getTransformer().setPosition(500, 0, 1000);


	//engine stuff
	game.keyPressed(keyInputPressed);
	game.keyReleased(keyInputReleased);
	game.mouseButtonReleased(mouseButtonReleased);
	game.gameLoopUpdate(update);
	game.run();

	//the game ended... why are you here?... leave
}