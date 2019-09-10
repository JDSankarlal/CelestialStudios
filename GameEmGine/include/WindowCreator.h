#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Utilities.h"

class WindowCreator
{
public:
	enum
	{
		WINDOW_CREATED,
		WINDOW_FAILED
	};

	WindowCreator();
	WindowCreator(std::string name, Coord3D<int>, Coord2D<int> = {}, int monitor = 0, bool fullScreeen = false, bool visable = true);
	~WindowCreator();

	int	createWindow(std::string name, Coord3D<int>, Coord2D<int> = {}, int monitor = 0, bool fullScreeen = false, bool visable = true);

	void setVisable(bool);

	void setFullScreen(bool);

	GLFWwindow* getWindow();

	std::string& getTitle();
	Coord3D<int>& getScreenSize();
	int getScreenWidth();
	int getScreenHeight();

private:
	GLFWwindow * m_window;
	GLFWmonitor* m_monitor;
	WindowInfo *m_info=new WindowInfo;
	bool _full;		

};

