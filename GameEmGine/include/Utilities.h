#pragma once
#include <GL/glew.h>
#include <glm/common.hpp>
#include <string>
#include <cstdio>

struct Coord2D
{
	float x = 0, y = 0;

	glm::vec2 toVec2()
	{
		return glm::vec2(x, y);
	}
	Coord2D()=default;
	
	Coord2D(float a_x,float a_y)
	{
		x = a_x, y = a_y;
	}
	float distance()
	{
		return sqrtf(x * x + y * y);
	}

	static float distance(Coord2D v1, Coord2D v2)
	{
		Coord2D v3 = v2 - v1;
		return sqrtf(v3.x*v3.x + v3.y*v3.y);
	}

	Coord2D normal()
	{
		return *this / distance();
	}

	float& operator[](int m_index)
	{
		float *error = nullptr;
		switch(m_index)
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
		return {x + coord.x, y + coord.y};
	}
	Coord2D operator-(Coord2D coord)
	{
		return {x - coord.x, y - coord.y};
	}
	Coord2D operator/(Coord2D coord)
	{
		return {x / coord.x,y / coord.y};
	}
	Coord2D operator/(float coord)
	{
		return {x / coord,y / coord};
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
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Coord3D()
	{}

	glm::vec3 toVec3()
	{
		return glm::vec3(x, y, z);
	}
	
	static float distance(Coord3D v1, Coord3D v2)
	{
		v2 = v2 - v1;
		return v2.distance();
	}

	float distance()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	Coord3D normal()
	{
		return *this / distance();
	}

	Coord3D(Coord2D coord)
	{
		x = coord.x;
		y = coord.y;
	}

	Coord3D(float scale)
	{
		this->x = scale;
		this->y = scale;
		this->z = scale;
	}

	Coord3D(float m_x, float m_y, float m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	Coord3D(float m_x, float m_y)
	{
		this->x = m_x;
		this->y = m_y;
	}

	void set(Coord2D coord)
	{
		x = coord.x;
		y = coord.y;
	}

	void set(float m_x, float m_y, float m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	void set(float m_x, float m_y)
	{
		this->x = m_x;
		this->y = m_y;
	}

	void normalize()
	{
		float norm = sqrtf(x * x + y * y + z * z);
		x *= norm;
		y *= norm;
		z *= norm;
	}

	float& operator[] (int m_index)
	{
		float* error = nullptr;
		switch(m_index)
		{
		case 0:
			return const_cast<float&>(x);
		case 1:
			return const_cast<float&>(y);
		case 2:
			return const_cast<float&>(z);
		}
		return *error;
	}

	Coord3D operator+(Coord3D coord)
	{
		return {x + coord.x, y + coord.y, z + coord.z};
	}

	Coord3D operator-(Coord3D coord)
	{
		return {x - coord.x, y - coord.y, z - coord.z};
	}

	friend Coord3D operator*(float scaler, Coord3D coord)
	{
		return {scaler * coord.x, scaler * coord.y, scaler * coord.z};
	}

	Coord3D operator*(Coord3D coord)
	{
		return {x * coord.x, y * coord.y, z * coord.z};
	}

	Coord3D operator*(float coord)
	{
		return {x * coord, y * coord, z * coord};
	}

	Coord3D operator/(Coord3D coord)
	{
		return {x / coord.x,y / coord.y,z / coord.z};
	}

	Coord3D operator/(float coord)
	{
		return {x / coord,y / coord,z / coord};
	}

	Coord3D& operator-() {
		static Coord3D tmp;
		tmp = *this*-1;
		return tmp;
	}

	void operator-=(Coord3D coord)
	{
		x -= coord.x;
		y -= coord.y;
		z -= coord.z;
	}

	void operator+=(Coord3D coord)
	{
		x += coord.x;
		y += coord.y;
		z += coord.z;
	}

	void operator*=(Coord3D coord)
	{
		x *= coord.x;
		y *= coord.y;
		z *= coord.z;
	}

	void operator*=(float coord)
	{
		x *= coord;
		y *= coord;
		z *= coord;
	}
};

struct Size2D
{
	float width = 0, height = 0;
	float& operator[](int m_index)
	{
		float *error = nullptr;
		switch(m_index)
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

	float& operator[] (int m_index)
	{
		float* error = nullptr;
		switch(m_index)
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

	ColourRGBA():colorR((GLubyte)255), colorG((GLubyte)255), colorB((GLubyte)255), colorA((GLubyte)255)
	{}

	ColourRGBA(GLubyte r, GLubyte g, GLubyte b, GLubyte a = (GLubyte)255):colorR(r), colorG(g), colorB(b), colorA(a)
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

	ColourRGBA operator+(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(colorR + rgba[0]),
			GLubyte(colorG + rgba[1]),
			GLubyte(colorB + rgba[2]),
			GLubyte(colorA + rgba[3])};
	}

	ColourRGBA operator*(float rgba)
	{
		return ColourRGBA{
			GLubyte(colorR * rgba),
			GLubyte(colorG * rgba),
			GLubyte(colorB * rgba),
			GLubyte(colorA * rgba)};
	}

	friend ColourRGBA operator*(float rgba, ColourRGBA colour)
	{
		return ColourRGBA{
			GLubyte(colour[0] * rgba),
			GLubyte(colour[1] * rgba),
			GLubyte(colour[2] * rgba),
			GLubyte(colour[3] * rgba)};
	}
	ColourRGBA operator*(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(colorR * (float)rgba.colorR / 255),
			GLubyte(colorG * (float)rgba.colorG / 255),
			GLubyte(colorB * (float)rgba.colorB / 255),
			GLubyte(colorA * (float)rgba.colorA / 255)};
	}

	void operator*=(ColourRGBA rgba)
	{
		*this = *this * rgba;
	}

	GLubyte& operator[](int m_index)
	{
		GLubyte *error = nullptr;
		switch(m_index)
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

	float& operator[](int m_index)
	{
		switch(m_index)
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
	VboInfo2D(Coord2D c = {0,0}, Size2D s = {0,0})
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
		coord.x = x;
		coord.y = y;
		coord.z = z;
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
		norm.x = x;
		norm.y = y;
		norm.z = z;
	}

	void print()
	{
		printf("Coord3D: (%f, %f, %f)\n", coord.x, coord.y, coord.z);
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