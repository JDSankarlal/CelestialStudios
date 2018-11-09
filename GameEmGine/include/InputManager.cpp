#include "InputManager.h"

#pragma region Static Variables
Xinput InputManager::m_controllers[16];
void(*InputManager::_keyUp)(int, int), (*InputManager::_keyInitDown)(int, int), (*InputManager::_keyAll)(int, int, int),
(*InputManager::_mouseButtonPress)(int, int), (*InputManager::_mouseButtonRelease)(int, int), (*InputManager::_mouseButtonAll)(int, int, int),
(*InputManager::m_controllerConnection)(int, int), (*InputManager::m_controllerConneced)(int), (*InputManager::m_controllerDisconnected)(int);
#pragma endregion

InputManager::InputManager()
{
	glfwInit();
	glfwSetKeyCallback(glfwGetCurrentContext(), keyUpdate);
	glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouseButtonUpdate);
	glfwSetJoystickCallback(xinputConnectionUpdate);
}

InputManager::~InputManager()
{}

void InputManager::mouseButtonPressCallback(void mouseButton(int button, int mods))
{
	_mouseButtonPress = mouseButton;
}

void InputManager::mouseButtonReleaseCallback(void mouseButton(int button, int mods))
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

void InputManager::mouseButtonAllCallback(void mouseButton(int state, int button, int mods))
{
	_mouseButtonAll = mouseButton;
}

Coord2D InputManager::getMouseCursorPosition()
{
	double x, y;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return {(float) x,(float) y};
}

void InputManager::keyPressedCallback(void key(int key, int mod))
{
	_keyInitDown = key;
}

void InputManager::keyReleasedCallback(void key(int key, int mod))
{
	_keyUp = key;
}

void InputManager::keyAllCallback(void key(int state, int key, int mod))
{
	_keyAll = key;
}

void InputManager::controllerConnectedCallback(void controllerConnection(int controllerNum))
{
	m_controllerConneced = controllerConnection;
}

void InputManager::controllerDisconnectedCallback(void controllerConnection(int controllerNum))
{
	m_controllerDisconnected = controllerConnection;
}

void InputManager::controllerAllConnectionCallback(void connected(int controllerNum, int connected))
{
	m_controllerConnection = connected;
}

int InputManager::controllersConnected()
{
	int count = 0;
	for(int a = 0; a < GLFW_JOYSTICK_LAST; a++)
		count += glfwJoystickPresent(a);
	return count;
}

bool InputManager::isControllerConnected(unsigned int index)
{
	return glfwJoystickPresent(index);
}

Xinput& InputManager::getController(unsigned int index)
{
	return m_controllers[index];
}

void InputManager::controllerUpdate()
{
							
	for(int a = 0; a < GLFW_JOYSTICK_LAST; a++)
		if(glfwJoystickPresent(a))
		{
			m_controllers[a].name = glfwGetJoystickName(a);
			unsigned char* tmp = (unsigned char*) glfwGetJoystickButtons(a, &m_controllers[a].numButtons);
			for(int b = 0; b < m_controllers[a].numButtons; b++)
				m_controllers[a].buttons.data[b] = tmp[a];
			m_controllers[a].updateSticks(a);
			m_controllers[a].triggers = (float*) glfwGetJoystickAxes(a, &m_controllers[a].numSticks) + m_controllers[a].numSticks - 2;
			m_controllers[a].numSticks -= 2;
			m_controllers[a].numSticks /= 2;
			m_controllers[a].numTriggers = 2;

			//Stick Dead Zone application
			for(int b = 0; b < m_controllers[a].numSticks; b++)
				for(int c = 0; c < 2; c++)
					if((abs(m_controllers[a].Coord2D_sticks[b][c])) <= m_controllers[a].getStickDeadZone())
						m_controllers[a].Coord2D_sticks[b][c] = 0;

			//modify trigger values
			m_controllers[a].triggers[0] = (m_controllers[a].triggers[0] + 1) * .5f;//triggers have values from 0 -> 1
			m_controllers[a].triggers[1] = (m_controllers[a].triggers[1] + 1) * .5f;//triggers have values from 0 -> 1

			//Trigger Dead Zone application
			for(int b = 0; b < m_controllers[a].numTriggers; b++)
				if(m_controllers[a].triggers[b] <= m_controllers[a].getTriggerDeadZone())
					m_controllers[a].triggers[b] = 0;
		}
}
