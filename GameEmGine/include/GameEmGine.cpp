#include "GameEmGine.h"
#include "EmGineAudioPlayer.h"

#pragma region Static Variables
void(*GameEmGine::m_compileShaders)();
//std::function<void()>GameEmGine::m_render;
std::function<void(double)>GameEmGine::m_gameLoop;
Camera* GameEmGine::m_mainCamera;
std::vector<Camera*>GameEmGine::m_cameras;

Shader
* GameEmGine::m_modelShader, *GameEmGine::m_postProcess,
*GameEmGine::m_forwardRender, *GameEmGine::m_grayScalePost,
*GameEmGine::m_bloomHighPass, *GameEmGine::m_blurHorizontal,
*GameEmGine::m_blurVertical, *GameEmGine::m_blurrComposite,
*GameEmGine::m_sobel, *GameEmGine::m_shadows;

FrameBuffer*
GameEmGine::m_mainFrameBuffer, *GameEmGine::m_postBuffer,
*GameEmGine::m_buffer1, *GameEmGine::m_buffer2,
*GameEmGine::m_greyscaleBuffer, *GameEmGine::m_outline,
*GameEmGine::m_shadowBuffer;


InputManager* GameEmGine::m_inputManager;
WindowCreator* GameEmGine::m_window;	//must be init in the constructor
ColourRGBA GameEmGine::m_colour{123,123,123};

std::unordered_map<std::string, FrameBuffer*> GameEmGine::m_frameBuffers;
std::vector<Model*> GameEmGine::m_models;

bool GameEmGine::exitGame = false;
float GameEmGine::m_fps;
short GameEmGine::m_fpsLimit;
Scene* GameEmGine::m_mainScene;

//GLuint GameEmGine::colorCustom;
//int GameEmGine::LUTsize = 0;

bool GameEmGine::lutActive = false;

#define SCREEN_RATIO 2
#pragma endregion

void GLAPIENTRY
GameEmGine::MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	source, id, length, userParam;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

Texture3D tmpLUT;
std::string LUTpath;

void GameEmGine::init(std::string name, int width, int height, int x, int y, int monitor, bool fullScreen, bool visable)
{
	createNewWindow(name, width, height, x, y, monitor, fullScreen, visable);

	LUTpath = "Texture/IWLTBAP_Aspen_-_Standard.cube";
	/////////////////////////////////////Bind Custom 3D Texture////////////////////////////////////////////

	tmpLUT = ResourceManager::getTexture3D(LUTpath.c_str());

	////////////////////////////////////////////////////////////////////////////////////////////////////
}

