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
	WindowCreator(std::string name, Size3D, Coord2D = {}, int monitor = 0, bool fullScreeen = false, bool visable = true);
	~WindowCreator();

	int	createWindow(std::string name, Size3D, Coord2D = {}, int monitor = 0, bool fullScreeen = false, bool visable = true);

	void setVisable(bool);

	void setFullScreen(bool);

	GLFWwindow* getWindow();

	std::string& getTitle();
	Size3D& getScreenSize();
	int getScreenWidth();
	int getScreenHeight();

private:
	GLFWwindow * m_window;
	GLFWmonitor* m_monitor;
	WindowInfo *m_info=new WindowInfo;
	bool _full;		

};

