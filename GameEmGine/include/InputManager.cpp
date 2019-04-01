#include "InputManager.h"

#pragma region Static Variables
std::function<void(int)>
InputManager::m_controllerConneced,
InputManager::m_controllerDisconnected;
std::function<void(int, int)>
InputManager::_keyUp,
InputManager::_keyInitDown,
InputManager::_mouseButtonPress,
InputManager::_mouseButtonRelease,
InputManager::m_controllerConnection;
std::function<void(int, int, int)>
InputManager::_keyAll,
InputManager::_mouseButtonAll;
#pragma endregion

InputManager::InputManager()
{
	glfwInit();
	glfwSetKeyCallback(glfwGetCurrentContext(), keyUpdate);
	glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouseButtonUpdate);
	//glfwSetJoystickCallback(xinputConnectionUpdate);
}

InputManager::~InputManager()
{}

void InputManager::mouseButtonPressCallback(std::function<void(int, int)>mouseButton)
{
	_mouseButtonPress = mouseButton;
}

void InputManager::mouseButtonReleaseCallback(std::function<void(int, int)>mouseButton)
{
	_mouseButtonRelease = mouseButton;
}

void InputManager::mouseButtonUpdate(GLFWwindow *, int button, int state, int mods)
{
	if(_mouseButtonAll != nullptr)
		_mouseButtonAll(state, button, mods);

	if(state == MOUSE_PRESSED)
		if(_mouseButtonPress != nullptr)
			_mouseButtonPress(button, mods);
	if(state == MOUSE_RELEASED)
		if(_mouseButtonRelease != nullptr)
			_mouseButtonRelease(button, mods);
}

void InputManager::keyUpdate(GLFWwindow *, int key, int scancode, int state, int mods)
{
	scancode;
	if(_keyAll != nullptr)
		_keyAll(state, key, mods);

	if(state == KEY_PRESSED)  //Key has been pressed initially
		if(_keyInitDown != nullptr)
			_keyInitDown(key, mods);
	if(state == KEY_RELEASED) //more of a stroke since release is only called if a key is pressed then released
		if(_keyUp != nullptr)
			_keyUp(key, mods);
}

void InputManager::xinputConnectionUpdate(int controller, int connected)
{
	controllerUpdate();
	if(m_controllerConnection != nullptr)
		m_controllerConnection(controller, connected);

	if(m_controllerConneced != nullptr)
		if(connected == GLFW_CONNECTED)
			m_controllerConneced(controller);

	if(m_controllerDisconnected != nullptr)
		if(connected == GLFW_DISCONNECTED)
			m_controllerDisconnected(controller);
}

void InputManager::mouseButtonAllCallback(std::function<void(int, int, int)>mouseButton)
{
	_mouseButtonAll = mouseButton;
}

Coord2D InputManager::getMouseCursorPosition()
{
	double x, y;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return {(float)x,(float)y};
}

void InputManager::keyPressedCallback(std::function<void(int, int)>key)
{
	_keyInitDown = key;
}

void InputManager::keyReleasedCallback(std::function<void(int, int)>key)
{
	_keyUp = key;
}

void InputManager::keyAllCallback(std::function<void(int, int, int)>key)
{
	_keyAll = key;
}

void InputManager::controllerConnectedCallback(std::function<void(int)>controllerConnection)
{
	m_controllerConneced = controllerConnection;
}

void InputManager::controllerDisconnectedCallback(std::function<void(int)>controllerConnection)
{
	m_controllerDisconnected = controllerConnection;
}

void InputManager::controllerAllConnectionCallback(std::function<void(int, int)>connected)
{
	m_controllerConnection = connected;
}

int InputManager::controllersConnected()
{
	int count = 0;
	for(int a = 0; a < 4; a++)
		count += XinputManager::controllerConnected(a);
	return count;
}

bool InputManager::isControllerConnected(unsigned int m_index)
{
	return XinputManager::controllerConnected(m_index);
}

XinputDevice& InputManager::getController(unsigned int m_index)
{
	return *XinputManager::getController(m_index);
}

void InputManager::controllerUpdate()
{

	XinputManager::update();
}