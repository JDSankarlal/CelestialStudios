#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_access.hpp>

template <typename T>
inline T bezier(const T &p0, const T &t1, const T &t2, const T &p3, const float t)
{
	return  t * t*t*(-p0 + 3.0f * t1 - 3.0f * t2 + p3) +
		t * t*(3.0f * p0 - 6.0f * t1 + 3.0f * t2) +
		t * (-3.0f * p0 + 3.0f * t1) +
		p0;
}

template<typename T>
inline T lerp(const T &v0, const T &v1, const float& t)
{
	return (T)((1.0f - t) * v0 + (t)* v1);
}

inline int vectorWrap(int num, int mod)
{
	return (num + mod) % mod;
}

inline glm::vec3 catmullUMP(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t)
{
	float t2 = t * t;  // t^2
	float t3 = t2 * t; // t^3
	glm::vec4 U(t3, t2, t, 1.0f);

	// Note: you should store the M matrix somewhere so you
	// do not need to re-create it every time this function is called
	glm::mat4 M;
	M = glm::row(M, 0, glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f));
	M = glm::row(M, 1, glm::vec4(2.0f, -5.0f, 4.0f, -1.0f));
	M = glm::row(M, 2, glm::vec4(-1.0f, 0.0f, 1.0f, 0.0f));
	M = glm::row(M, 3, glm::vec4(0.0f, 2.0f, 0.0f, 0.0f));

	glm::mat4 P;
	P = glm::row(P, 0, glm::vec4(p0, 0.0f));
	P = glm::row(P, 1, glm::vec4(p1, 0.0f));
	P = glm::row(P, 2, glm::vec4(p2, 0.0f));
	P = glm::row(P, 3, glm::vec4(p3, 0.0f));

	return 0.5f * U * M * P;
}

inline glm::vec3 bezierUMP(glm::vec3 p0, glm::vec3 p1, glm::vec3 t0, glm::vec3 t1, float t)
{
	glm::vec4 U(t * t * t, t * t, t, 1.0f);

	glm::mat4 M;
	M = glm::row(M, 0, glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f));
	M = glm::row(M, 1, glm::vec4(3.0f, -6.0f, 3.0f, 0.0f));
	M = glm::row(M, 2, glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f));
	M = glm::row(M, 3, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));


	glm::mat4 P;
	P = glm::row(P, 0, glm::vec4(p0, 0.0f));
	P = glm::row(P, 1, glm::vec4(t0, 0.0f));
	P = glm::row(P, 2, glm::vec4(t1, 0.0f));
	P = glm::row(P, 3, glm::vec4(p1, 0.0f));
	return U * M * P;
}

template <typename T>
inline T catmull(T p0, T p1, T p2, T p3, float t)
{
	return 0.5f *
		(t * t * t * (-p0 +  p1* 3.0f +  p2*-3.0f + p3) +
			t * t * ( p0 *2.0f +  p1*-5.0f +  p2 * 4.0f - p3) +
			t * (-p0 + p2) +
			(2.0F * p1));

	//return p0;
}