#include "WindowCreator.h"



WindowCreator::WindowCreator()
{
	m_monitor = glfwGetPrimaryMonitor();
}

WindowCreator::WindowCreator(std::string name, Size3D size, Coord2D position, int monitor, bool fullScreeen, bool visable)
{
	createWindow(name, size, position, monitor, fullScreeen, visable);
}


WindowCreator::~WindowCreator()
{}

int WindowCreator::createWindow(std::string name, Size3D size, Coord2D position, int monitor, bool fullScreeen, bool visable)
{
	int monCount;
	GLFWmonitor** mons = glfwGetMonitors(&monCount);


	if(monCount > monitor)
		m_monitor = mons[m_info->monitor = monitor];
	else
		m_monitor = glfwGetPrimaryMonitor();

	int tmp;
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetMonitors(&tmp)[m_info->monitor]);

	size.width = float((int)size.width < mode->width ? size.width : mode->width), size.height = float((int)size.height < mode->height ? size.height : mode->height);
	*m_info->size = size;
	*m_info->position = position;


	m_window = glfwCreateWindow((int)m_info->size->width, (int)m_info->size->height, (*(m_info->title) = name).c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_window); //gives opengl the window it renders to

	setFullScreen(fullScreeen);
	setVisable(visable);

	return m_window != nullptr ? WINDOW_CREATED : WINDOW_FAILED;
}

void WindowCreator::setVisable(bool viz)
{
	if(viz)
		glfwShowWindow(m_window);
	else
		glfwHideWindow(m_window);
}

void WindowCreator::setFullScreen(bool full)
{
	if(_full == full)
		return;//Return if the window is fullscreen


	int w, h;
	double x, y;
	if(full)
	{

		glfwGetWindowSize(m_window, &w, &h);
		m_info->size = new Size3D{float(w), float(h)};
		glfwGetWindowPos(m_window, &w, &h);
		m_info->position = new Coord2D{float(w), float(h)};


		glfwGetCursorPos(m_window, &x, &y);

		// get reolution of monitor
		int tmp;
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetMonitors(&tmp)[m_info->monitor]);

		// switch to full screen
		glfwGetFramebufferSize(m_window, &w, &h);
		glViewport(0, 0, w, h);
		glfwSetWindowMonitor(m_window, m_monitor, 0, 0, mode->width, mode->height, 0);

		glfwSetCursorPos(m_window, x, y);
	}
	else
	{
		glfwGetCursorPos(m_window, &x, &y);
		glfwGetFramebufferSize(m_window, &w, &h);
		glViewport(0, 0, w, h);
		glfwSetWindowMonitor(m_window, nullptr, int(m_info->position->x), int(m_info->position->y), int(m_info->size->width), int(m_info->size->height), 0);
		glfwSetCursorPos(m_window, x, y);
		//	m_monitor = nullptr;
	}

	_full = full;
}

GLFWwindow* WindowCreator::getWindow()
{
	return m_window;
}

std::string & WindowCreator::getTitle()
{
	return *m_info->title;
}

Size3D& WindowCreator::getScreenSize()
{
	return *m_info->size;
}

int WindowCreator::getScreenWidth()
{
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);
	return w;
}

int WindowCreator::getScreenHeight()
{
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);
	return h;
}
