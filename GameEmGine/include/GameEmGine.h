#pragma once
#include "InputManager.h"//needs to be at top
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <ctime>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <utility>
#include "Scene.h"
#include "Shader.h"
#include "WindowCreator.h"
#include "Camera.h"
#include "Model.h"
#include "ExtraMath.h"
#include "FrameBuffer.h"
#include "LightSource.h"
#include "Text.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//#define Game GameEmGine
//#define sleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

class GameEmGine
{
public:
	GameEmGine() = delete;
	static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	static void init(std::string name, int width, int height, int x = 0, int y = 0, int monitor = 0, bool fullScreen = false, bool visable = true);


	/*
	Creates a new window
	*/
	static void createNewWindow(std::string name, int width, int height, int x = 0, int y = 0, int monitor = 0, bool fullScreen = false, bool visable = true);

	/*
	Runs the engine.
	does not exit loop until window is exited or void exit(); is called
	*/
	static void run();

	/*
	Exists the game
	*/
	static void exit();


	static void setScene(Scene* scene);

	/*
	Set background colour in sigle bite (255 per colour) format
	*/
	static void setBackgroundColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);


	/*
	Gets window width in pixels
	*/
	static int getWindowWidth();
	/*
	Gets window height in pixels
	*/
	static int getWindowHeight();


	/*
	Gets window size in pixels
	*/
	static Coord3D<int> getWindowSize();

	static Camera* getMainCamera();

	static bool mouseCollision(Model* model);

	static void setCameraType(CAMERA_TYPE type, ProjectionPeramiters* proj = nullptr);

	/*
	moves the camera position in pixels
	*/
	static void translateCameraBy(Coord3D<> pos);
	/*
	sets the camera position in pixels
	*/
	static void setCameraPosition(Coord3D<> pos);

	/*
	moves the camera angle
	*/
	static void rotateCameraBy(Coord3D<> direction);

	/*
		sets the camera angle
	*/
	static void setCameraRotation(Coord3D<> direction);

	static void addModel(Model* model);

	static void addText(Text* text);

	static void removeModel(Model* model);

	static void removeText(Text* text);

	static void addCamera(Camera* camera);


	static void setFPSLimit(short limit);
	static short getFPSLimit();

	/*enables/disables V-sync*/
	static void vsync(bool enable);

	static void updateControllerConnections();

	static WindowCreator* getWindow();

	/*Controller input*/

	static int controllersConnected();

	static bool isControllerConnected(int m_index);

	static XinputDevice* getController(int m_index);

	static Shader* m_modelShader, * m_postProcess, * m_forwardRender, * m_grayScalePost, * m_bloomHighPass, * m_blurHorizontal, * m_blurVertical, * m_blurrComposite, * m_sobel, * m_shadows;

	static Texture2D* m_LUT;

	static bool lutActive, toonActive;

private:
	static void shaderInit();
	static void calculateFPS();
	static void fpsLimiter();


	/*
	static void InitOpenGlCallback ();
	static void OpenGLDebugCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data);
	*/

	static void update();
	static void changeViewport(GLFWwindow* win, int w, int h);
	static void(*m_compileShaders)();
	static std::function<void()>m_buffer;
	static std::function<void(double)> m_gameLoop;
	static WindowCreator* m_window;
	static ColourRGBA m_colour;
	static Camera* m_mainCamera;
	static std::vector<Camera*> m_cameras;
	static FrameBuffer* m_mainFrameBuffer, * m_postBuffer, * m_buffer1, * m_buffer2, * m_greyscaleBuffer, * m_outline, * m_shadowBuffer;
	static std::unordered_map<std::string, FrameBuffer*> m_frameBuffers;
	static std::map<void*, Model*> m_models;
	static Scene* m_mainScene;
	static std::vector<Text*> m_text;

	static bool exitGame;
	static float m_fps;
	static short m_fpsLimit;

	//static GLuint colorCustom;
	//static int LUTsize;
};