void GameEmGine::createNewWindow(std::string name, int width, int height, int x, int y, int monitor, bool fullScreen, bool visable)
{
	glfwInit();


	printf("Creating The Window...\n");

	m_window = new WindowCreator(name, {(float)width,(float)height}, {(float)x,(float)y}, monitor, fullScreen, visable);

	if(m_window)
		puts("Window Creation Successful\n");
	else
	{
		puts("Window Creation Unsuccessful\n");
		return;
	}

	glfwSetFramebufferSizeCallback(m_window->getWindow(), changeViewport);

	m_inputManager = new InputManager;
	m_mainCamera = new Camera({(float)getWindowWidth(), (float)getWindowHeight(),500});

	shaderInit();

	printf("created the window\n");

	m_mainFrameBuffer = new FrameBuffer("Main Buffer", 3);
	m_shadowBuffer = new FrameBuffer("Shadow Buffer", 1);
	m_outline = new FrameBuffer("Sobel Outline", 1);
	m_postBuffer = new FrameBuffer("Post Process Buffer", 1);
	m_greyscaleBuffer = new FrameBuffer("Greyscale", 1);
	m_buffer1 = new FrameBuffer("Test1", 1);
	m_buffer2 = new FrameBuffer("Test2", 1);

	m_mainFrameBuffer->initDepthTexture(getWindowWidth(), getWindowHeight());
	m_mainFrameBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB16F, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_mainFrameBuffer->initColourTexture(1, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_mainFrameBuffer->initColourTexture(2, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if(!m_mainFrameBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_shadowBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_shadowBuffer->initDepthTexture( getWindowWidth(), getWindowHeight());
	if(!m_shadowBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_outline->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if(!m_outline->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_postBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if(!m_postBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_greyscaleBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!m_greyscaleBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_buffer1->initColourTexture(0, getWindowWidth() / SCREEN_RATIO, getWindowHeight() / SCREEN_RATIO, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!m_buffer1->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}
	m_buffer2->initColourTexture(0, getWindowWidth() / SCREEN_RATIO, getWindowHeight() / SCREEN_RATIO, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);

	if(!m_buffer2->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}
	
	//// During init, enable debug output
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

}

void GameEmGine::run()
{

	//#ifdef _DEBUG
	//	InitOpenGlCallback ();
	//#endif

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	while(!glfwWindowShouldClose(m_window->getWindow()) && !exitGame)//update loop
	{
		glClearColor((float)m_colour.colorR / 255, (float)m_colour.colorG / 255, (float)m_colour.colorB / 255, (float)m_colour.colorA / 255);//BG colour

		InputManager::controllerUpdate();
		update();

		if(true)//fps calculation
		{
			calculateFPS();
			char str[20];
			sprintf_s(str, "fps: %.2f", m_fps);
			glfwSetWindowTitle(m_window->getWindow(), (m_window->getTitle() + "--> " + str).c_str());
		}

		glfwSwapBuffers(m_window->getWindow());
		//glFlush();
		fpsLimiter();
	}
	glfwInit();
	glfwTerminate();
}

void GameEmGine::exit()
{
	exitGame = true;
}

void GameEmGine::setFPSLimit(short limit)
{
	m_fpsLimit = limit;
}

short GameEmGine::getFPSLimit()
{
	return m_fpsLimit;
}

void GameEmGine::vsync(bool enable)
{
	glfwSwapInterval(enable);
}

void GameEmGine::updateControllerConnections()
{
	m_inputManager->controllerUpdate();
}

int GameEmGine::controllersConnected()
{
	return m_inputManager->controllersConnected();
}

bool GameEmGine::isControllerConnected(int m_index)
{
	return m_inputManager->isControllerConnected(m_index);
}

XinputDevice* GameEmGine::getController(int m_index)
{
	return &m_inputManager->getController(m_index);
}

WindowCreator* GameEmGine::getWindow()
{
	return m_window;
}

void GameEmGine::shaderInit()
{
	//m_cameraShader = new GLSLCompiler;
	//m_cameraShader->create("Shaders/Texture.vtsh", "Shaders/Texture.fmsh");
	m_modelShader = ResourceManager::getShader("Shaders/DeferredRender.vtsh", "Shaders/DeferredRender.fmsh");

	m_postProcess = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/PassThrough.frag");
	m_forwardRender = ResourceManager::getShader("Shaders/DeferredRender.vtsh", "Shaders/ForwardRender.fmsh");

	m_bloomHighPass = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/BloomHighPass.fmsh");
	m_blurHorizontal = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/BlurHorizontal.fmsh");
	m_blurVertical = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/BlurVertical.fmsh");
	m_blurrComposite = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/BloomComposite.fmsh");

	m_grayScalePost = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/GrayscalePost.fmsh");
	m_sobel = ResourceManager::getShader("Shaders/Main Buffer.vtsh", "shaders/Sobel.fmsh");

	
}

void GameEmGine::calculateFPS()
{
	static const int SAMPLE = 15;
	static short count;
	static float frameTimes[SAMPLE];

	frameTimes[count++] = 1 / float(glfwGetTime());
	if(count == SAMPLE)
	{
		count = 0;
		m_fps = 0;
		for(auto& a : frameTimes)
			m_fps += a;
		m_fps /= SAMPLE;
	}

	glfwSetTime(0);
}

void GameEmGine::fpsLimiter()
{
	static bool enter = false;
	static clock_t frameStart;


	//way 1: 
	if(enter)
		if(m_fpsLimit > 0)
			while((CLOCKS_PER_SEC / m_fpsLimit) > (clock() - frameStart));

	////way 2: puts the thread to sleep 
	//if(enter)
	//	if(m_fpsLimit > 0)
	//		sleep((CLOCKS_PER_SEC / m_fpsLimit) - (clock() - frameStart));

	frameStart = clock();

	enter = true;
}



void GameEmGine::setScene(Scene * scene)
{
	m_models.clear();
	m_frameBuffers.clear();
	m_frameBuffers[m_mainFrameBuffer->getTag()] = m_mainFrameBuffer;
	scene->parent = m_mainScene;//set the parent to the previous scene
	m_mainScene = scene;
	scene->init();
	m_inputManager->keyPressedCallback(scene->keyPressed);
	m_inputManager->keyReleasedCallback(scene->keyReleased);
	m_inputManager->mouseButtonPressCallback(scene->mousePressed);
	m_inputManager->mouseButtonReleaseCallback(scene->mouseReleased);

	//m_render = scene->render;
	m_gameLoop = [&](double a)->void {m_mainScene->update(a); };
}

void GameEmGine::setBackgroundColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_colour = {GLubyte(r * 255),GLubyte(g * 255),GLubyte(b * 255),GLubyte(a * 255)};
}

int GameEmGine::getWindowWidth()
{
	return m_window->getScreenWidth();
}

int GameEmGine::getWindowHeight()
{
	return m_window->getScreenHeight();
}

Camera* GameEmGine::getMainCamera()
{
	return m_mainCamera;
}

void GameEmGine::setCameraType(CAMERA_TYPE type)
{
	m_mainCamera->setType(type);
}

void GameEmGine::moveCameraPositionBy(Coord3D pos)
{
	m_mainCamera->movePositionBy(pos);
}

void GameEmGine::setCameraPosition(Coord3D pos)
{
	m_mainCamera->setPosition(pos);
}

void GameEmGine::moveCameraAngleBy(float angle, Coord3D direction)
{
	m_mainCamera->moveAngleBy(angle, direction);
}

void GameEmGine::setCameraAngle(float angle, Coord3D direction)
{
	m_mainCamera->setAngle(angle, direction);
}

void GameEmGine::addModel(Model * model)
{
	//m_models = (Model**) realloc(m_models, sizeof(Model*)*++m_numModels);
	//m_models[m_numModels - 1] = model;
	m_models.push_back(model);
	//m_models.back()->addFrameBuffer(m_mainBuffer);
}

void GameEmGine::removeModel(Model * model)
{
	if(model)
		for(unsigned a = 0; a < m_models.size(); a++)
			if(m_models[a] == model)
				m_models.erase(m_models.begin() + a);
}

void GameEmGine::addCamera(Camera * cam)
{
	cam;

	//realloc(m_cameras, sizeof(Camera3D*)*++_numCameras);
	//m_cameras[_numCameras - 1] = cam;
}

void GameEmGine::update()
{

	glClearDepth(1.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_mainFrameBuffer->clear();
	m_postBuffer->clear();
	m_buffer1->clear();
	m_buffer2->clear();


	m_mainCamera->update();


	//if(m_render != nullptr)
	//	m_render();

	if(m_mainCamera->getTransformer().isUpdated())
	{
		m_modelShader->enable();
		glUniformMatrix4fv(m_modelShader->getUniformLocation("uView"), 1, GL_FALSE, &((m_mainCamera->getObjectMatrix() * m_mainCamera->getViewMatrix())[0][0]));
		glUniformMatrix4fv(m_modelShader->getUniformLocation("uProj"), 1, GL_FALSE, &(m_mainCamera->getProjectionMatrix()[0][0]));
		m_modelShader->disable();

		m_forwardRender->enable();
		glUniformMatrix4fv(m_forwardRender->getUniformLocation("uView"), 1, GL_FALSE, &((m_mainCamera->getObjectMatrix() * m_mainCamera->getViewMatrix())[0][0]));
		glUniformMatrix4fv(m_forwardRender->getUniformLocation("uProj"), 1, GL_FALSE, &(m_mainCamera->getProjectionMatrix()[0][0]));
		m_forwardRender->disable();
	}

	LightSource::setCamera(m_mainCamera);
	LightSource::setShader(m_postProcess);
	LightSource::update();


	///~ model sorting ~///
	//std::sort(m_models.begin(), m_models.end(), [&](Model * a, Model * b)->bool
	//	{
	//		return (m_mainCamera->getPosition() - a->getTransformer().getPosition()).distance() >= (m_mainCamera->getPosition() - b->getTransformer().getPosition()).distance();
	//	}
	//);

	glViewport(0, 0, getWindowWidth(), getWindowHeight());

	///~ 3D-Graphics 1 ~///
	std::vector<Model*> transparent;
	m_frameBuffers["Main Buffer"]->enable();
	m_mainCamera->render(m_modelShader, m_models, false);


	m_frameBuffers["Main Buffer"]->disable();

	//store data for post process
	m_postBuffer->enable();
	m_postProcess->enable();

	//bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(2));

	m_postProcess->sendUniform("uPos", 0);
	m_postProcess->sendUniform("uNorm", 1);
	m_postProcess->sendUniform("uScene", 2);

	FrameBuffer::drawFullScreenQuad();

	//un-bind textures
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	m_postProcess->disable();
	m_postBuffer->disable();


	//for(int a = 0; a < (int)LightSource::size(); a++)
	//{
	//	std::vector<FrameBuffer*>shadowBuffer = LightSource::shadowBuffers(getWindowWidth(), getWindowHeight(), m_models, 0);
	//
	//	m_shadowBuffer->enable();
	//	m_postProcess->enable();
	//	for(auto &b : shadowBuffer)
	//	{
	//
	//		//bind textures
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(0));
	//		glActiveTexture(GL_TEXTURE1);
	//		glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(1));
	//		glActiveTexture(GL_TEXTURE2);
	//		glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(2));
	//		glActiveTexture(GL_TEXTURE3);
	//		glBindTexture(GL_TEXTURE_2D, b->getDepthHandle());
	//
	//		m_postProcess->sendUniform("uPos", 0);
	//		m_postProcess->sendUniform("uNorm", 1);
	//		m_postProcess->sendUniform("uScene", 2);
	//		m_postProcess->sendUniform("shadowMap", 3);
	//		glm::mat4 tmpMat = m_mainCamera->getProjectionMatrix() *  glm::lookAt(LightSource::getLightInfo(a).position.toVec3(),
	//			glm::vec3(0.0f, 0.0f, 0.0f),
	//			glm::vec3(0.0f, 1.0f, 0.0f));
	//		m_postProcess->sendUniform("lightSpaceMatrix", tmpMat);
	//
	//		FrameBuffer::drawFullScreenQuad();
	//
	//		//un-bind textures
	//		glActiveTexture(GL_TEXTURE3);
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//		glActiveTexture(GL_TEXTURE2);
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//		glActiveTexture(GL_TEXTURE1);
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//
	//	}
	//	m_postProcess->disable();
	//	m_shadowBuffer->disable();
	//
	//}
	//
	glViewport(0, 0, getWindowWidth() / SCREEN_RATIO, getWindowHeight() / SCREEN_RATIO);

	//binds the initial bloom affect to buffer 1
	m_buffer1->enable();
	m_bloomHighPass->enable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_postBuffer->getColorHandle(0));

	m_bloomHighPass->sendUniform("uTex", 0);
	m_bloomHighPass->sendUniform("uThresh", 0.15f);

	FrameBuffer::drawFullScreenQuad();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	m_bloomHighPass->disable();
	m_buffer1->disable();

	//Takes the high pass and blurs it
	glViewport(0, 0, getWindowWidth() / SCREEN_RATIO, getWindowHeight() / SCREEN_RATIO);
	for(int a = 0; a < SCREEN_RATIO; a++)
	{
		m_buffer2->enable();
		m_blurHorizontal->enable();
		m_blurHorizontal->sendUniform("uTex", 0);
		m_blurHorizontal->sendUniform("uPixleSize", 1.0f / getWindowHeight());
		glBindTexture(GL_TEXTURE_2D, m_buffer1->getColorHandle(0));
		FrameBuffer::drawFullScreenQuad();

		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		m_blurHorizontal->disable();


		m_buffer1->enable();
		m_blurVertical->enable();
		m_blurHorizontal->sendUniform("uTex", 0);
		m_blurHorizontal->sendUniform("uPixleSize", 1.0f / getWindowWidth());
		glBindTexture(GL_TEXTURE_2D, m_buffer2->getColorHandle(0));
		FrameBuffer::drawFullScreenQuad();

		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		m_blurVertical->disable();
	}

	FrameBuffer::disable();//return to base frame buffer

	glViewport(0, 0, getWindowWidth(), getWindowHeight());


	m_greyscaleBuffer->enable();
	m_blurrComposite->enable();
	glActiveTexture(GL_TEXTURE0);
	m_blurrComposite->sendUniform("uScene", 0);
	glBindTexture(GL_TEXTURE_2D, m_postBuffer->getColorHandle(0));

	glActiveTexture(GL_TEXTURE1);
	m_blurrComposite->sendUniform("uBloom", 1);
	glBindTexture(GL_TEXTURE_2D, m_buffer1->getColorHandle(0));
	FrameBuffer::drawFullScreenQuad();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	m_blurrComposite->disable();
	m_greyscaleBuffer->disable();




	//3D look up table being applied
	m_grayScalePost->enable();

	m_grayScalePost->sendUniform("uTex", 0);
	m_grayScalePost->sendUniform("customTexure", 6);
	m_grayScalePost->sendUniform("lutSize", (int)ResourceManager::getTexture3D(LUTpath.c_str()).lutSize);
	m_grayScalePost->sendUniform("lutActive", lutActive);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_greyscaleBuffer->getColorHandle(0));
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_3D, ResourceManager::getTexture3D(LUTpath.c_str()).id);

	FrameBuffer::drawFullScreenQuad();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_3D, GL_NONE);

	m_grayScalePost->disable();

	

	m_mainFrameBuffer->moveDepthToBackBuffer(getWindowWidth(), getWindowHeight());
	LightSource::setShader(m_forwardRender);
	LightSource::update();
	m_mainCamera->render(m_forwardRender, m_models, true);


	//m_greyscaleBuffer->clear();
	//m_greyscaleBuffer->takeFromBackBufferColour(getWindowWidth(), getWindowHeight());

	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	//m_blurrComposite->enable();
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_greyscaleBuffer->getColorHandle(0));//normal map
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getDepthHandle());
	//
	//m_blurrComposite->sendUniform("uScene", 0);
	//m_blurrComposite->sendUniform("uBloom", 1);
	//
	//drawFullScreenQuad();
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//glActiveTexture(GL_TEXTURE6);
	//glBindTexture(GL_TEXTURE_3D, GL_NONE);
	//
	//m_blurrComposite->enable();


	if(m_gameLoop != nullptr)
		m_gameLoop(glfwGetTime());

	glfwPollEvents();//updates the event handlers

}

