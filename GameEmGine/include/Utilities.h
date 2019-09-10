#pragma once
//#define clamp(val, min, max) (val > max ? max : val < min ? min : val)
#include <GL/glew.h>
#include <glm/common.hpp>
#include <string>

#define reclass(a_class,a_val) (*(a_class*)&(a_val))
//#define unsigned int unsigned int
//#define ushort unsigned short

inline char* cDir(char* dir)
{
	char* tmp;
	if(strlen(dir) > 0)
		while(bool(tmp = strchr(dir, '\\')))
		{
			tmp[0] = '/';
		}

	if(strlen(dir) > 1)
		while(bool(tmp = strstr(dir, "//")))
		{
			memmove_s(tmp, strlen(tmp), tmp + 1, strlen(tmp + 1));
			dir[strlen(dir) - 1] = 0;
		}

	return dir;
}

template<class T = float>
struct Coord2D
{
	union
	{
		struct { T x, y; };
		struct { T u, v; };
		struct { T width, height; };
	};
	glm::vec2 toVec2()
	{
		return glm::vec2(x, y);
	}
	Coord2D() = default;

	Coord2D(T a_x, T a_y)
	{
		x = a_x, y = a_y;
	}
	T distance()
	{
		return sqrtf(x * x + y * y);
	}

	static T distance(Coord2D v1, Coord2D v2)
	{
		Coord2D v3 = v2 - v1;
		return sqrtf(v3.x * v3.x + v3.y * v3.y);
	}

	Coord2D normal()
	{
		return *this / distance();
	}

