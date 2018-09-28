#pragma once

#include "Camera3D.h"

class Camera2D :protected Camera3D
{
public:
	Camera2D(Size2D = {});
	~Camera2D();

	void init(Size2D);

	void setPosition(Coord3D);
};

