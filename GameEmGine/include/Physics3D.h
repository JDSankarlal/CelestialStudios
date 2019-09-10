#pragma once
#include "Transformer.h"
#include "Utilities.h"
class Physics3D
{
public:
	Physics3D();
	Physics3D(Transformer*);
	~Physics3D();

	void setTransformer(Transformer*);

	void setGravityMagnitude(float);

	void setGravityDirection(Coord3D<>);

	void update();

private:
	Transformer *transform;
	Coord3D<> direction;
	float magnitude;
};

