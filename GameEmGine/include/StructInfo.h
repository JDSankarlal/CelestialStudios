#pragma once
#include <GL/glew.h>
#include <string>
#include <cstdio>

struct Coord2D
{
	float x = 0, y = 0;

	float distance()
	{
		return sqrt(x * x + y * y);
	}

	static float distance(Coord2D v1, Coord2D v2)
	{
		Coord2D v3 = v2 - v1;
		return sqrt(v3.x*v3.x + v3.y*v3.y);
	}

	Coord2D normal()
	{
		return *this / distance();
	}

	float& operator[](int index)
	{
		float *error = nullptr;
		switch (index)
		{
		case 0:
			return static_cast<float&>(x);
		case 1:
			return static_cast<float&>(y);
		}
		return *error;
	}

	Coord2D operator+(Coord2D coord)
	{
		return { x + coord.x, y + coord.y };
	}
	Coord2D operator-(Coord2D coord)
	{
		return { x - coord.x, y - coord.y };
	}
	Coord2D operator/(Coord2D coord)
	{
		return { x / coord.x,y / coord.y };
	}
	Coord2D operator/(float coord)
	{
		return { x / coord,y / coord };
	}
	void operator-=(Coord2D coord)
	{
		x -= coord.x;
		y -= coord.y;
	}
	void operator+=(Coord2D coord)
	{
		x += coord.x;
		y += coord.y;
	}
	void operator/=(Coord2D coord)
	{
		x /= coord.x, y /= coord.y;
	}
	void operator/=(float coord)
	{
		x /= coord, y /= coord;
	}
};

struct Coord3D
{
	float coordX = 0.0f, coordY = 0.0f, coordZ = 0.0f;

	Coord3D()
	{}

	static float distance(Coord3D v1, Coord3D v2)
	{
		Coord3D v3 = v2 - v1;
		return v3.distance();
	}

	float distance()
	{
		return sqrt(coordX * coordX + coordY * coordY + coordZ * coordZ);
	}

	Coord3D normal()
	{
		return *this / distance();
	}

	Coord3D(Coord2D coord)
	{
		coordX = coord.x;
		coordY = coord.y;
	}

	Coord3D(float x, float y, float z)
	{
		this->coordX = x;
		this->coordY = y;
		this->coordZ = z;
	}

	Coord3D(float x, float y)
	{
		this->coordX = x;
		this->coordY = y;
	}

	void set(Coord2D coord)
	{
		coordX = coord.x;
		coordY = coord.y;
	}

	void set(float x, float y, float z)
	{
		this->coordX = x;
		this->coordY = y;
		this->coordZ = z;
	}

	void set(float x, float y)
	{
		this->coordX = x;
		this->coordY = y;
	}

	void normalize()
	{
		float norm = sqrt(coordX * coordX + coordY * coordY + coordZ * coordZ);
		coordX *= norm;
		coordY *= norm;
		coordZ *= norm;
	}

	float& operator[] (int index)
	{
		float* error = nullptr;
		switch (index)
		{
		case 0:
			return const_cast<float&>(coordX);
		case 1:
			return const_cast<float&>(coordY);
		case 2:
			return const_cast<float&>(coordZ);
		}
		return *error;
	}

	Coord3D operator+(Coord3D coord)
	{
		return { coordX + coord.coordX, coordY + coord.coordY, coordZ + coord.coordZ };
	}

	Coord3D operator-(Coord3D coord)
	{
		return { coordX - coord.coordX, coordY - coord.coordY, coordZ + coord.coordZ };
	}

	Coord3D operator*(Coord3D coord)
	{
		return { coordX * coord.coordX, coordY * coord.coordY, coordZ * coord.coordZ };
	}

	Coord3D operator/(Coord3D coord)
	{
		return { coordX / coord.coordX,coordY / coord.coordY,coordZ / coord.coordZ };
	}
	Coord3D operator/(float coord)
	{
		return { coordX / coord,coordY / coord,coordZ / coord };
	}


	void operator-=(Coord3D coord)
	{
		coordX -= coord.coordX;
		coordY -= coord.coordY;
		coordZ -= coord.coordZ;
	}

	void operator+=(Coord3D coord)
	{
		coordX += coord.coordX;
		coordY += coord.coordY;
		coordZ += coord.coordZ;
	}

	void operator*=(Coord3D coord)
	{
		coordX *= coord.coordX;
		coordY *= coord.coordY;
		coordZ *= coord.coordZ;
	}

	void operator*=(float coord)
	{
		coordX *= coord;
		coordY *= coord;
		coordZ *= coord;
	}
};

struct Size2D
{
	float width = 0, height = 0;
	float& operator[](int index)
	{
		float *error = nullptr;
		switch (index)
		{
		case 0:
			return static_cast<float&>(width);
		case 1:
			return static_cast<float&>(height);
		}
		return *error;
	}
};

struct Size3D
{
	float width = 0, height = 0, depth = 0;

	Size3D()
	{}

	Size3D(Size2D size)
	{
		width = size.width;
		height = size.height;
	}

	Size3D(float w, float h, float d)
	{
		this->width = w;
		this->height = h;
		this->depth = d;
	}

	Size3D(float w, float h)
	{
		this->width = w;
		this->height = h;
	}

	void set(Size2D size)
	{
		width = size.width;
		height = size.height;
	}

