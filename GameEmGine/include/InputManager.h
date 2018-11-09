#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "StructInfo.h"

/***Enums***/

enum  KeyState
{
	KEY_RELEASED,
	KEY_PRESSED,
	KEY_HELD
};

enum ModKey
{
	MODI_SHIFT = 1,
	MODI_CONTROL = 2,
	MODI_ALT = 4,
	MODI_SUPER = 8
};

enum MouseState
{
	MOUSE_RELEASED,
	MOUSE_PRESSED
};

enum MouseButton
{
	MOUSE_BUTTON_1,
	MOUSE_BUTTON_2,
	MOUSE_BUTTON_3,
	MOUSE_BUTTON_4,
	MOUSE_BUTTON_5,
	MOUSE_BUTTON_6,
	MOUSE_BUTTON_7,
	MOUSE_BUTTON_8,

	LEFT_BUTTON = 0,
	RIGHT_BUTTON = 1,
	MIDDLE_BUTTON = 2,
	LAST_BUTTON = 7
};

enum XinputButtons
{
	A, B, X, Y, LB, RB, SELECT, START, L_STICK_BUTTON, R_STICK_BUTTON, DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT
};

enum XinputSticks
{
	LS, RS
};

enum XinputTriggers
{
	LT, RT
};
/***Structs***/

struct Xinput
{
	//Controller Name
	const char* name;

	//Values from -1 -> 1
	Coord2D *Coord2D_sticks;

	//Values from  0 -> 1
	float* triggers;

	union
	{
		struct
		{
			unsigned char	A, B, X, Y, LB, RB, SELECT, START, L_STICK_BUTTON, R_STICK_BUTTON, DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT=' ';
		};
		unsigned char data[14];
	};

	//counter variables
	int numSticks = 0, numTriggers = 0, numButtons = 0;

	//Stick update is taken care of in GameEmGine automatically 
	void updateSticks(int index)
	{
		delete[] Coord2D_sticks;
		float* sticks = (float*) glfwGetJoystickAxes(index, &numSticks);
		this->Coord2D_sticks = new Coord2D[(numSticks - 2) / 2];
		for(int a = 0; a < numSticks - 2; a += 2)
			this->Coord2D_sticks[a / 2] = {sticks[a],sticks[a + 1]};
	}

	const float getStickDeadZone()
	{
		return stickDeadZone;
	}
	const float getTriggerDeadZone()
	{
		return triggerDeadZone;
	}
	void setStickDeadZone(float val)
	{
		stickDeadZone = val;
	}

	void setTriggerDeadZone(float val)
	{
		triggerDeadZone = val;
	}

	static bool buttonPressed(const char button)
	{
		return button == GLFW_PRESS;
	}

	static bool buttonReleased(const char button)
	{
		return button == GLFW_RELEASE;
	}

private:
	float stickDeadZone = 0, triggerDeadZone = 0;
};

/***Class***/

class InputManager
{
public:

	InputManager();
	~InputManager();

	void mouseButtonPressCallback(void mouseButton(int button, int mods));

	void mouseButtonReleaseCallback(void mouseButton(int button, int mods));

	void mouseButtonAllCallback(void mouseButton(int state, int button, int mods));

	static Coord2D getMouseCursorPosition();

	/*
	Callback for whenever any key is pressed or held
	*/
	void keyPressedCallback(void key(int key, int mod));

	/*
	Callback for whenever any key is released
	*/
	void keyReleasedCallback(void key(int key, int mod));

	/*
	Callback for any key pressed or released
	*/
	void keyAllCallback(void key(int state, int key, int mod));

	void controllerConnectedCallback(void controllerConnection(int controllerNum));

	void controllerDisconnectedCallback(void controllerConnection(int controllerNum));

	void controllerAllConnectionCallback(void controllerConnection(int controllerNum, int connected));

	int controllersConnected();

	bool isControllerConnected(unsigned int index);

	Xinput& getController(unsigned int index);

	static void controllerUpdate();

	//template<class T> friend void operator=(T[], T*);

private:
	static Xinput m_controllers[16];

	static void mouseButtonUpdate(GLFWwindow *, int button, int action, int mods);
	static void keyUpdate(GLFWwindow *, int key, int scancode, int action, int mods);
	static void xinputConnectionUpdate(int controller, int connected);
	static void(*_keyUp)(int, int), (*_keyInitDown)(int, int), (*_keyAll)(int, int, int),
		(*_mouseButtonPress)(int, int), (*_mouseButtonRelease)(int, int), (*_mouseButtonAll)(int, int, int),
		(*m_controllerConnection)(int, int), (*m_controllerConneced)(int), (*m_controllerDisconnected)(int);
};

//template<class T>
// void operator=(T a[], T* b)
//{
//	int size = sizeof(a) / sizeof(T);
//	for(int c = 0; c < size; c++)
//		a[c] = b[c];
//}