	T& operator[](int m_index)
	{
		T* error = nullptr;
		switch(m_index)
		{
		case 0:
			return static_cast<T&>(x);
		case 1:
			return static_cast<T&>(y);
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
	Coord2D operator/(T coord)
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
	void operator/=(T coord)
	{
		x /= coord, y /= coord;
	}
};

template<class T = float>
struct Coord3D
{
	union
	{
		struct { T x, y, z; };
		struct { T width, height, depth; };
	};


	Coord3D():x(0), y(0), z(0) {};

	Coord3D(Coord2D<T> coord)
	{
		x = coord.x;
		y = coord.y;
		z = 0;
	}

	void operator=(Coord2D<T> coord)
	{
		x = coord.x;
		y = coord.y;
	}

	Coord3D(Coord2D<T> coord, T last)
	{
		x = coord.x;
		y = coord.y;
		z = last;
	}

	Coord3D(T scale)
	{
		this->x = scale;
		this->y = scale;
		this->z = scale;
	}

	Coord3D(T m_x, T m_y, T m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	Coord3D(T m_x, T m_y)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = 0;
	}

	glm::vec3 toVec3()
	{
		return {x,y,z};
	}

	static glm::vec3 toVec3(Coord3D<float> a0)
	{
		return reclass(glm::vec3, a0);
	}

	static glm::vec3 toVec3(Coord3D<int> a0)
	{
		return reclass(glm::vec3, a0);
	}

	static T distance(Coord3D<T> v1, Coord3D<T> v2)
	{
		v1 -= v2;
		return v1.distance();
	}

	T distance()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	T distanceSquare()
	{
		return (x * x + y * y + z * z);
	}

	static T dotProduct(Coord3D<T> a, Coord3D<T> b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Coord3D<T> crossProduct(Coord3D<T> a, Coord3D<T> b)
	{
		return
		{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
	friend static Coord3D<T> abs(Coord3D<T> val)
	{
		return {sqrtf(val.x * val.x),sqrtf(val.y * val.y),sqrtf(val.z * val.z)};
	}


	Coord3D<T> normal()
	{
		return *this / distance();
	}


	void set(Coord2D<T> coord)
	{
		x = coord.x;
		y = coord.y;
	}

	void set(T m_x, T m_y, T m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	void set(T m_x, T m_y)
	{
		this->x = m_x;
		this->y = m_y;
	}

	void normalize()
	{
		T norm = sqrtf(x * x + y * y + z * z);
		x /= norm;
		y /= norm;
		z /= norm;
	}

	T& operator[] (int m_index)
	{
		T* error = nullptr;
		switch(m_index)
		{
		case 0:
			return const_cast<T&>(x);
		case 1:
			return const_cast<T&>(y);
		case 2:
			return const_cast<T&>(z);
		}
		return *error;
	}

	Coord3D<T> operator+(Coord3D<T> coord)
	{
		return {T(x + coord.x), T(y + coord.y), T(z + coord.z)};
	}

	Coord3D<T> operator-(Coord3D<T> coord)
	{
		return {T(x - coord.x), T(y - coord.y), T(z - coord.z)};
	}

	friend Coord3D<T> operator*(T scaler, Coord3D<T> coord)
	{
		return {scaler * coord.x, scaler * coord.y, scaler * coord.z};
	}

	Coord3D<T> operator*(Coord3D<T> coord)
	{
		return {x * coord.x, y * coord.y, z * coord.z};
	}

	Coord3D<T> operator*(T coord)
	{
		return {x * coord, y * coord, z * coord};
	}

	Coord3D<T> operator/(Coord3D<T> coord)
	{
		return {x / coord.x,y / coord.y,z / coord.z};
	}

	Coord3D<T> operator/(T coord)
	{
		return {x / coord,y / coord,z / coord};
	}

	Coord3D<T>& operator-() {
		static Coord3D<T> tmp;
		tmp = *this * -1;
		return tmp;
	}

	void operator-=(Coord3D<T> coord)
	{
		x -= coord.x;
		y -= coord.y;
		z -= coord.z;
	}

	void operator+=(Coord3D<T> coord)
	{
		x += coord.x;
		y += coord.y;
		z += coord.z;
	}

	void operator*=(Coord3D<T> coord)
	{
		x *= coord.x;
		y *= coord.y;
		z *= coord.z;
	}

	void operator*=(T coord)
	{
		x *= coord;
		y *= coord;
		z *= coord;
	}

	void operator/=(Coord3D<T> coord)
	{
		x /= coord.x;
		y /= coord.y;
		z /= coord.z;
	}

	void operator/=(T coord)
	{
		x /= coord;
		y /= coord;
		z /= coord;
	}

	bool operator==(Coord3D<T> coord)const
	{
		return
			x == coord.x &&
			y == coord.y &&
			z == coord.z;
	}

	bool operator!=(Coord3D<T> coord)const
	{
		return !(*this == coord);
	}

	bool operator>(Coord3D<T> coord)
	{
		return this->distanceSquare() > coord.distanceSquare();
	}

	bool operator<=(Coord3D<T> coord)
	{
		return !(*this > coord);
	}
	bool operator<(Coord3D<T> coord)
	{
		return this->distanceSquare() < coord.distanceSquare();
	}
	bool operator>=(Coord3D<T> coord)
	{
		return !(*this < coord);
	}
};

struct vboInfo3D
{
	Coord3D<> pos, size;
};

struct ColourRGBA
{
	GLubyte r, g, b, a;

	ColourRGBA():r((GLubyte)255), g((GLubyte)255), b((GLubyte)255), a((GLubyte)255)
	{}

	ColourRGBA(GLubyte a_r, GLubyte a_g, GLubyte a_b, GLubyte a_a = (GLubyte)255):r(a_r), g(a_g), b(a_b), a(a_a)
	{}

	void set(ColourRGBA rgba)
	{
		set(
			rgba.r,
			rgba.g,
			rgba.b,
			rgba.a);
	}

	void set(GLubyte a_r, GLubyte a_g, GLubyte a_b)
	{
		this[0][0] = a_r;
		this[0][1] = a_g;
		this[0][2] = a_b;
	}

	void set(GLubyte a_r, GLubyte a_g, GLubyte a_b, GLubyte a_a)
	{
		this[0][0] = a_r;
		this[0][1] = a_g;
		this[0][2] = a_b;
		this[0][3] = a_a;
	}

	void set(float a_r, float a_g, float a_b)
	{
		this[0][0] = GLubyte(a_r * 255);
		this[0][1] = GLubyte(a_g * 255);
		this[0][2] = GLubyte(a_b * 255);
	}

	void set(float a_r, float a_g, float a_b, float a_a)
	{
		this[0][0] = GLubyte(a_r * 255);
		this[0][1] = GLubyte(a_g * 255);
		this[0][2] = GLubyte(a_b * 255);
		this[0][3] = GLubyte(a_a * 255);
	}

	ColourRGBA operator+(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(r + rgba[0]),
			GLubyte(g + rgba[1]),
			GLubyte(b + rgba[2]),
			GLubyte(a + rgba[3])};
	}

	ColourRGBA operator*(float rgba)
	{
		return ColourRGBA{
			GLubyte(r * rgba),
			GLubyte(g * rgba),
			GLubyte(b * rgba),
			GLubyte(a * rgba)};
	}

	ColourRGBA operator/(float rgba)
	{
		return ColourRGBA{
			GLubyte(r / rgba),
			GLubyte(g / rgba),
			GLubyte(b / rgba),
			GLubyte(a / rgba)};
	}

	friend ColourRGBA operator*(float rgba, ColourRGBA colour)
	{
		return ColourRGBA{
			GLubyte(colour[0] * rgba),
			GLubyte(colour[1] * rgba),
			GLubyte(colour[2] * rgba),
			GLubyte(colour[3] * rgba)};
	}
	friend ColourRGBA operator/(float rgba, ColourRGBA colour)
	{
		return ColourRGBA{
			GLubyte(colour[0] / rgba),
			GLubyte(colour[1] / rgba),
			GLubyte(colour[2] / rgba),
			GLubyte(colour[3] / rgba)};
	}
	ColourRGBA operator*(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(r * rgba.r / 255.f),
			GLubyte(g * rgba.g / 255.f),
			GLubyte(b * rgba.b / 255.f),
			GLubyte(a * rgba.a / 255.f)};
	}

	ColourRGBA operator/(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(r / (float)rgba.r / 255.f),
			GLubyte(g / (float)rgba.g / 255.f),
			GLubyte(b / (float)rgba.b / 255.f),
			GLubyte(a / (float)rgba.a / 255.f)};
	}

	void operator*=(ColourRGBA rgba)
	{
		*this = *this * rgba;
	}

	void operator/=(ColourRGBA rgba)
	{
		*this = *this / rgba;
	}

	GLubyte& operator[](int index)
	{
		GLubyte* error = nullptr;
		switch(index)
		{
		case 0:
			return static_cast<GLubyte&>(r);
		case 1:
			return static_cast<GLubyte&>(g);
		case 2:
			return static_cast<GLubyte&>(b);
		case 3:
			return static_cast<GLubyte&>(a);
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
		float* error = nullptr;
		return *error;
	}
};

struct VboInfo2D
{
	VboInfo2D(Coord2D<> c = {0,0}, Coord2D<> s = {0,0})
	{
		position = c;
		size = s;
	}

	Coord2D<> position, size;
protected:
	float _angle;
};

struct Vertex2D
{
	Coord2D<> coord;
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
		colour.r = r;
		colour.g = g;
		colour.b = b;
		colour.a = a;
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
		printf("Colour : (%d, %d, %d, %d)\n", colour.r, colour.g, colour.b, colour.a);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct Vertex3D
{
	Coord3D<> coord, norm;
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
		colour.r = r;
		colour.g = g;
		colour.b = b;
		colour.a = a;
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
		printf("Colour : (%d, %d, %d, %d)\n", colour.r, colour.g, colour.b, colour.a);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct WindowInfo
{
	std::string title;
	Coord3D<int> position, size;
	int monitor;
	void print()
	{
		printf("Title    : %s\n\n", title.c_str());
		printf("Position : (%d, %d)\n", position.x, position.y);
		printf("Size     : (%d, %d, %d)\n", size.width, size.height, size.depth);
		printf("Monitor  : %d\n\n", monitor);
	}
};