	void set(float w, float h, float d)
	{
		this->width = w;
		this->height = h;
		this->depth = d;
	}

	void set(float w, float h)
	{
		this->width = w;
		this->height = h;
	}

	float& operator[] (int index)
	{
		float* error = nullptr;
		switch (index)
		{
		case 0:
			return const_cast<float&>(width);
		case 1:
			return const_cast<float&>(height);
		case 2:
			return const_cast<float&>(depth);
		}
		return *error;
	}
};

struct vboInfo3D
{
	Coord3D pos;
	Size3D size;
};

struct ColourRGBA
{
	GLubyte colorR, colorG, colorB, colorA;

	ColourRGBA() :colorR((GLubyte)255), colorG((GLubyte)255), colorB((GLubyte)255), colorA((GLubyte)255)
	{}

	ColourRGBA(GLubyte r, GLubyte g, GLubyte b, GLubyte a = (GLubyte)255) :colorR(r), colorG(g), colorB(b), colorA(a)
	{}

	void set(ColourRGBA rgba)
	{
		set(
			rgba.colorR,
			rgba.colorG,
			rgba.colorB,
			rgba.colorA);
	}

	void set(GLubyte r, GLubyte g, GLubyte b)
	{
		this[0][0] = r;
		this[0][1] = g;
		this[0][2] = b;
	}

	void set(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		this[0][0] = r;
		this[0][1] = g;
		this[0][2] = b;
		this[0][3] = a;
	}

	ColourRGBA operator*(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(colorR * (float)rgba.colorR / 255),
			GLubyte(colorG * (float)rgba.colorG / 255),
			GLubyte(colorB * (float)rgba.colorB / 255),
			GLubyte(colorA * (float)rgba.colorA / 255) };
	}

	void operator*=(ColourRGBA rgba)
	{
		*this = *this * rgba;
	}

	GLubyte& operator[](int index)
	{
		GLubyte *error = nullptr;
		switch (index)
		{
		case 0:
			return static_cast<GLubyte&>(colorR);
		case 1:
			return static_cast<GLubyte&>(colorG);
		case 2:
			return static_cast<GLubyte&>(colorB);
		case 3:
			return static_cast<GLubyte&>(colorA);
		}
		return *error;
	}
};

struct UV
{
	float uv_u = 0, uv_v = 0, uv_w = 0;
	void set(float u, float v, float w)
	{
		this->uv_u = u;
		this->uv_v = v;
		this->uv_w = w;
	}

	void set(float u, float v)
	{
		this->uv_u = u;
		this->uv_v = v;
	}

	float& operator[](int index)
	{
		switch (index)
		{
		case 0:
			return static_cast<float&>(uv_u);
		case 1:
			return static_cast<float&>(uv_v);
		case 2:
			return static_cast<float&>(uv_w);
		}
		float *error = nullptr;
		return *error;
	}
};

struct VboInfo2D
{
	VboInfo2D(Coord2D c = { 0,0 }, Size2D s = { 0,0 })
	{
		position = c;
		size = s;
	}

	Coord2D position;
	Size2D size;
protected:
	float _angle;
};

struct Vertex2D
{
	Coord2D coord;
	ColourRGBA	colour;
	UV uv;

	void setCoord2D(float x, float y)
	{
		coord.x = x;
		coord.y = y;
	}
	//uses the 0-255 representation instead of 0-1
	void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
	{
		colour.colorR = r;
		colour.colorG = g;
		colour.colorB = b;
		colour.colorA = a;
	}
	//sets uv
	void setUV(float u, float v)
	{
		uv.uv_u = u;
		uv.uv_v = v;
	}

	void print()
	{
		printf("Coord2D: (%f, %f)\n", coord.x, coord.y);
		printf("Colour : (%d, %d, %d, %d)\n", colour.colorR, colour.colorG, colour.colorB, colour.colorA);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct Vertex3D
{
	Coord3D coord, norm;
	ColourRGBA	colour;
	UV uv;

	void setCoord(float x, float y, float z)
	{
		coord.coordX = x;
		coord.coordY = y;
		coord.coordZ = z;
	}
	//uses the 0-255 representation instead of 0-1
	void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
	{
		colour.colorR = r;
		colour.colorG = g;
		colour.colorB = b;
		colour.colorA = a;
	}
	//sets uv
	void setUV(float u, float v)
	{
		uv.uv_u = u;
		uv.uv_v = v;
	}

	void setNorm(float x, float y, float z)
	{
		norm.coordX = x;
		norm.coordY = y;
		norm.coordZ = z;
	}

	void print()
	{
		printf("Coord3D: (%f, %f, %f)\n", coord.coordX, coord.coordY, coord.coordZ);
		printf("Colour : (%d, %d, %d, %d)\n", colour.colorR, colour.colorG, colour.colorB, colour.colorA);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct WindowInfo
{
	std::string *title = new std::string;
	Size3D  *size = new Size3D;
	Coord2D *position = new Coord2D;
	int monitor;
	void print()
	{
		printf("Title    : %s\n\n", title->c_str());
		printf("Position : (%f, %f)\n", position->x, position->y);
		printf("Size     : (%.0f, %.0f, %.0f)\n", size->width, size->height, size->depth);
		printf("Monitor  : %d\n\n", monitor);
	}
};