void GameEmGine::changeViewport(GLFWwindow*, int w, int h)
{

	//printf("Width : %d\n"
	//	   "Height: %d\n\n", w, h);

	//m_window->getScreenWidth(); //just for updating window width & height

	//if(m_mainBuffer)
	//{
	//	m_mainBuffer->initDepthTexture(w, h);
	//	m_mainBuffer->initColourTexture(w, h, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, 0);
	//}


	w, h;
	glfwGetFramebufferSize(m_window->getWindow(), &w, &h);
	m_mainCamera->init({(float)w,(float)h,500.f}, m_mainCamera->getType());

	//m_frameBuffers["Main Buffer"]->resizeDepth(w2, h2);
	//m_frameBuffers["Main Buffer"]->resizeColour(0, w2, h2);
	//
	//m_postBuffer->resizeColour(0, w2, h2);
	//m_greyscaleBuffer->resizeColour(0, w2, h2);
	//
	////m_buffer1->resizeDepth(w / SCREEN_RATIO, h / SCREEN_RATIO);
	//m_buffer1->resizeColour(0, unsigned(w2 / SCREEN_RATIO), unsigned(h2 / SCREEN_RATIO));
	//
	////m_buffer2->resizeDepth(w / SCREEN_RATIO, h / SCREEN_RATIO);
	//m_buffer2->resizeColour(0, unsigned(w2 / SCREEN_RATIO), unsigned(h2 / SCREEN_RATIO));


	//glFrustum(0, w, 0, h, 0, h);//eye view
	//glOrtho(0, 1, 0, 1, 0, 1);//box view
}