#include "XinputManager.h"

XinputDevice* XinputManager::controllers[4];

void XinputManager::update()
{

	for(int index = 0; index < 4; index++)
	{
		if(!controllers[index])
			controllers[index] = new XinputDevice;



		controllers[index]->index = index;
		if(controllerConnected(index))
		{
			if(controllers[index]->type != getControllerType(index))
			{
				XinputController* controller = new XinputController;
				XinputDrum* drum = new XinputDrum;
				XinputGuitar* guitar = new XinputGuitar;

				switch(getControllerType(index))
				{
				case XINPUT_CONTROLLER:
					*controller = *(XinputController*)controllers[index];
					delete controllers[index];
					controllers[index] = controller;
					delete drum;
					delete guitar;
					break;
				case XINPUT_GUITAR:
					*guitar = *(XinputGuitar*)controllers[index];
					delete controllers[index];
					controllers[index] = guitar;
					delete controller;
					delete drum;
					break;
				case XINPUT_DRUM:
					*drum = *(XinputDrum*)controllers[index];
					delete controllers[index];
					controllers[index] = drum;
					delete controller;
					delete guitar;
					break;
				}
			}
			controllers[index]->update();
		}
	}
}

bool XinputManager::controllerConnected(int index)
{
	XINPUT_STATE connected;
	return XInputGetState(index, &connected) == ERROR_SUCCESS;
}

CONTROLLER_TYPE XinputManager::getControllerType(int index)
{
	XINPUT_CAPABILITIES info;
	XInputGetCapabilities(index, NULL, &info);

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

XinputDevice* XinputManager::getController(int index)
{
	return controllers[index];
}

