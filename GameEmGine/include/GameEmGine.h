#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <vector>
#include <string>
#include <thread>
#include "ShaderCombiner.h"
//#include "EmGineAudioPlayer.h"
//#include "Sprite.h"
#include "Shader.h"
#include "WindowCreator.h"
#include "Camera.h"
#include "Model.h"
//#include "ModelBatch.h"
#include "InputManager.h"
#include "FrameBuffer.h"
//will get joystic input latter :>



class GameEmGine
{
public:
	GameEmGine();
	GameEmGine(std::string name, int width, int height, int x = 0, int y = 0, int monitor = 0, bool fullScreen = false, bool visable = true);
	~GameEmGine();
#define sleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))


	/*
	Creates a new window
	*/
	void createNewWindow(std::string name, int width, int height, int x = 0, int y = 0, int monitor = 0, bool fullScreen = false, bool visable = true);
	/*
	Runs the engine.
	does not exit loop until window is exited
	*/
	void run();
	/*
	Exists the game
	*/
	void exit();

	/*
	Callback for whenever any key is pressed
	*/
	void keyPressed(void key(int key, int mod));
	/*
	Callback for whenever any key is released
	*/
	void keyReleased(void key(int key, int mod));
	/*

	/*
	Callback for whenever any mouse button is pressed
	*/
	void mouseButtonPressed(void button(int key, int mod));
	/*
	Callback for whenever any mouse button is released
	*/
	void mouseButtonReleased(void button(int key, int mod));
	/*


	What the game must render each update
	*/
	void renderUpdate(void update());
	/*
	  intializes any shaders
	*/
	void shaderInit(void shaders());
	/*
	 What the game must do each update
	*/
	void gameLoopUpdate(void update(double));
	/*
	Set background colour
	*/
	void backgroundColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
	/*
	Gets window width in pixles
	*/
	int getWindowWidth();
	/*
	Gets window height in pixles
	*/
	int getWindowHeight();

	void setCameraType(CAMERA_TYPE type);

	/*
	moves the camera position in pixles
	*/
	void moveCameraPositionBy(Coord3D pos);
	/*
	sets the camera position in pixles
	*/
	void setCameraPosition(Coord3D pos);

	/*
	moves the camera angle
	*/
	void moveCameraAngleBy(float angle, Coord3D direction);

	/*
		moves the camera angle
	*/
	void setCameraAngle(float angle, Coord3D direction);

	void addModel(Model* model);

	void removeModel(Model * model);

	/*does not work!!!!*/
	void addModelBatch(const char *model);


	void removeSprite(int index);


	void addCamera(Camera* camera);


	void setFPSLimit(short limit);
	short getFPSLimit();

	/*enables/disables V-sync*/
	void vsync(bool enable);

	void updateControllerConnections();

	WindowCreator * getWindow();

	/*Controller input*/

	int controllersConnected();

	bool isControllerConnected(int index);

	Xinput& getController(int index);

	static Shader *m_cameraShader, *m_modelShader;

private:
	void shaderInit();
	void calculateFPS();
	void fpsLimiter();

	/*static void InitOpenGlCallback ();
	static void OpenGLDebugCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data);
	*/
	static void update();
	static void changeViewport(GLFWwindow * win, int w, int h);
	static void(*m_compileShaders)(), (*m_render)(), (*m_gameLoop)(double);
	static WindowCreator *m_window;
	static ColourRGBA m_colour;
	static Camera *m_mainCamera;
	static std::vector<Camera*> m_cameras;
//	static ModelBatch* m_modelBatch;
	static FrameBuffer m_mainBuffer;
	static InputManager *m_inputManager;
	static std::vector<Model*> m_models;

	static bool exitGame;
	static float m_fps;
	static short m_fpsLimit;
};

