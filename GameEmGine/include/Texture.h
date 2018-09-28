#pragma once
#include <GL\glew.h>
#include <string>
#include <vector>

struct Texture2D
{
	GLuint id;
	int width, height;
	std::string type="",path="";
	
	bool operator==(Texture2D arg)
	{
		return id == arg.id;
	}

	//template <class T> friend bool operator==(vector<T> a,vector<T> b)
	//{
	//	if(a.size() != b.size())
	//		return false;
	//	for(int c = 0; c < a.size(); c++)
	//	{
	//		if(a[c] != b[c])
	//			return false;
	//	}
	//	return true;
	//}
};



struct Texture3D
{
	GLuint id;
	int width, height, depth;
};

