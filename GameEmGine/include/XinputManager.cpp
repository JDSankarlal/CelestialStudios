#include "XinputManager.h"

XinputDevice* XinputManager::controllers[4];

void XinputManager::update()
{

	for(int m_index = 0; m_index < 4; m_index++)
	{
		if(!controllers[m_index])
			controllers[m_index] = new XinputDevice;



		if(controllerConnected(m_index))
		{
			if(controllers[m_index]->type != getControllerType(m_index))
			{
				XinputController* controller = new XinputController;
				XinputDrum* drum = new XinputDrum;
				XinputGuitar* guitar = new XinputGuitar;

				switch(getControllerType(m_index))
				{
				case XINPUT_CONTROLLER:
					*controller = *(XinputController*)controllers[m_index];
					delete controllers[m_index];
					controllers[m_index] = controller;
					delete drum;
					delete guitar;
					controller->type = XINPUT_CONTROLLER;
					break;
				case XINPUT_GUITAR:
					*guitar = *(XinputGuitar*)controllers[m_index];
					delete controllers[m_index];
					controllers[m_index] = guitar;
					delete controller;
					delete drum;
					controller->type = XINPUT_GUITAR;
					break;
				case XINPUT_DRUM:
					*drum = *(XinputDrum*)controllers[m_index];
					delete controllers[m_index];
					controllers[m_index] = drum;
					delete controller;
					delete guitar;
					controller->type = XINPUT_DRUM;
					break;
				}
			}
			controllers[m_index]->m_index = m_index;
			controllers[m_index]->update();
		}
	}
}

bool XinputManager::controllerConnected(int m_index)
{
	XINPUT_STATE connected;
	return XInputGetState(m_index, &connected) == ERROR_SUCCESS;
}

CONTROLLER_TYPE XinputManager::getControllerType(int m_index)
{
	XINPUT_CAPABILITIES info;
	XInputGetCapabilities(m_index, NULL, &info);

	switch(info.SubType)
	{
	case XINPUT_DEVSUBTYPE_GAMEPAD:
		return XINPUT_CONTROLLER;
	case XINPUT_DEVSUBTYPE_GUITAR:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_GUITAR_BASS:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_DRUM_KIT:
		return XINPUT_DRUM;
	}

	return XINPUT_UNKNOWN;
}

XinputDevice* XinputManager::getController(int m_index)
{
	return controllers[m_index];
}

