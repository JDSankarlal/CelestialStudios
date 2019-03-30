#pragma once
#include <cstdio>
class fileIO
{
public:
	//No longer needed but good resource for how to read in image files in C binary
	static int readFileToBuffer(const char* path,unsigned char * buffer);
};

