#include "EmGineAudioPlayer.h"
#include "GameEmGine.h"

#pragma region Static Variables
void(*GameEmGine::m_compileShaders)();
//std::function<void()>GameEmGine::m_render;
std::function<void(double)>GameEmGine::m_gameLoop;
Camera* GameEmGine::m_mainCamera;
std::vector<Camera*>GameEmGine::m_cameras;

Shader
* GameEmGine::m_modelShader, * GameEmGine::m_postProcess,
* GameEmGine::m_forwardRender, * GameEmGine::m_grayScalePost,
* GameEmGine::m_bloomHighPass, * GameEmGine::m_blurHorizontal,
* GameEmGine::m_blurVertical, * GameEmGine::m_blurrComposite,
* GameEmGine::m_sobel, * GameEmGine::m_shadows;

FrameBuffer*
GameEmGine::m_mainFrameBuffer, * GameEmGine::m_postBuffer,
* GameEmGine::m_buffer1, * GameEmGine::m_buffer2,
* GameEmGine::m_greyscaleBuffer, * GameEmGine::m_outline,
* GameEmGine::m_shadowBuffer;


WindowCreator* GameEmGine::m_window;	//must be init in the constructor
ColourRGBA GameEmGine::m_colour{123,123,123};

std::unordered_map<std::string, FrameBuffer*> GameEmGine::m_frameBuffers;
std::map<void*, Model*> GameEmGine::m_models;

bool GameEmGine::exitGame = false;
float GameEmGine::m_fps;
short GameEmGine::m_fpsLimit;
Scene* GameEmGine::m_mainScene;
std::vector<Text*> GameEmGine::m_text;
//GLuint GameEmGine::colorCustom;
//int GameEmGine::LUTsize = 0;

bool GameEmGine::lutActive = false;
bool GameEmGine::toonActive = false;

#define SCREEN_RATIO 2
#pragma endregion

void GLAPIENTRY
GameEmGine::MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar * message,
	const void* userParam)
{
	source, id, length, userParam;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

Texture3D tmpLUT;
Texture2D tmpRamp;
std::string LUTpath;

void GameEmGine::init(std::string name, int width, int height, int x, int y, int monitor, bool fullScreen, bool visable)
{
	createNewWindow(name, width, height, x, y, monitor, fullScreen, visable);
	AudioPlayer::init();
	InputManager::init();

	//LUTpath = "Texture/IWLTBAP_Aspen_-_Standard.cube";
	///////////////////////////////////////Bind Custom 3D Texture////////////////////////////////////////////
	//
	//tmpLUT = ResourceManager::getTexture3D(LUTpath.c_str());
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////Bind Custom 2D Texture////////////////////////////////////////////
	//
	//tmpRamp = ResourceManager::getTexture2D("Texture/pinkRamp.png");
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

void GameEmGine::createNewWindow(std::string name, int width, int height, int x, int y, int monitor, bool fullScreen, bool visable)
{
	glfwInit();


	printf("Creating The Window...\n");

	m_window = new WindowCreator(name, {width,height,500}, Coord2D<int>{x, y}, monitor, fullScreen, visable);

	if(m_window)
		puts("Window Creation Successful\n");
	else
	{
		puts("Window Creation Unsuccessful\n");
		return;
	}

	glfwSetFramebufferSizeCallback(m_window->getWindow(), changeViewport);

	m_mainCamera = new Camera({(float)getWindowWidth(), (float)getWindowHeight(),500});

	shaderInit();

	printf("created the window\n");

	m_mainFrameBuffer = new FrameBuffer(3, "Main Buffer");
	m_shadowBuffer = new FrameBuffer(1, "Shadow Buffer");
	m_outline = new FrameBuffer(1, "Sobel Outline");
	m_postBuffer = new FrameBuffer(1, "Post Process Buffer");
	m_greyscaleBuffer = new FrameBuffer(1, "Greyscale");
	m_buffer1 = new FrameBuffer(1, "Test1");
	m_buffer2 = new FrameBuffer(1, "Test2");

	m_mainFrameBuffer->initDepthTexture(getWindowWidth(), getWindowHeight());
	m_mainFrameBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_mainFrameBuffer->initColourTexture(1, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_mainFrameBuffer->initColourTexture(2, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if(!m_mainFrameBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_shadowBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_shadowBuffer->initDepthTexture(getWindowWidth(), getWindowHeight());
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	while(!glfwWindowShouldClose(m_window->getWindow()) && !exitGame)//update loop
	{
		glClearColor((float)m_colour.r / 255, (float)m_colour.g / 255, (float)m_colour.b / 255, (float)m_colour.a / 255);//BG colour

		InputManager::update();
		update();

		if(true)//fps calculation
		{
			calculateFPS();
			char str[20];
			sprintf_s(str, "fps: %.2f", m_fps);

			glClear(GL_DEPTH_BUFFER_BIT);

			static Text fps;
			OrthoPeramiters ortho{0,(float)getWindowWidth(),(float)getWindowHeight(),0,0,(float)getWindowSize().depth};
			static Camera cam({(float)getWindowSize().width,(float)getWindowSize().height,(float)getWindowSize().depth}
			, ORTHOGRAPHIC, &ortho);
			cam.update();

			fps.setColour(1, 0, 0, .7f);
			fps.setText(str);
			fps.textSize(30);
			fps.translate(0, fps.getHeight(), 0);
			fps.rotate(180, 0, 0);

			static std::map<void*, Model*> tmp;

			tmp[&fps] = (Model*)& fps;
			cam.render(m_forwardRender, tmp, true);

			//glfwSetWindowTitle(m_window->getWindow(), (m_window->getTitle() + "--> " + str).c_str());
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
	InputManager::controllerUpdate();
}

int GameEmGine::controllersConnected()
{
	return InputManager::controllersConnected();
}

bool GameEmGine::isControllerConnected(int m_index)
{
	return InputManager::isControllerConnected(m_index);
}

XinputDevice* GameEmGine::getController(int m_index)
{
	return InputManager::getController(m_index);
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

void GameEmGine::setScene(Scene* scene)
{
	m_models.clear();
	m_frameBuffers.clear();
	m_frameBuffers[m_mainFrameBuffer->getTag()] = m_mainFrameBuffer;
	scene->parent = m_mainScene;//set the parent to the previous scene
	m_mainScene = scene;
	scene->init();
	InputManager::setKeyPressedCallback(scene->keyPressed);
	InputManager::setKeyReleasedCallback(scene->keyReleased);
	InputManager::setKeyAllCallback(scene->keyInput);
	InputManager::mouseButtonPressedCallback(scene->mousePressed);
	InputManager::mouseButtonReleasedCallback(scene->mouseReleased);
	InputManager::mouseButtonAllCallback(scene->mouseInput);

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

Coord3D<int> GameEmGine::getWindowSize()
{
	return m_window->getScreenSize();
}

Camera* GameEmGine::getMainCamera()
{
	return m_mainCamera;
}

bool GameEmGine::mouseCollision(Model* model)
{
	static Model mouse(&primitiveCube({.01f}), "Mouse");

	addModel(&mouse);
	mouse.setColour(0, 1, 0);

	Camera* cam = getMainCamera();
	//	glm::mat4 tmp = glm::inverse(cam->getProjectionMatrix());
	Coord3D<> mPos = {InputManager::getMousePosition(),0};
	glm::vec4 direction((mPos * 2 / (Coord3D<>{(float)getWindowSize().x, (float)getWindowSize().y, (float)getWindowSize().z} -1)).toVec3(), 1);
	direction = {direction.x,-direction.y,1,1};


	direction = glm::inverse((cam->getViewMatrix() * cam->getObjectMatrix()) * cam->getProjectionMatrix()) * direction;
	direction = glm::normalize(direction);

	//position = position * 2 - 1;
	//position /= position.w;

	//position.z = cam->getPosition().z;
	mouse.translate(cam->getPosition());

	return mouse.collision2D(model, reclass(Coord3D<>, direction));
}

void GameEmGine::setCameraType(CAMERA_TYPE type, ProjectionPeramiters* proj )
{
	m_mainCamera->setType(type, proj);
}

void GameEmGine::translateCameraBy(Coord3D<> pos)
{
	m_mainCamera->translateBy(pos);
}

void GameEmGine::setCameraPosition(Coord3D<> pos)
{
	m_mainCamera->translate(pos);
}

void GameEmGine::rotateCameraBy(Coord3D<> direction)
{
	m_mainCamera->rotateBy(direction);
}

void GameEmGine::setCameraRotation(Coord3D<> direction)
{
	m_mainCamera->rotate(direction);
}

void GameEmGine::addModel(Model* model)
{
	m_models[model] = model;
}

void GameEmGine::addText(Text* text)
{
	addModel(reclass(Model*, text));
}

void GameEmGine::removeModel(Model* model)
{
	if(model)
		m_models.erase(model);
}

void GameEmGine::removeText(Text* text)
{
	removeModel(reclass(Model*, text));
}

void GameEmGine::addCamera(Camera* cam)
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
	m_greyscaleBuffer->clear();
	m_postBuffer->clear();
	m_buffer1->clear();
	m_buffer2->clear();

	m_mainCamera->update();

	m_modelShader->enable();
	glUniformMatrix4fv(m_modelShader->getUniformLocation("uView"), 1, GL_FALSE, &((m_mainCamera->getObjectMatrix() * m_mainCamera->getViewMatrix())[0][0]));
	glUniformMatrix4fv(m_modelShader->getUniformLocation("uProj"), 1, GL_FALSE, &(m_mainCamera->getProjectionMatrix()[0][0]));
	m_modelShader->disable();

	m_forwardRender->enable();
	glUniformMatrix4fv(m_forwardRender->getUniformLocation("uView"), 1, GL_FALSE, &((m_mainCamera->getObjectMatrix() * m_mainCamera->getViewMatrix())[0][0]));
	glUniformMatrix4fv(m_forwardRender->getUniformLocation("uProj"), 1, GL_FALSE, &(m_mainCamera->getProjectionMatrix()[0][0]));
	m_forwardRender->disable();


	LightSource::setCamera(m_mainCamera);
	LightSource::setShader(m_postProcess);
	LightSource::update();


	glViewport(0, 0, getWindowWidth(), getWindowHeight());

	m_mainFrameBuffer->enable();
	m_mainCamera->render(m_modelShader, m_models, false);
	m_mainFrameBuffer->disable();



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
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tmpRamp.id);

	m_postProcess->sendUniform("uPos", 0);
	m_postProcess->sendUniform("uNorm", 1);
	m_postProcess->sendUniform("uScene", 2);
	m_postProcess->sendUniform("uRamp", 3);
	m_postProcess->sendUniform("toonActive", toonActive);

	FrameBuffer::drawFullScreenQuad();

	//un-bind textures
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);


	m_postProcess->disable();
	m_postBuffer->disable();


	m_mainFrameBuffer->moveDepthToBuffer(getWindowWidth(), getWindowHeight(), m_postBuffer->getFrameBufferID());
	m_postBuffer->enable();

	LightSource::setShader(m_forwardRender);
	LightSource::update();
	m_mainCamera->render(m_forwardRender, m_models, true);

	m_postBuffer->disable();

	glViewport(0, 0, getWindowWidth() / SCREEN_RATIO, getWindowHeight() / SCREEN_RATIO);

	//binds the initial high pass to buffer 1
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
		m_blurVertical->sendUniform("uTex", 0);
		m_blurVertical->sendUniform("uPixleSize", 1.0f / getWindowWidth());
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


	glClearDepth(1.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//3D look up table being applied
	m_grayScalePost->enable();

	m_grayScalePost->sendUniform("uTex", 0);
	m_grayScalePost->sendUniform("customTexure", 6);
	m_grayScalePost->sendUniform("lutSize", tmpLUT.lutSize);
	m_grayScalePost->sendUniform("lutActive", false);


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


	if(m_gameLoop != nullptr)
		m_gameLoop(glfwGetTime());

	glfwPollEvents();//updates the event handlers

}

void GameEmGine::changeViewport(GLFWwindow*, int w, int h)
{
	w, h;
	//printf("Width : %d\n"
	//	   "Height: %d\n\n", w, h);



	//Framebuffer Resizing 
	m_mainFrameBuffer->resizeDepth(w, h);
	m_mainFrameBuffer->resizeColour(0, w, h);
	m_mainFrameBuffer->resizeColour(1, w, h);
	m_mainFrameBuffer->resizeColour(2, w, h);

	m_postBuffer->resizeColour(0, w, h);
	m_greyscaleBuffer->resizeColour(0, w, h);

	m_buffer1->resizeColour(0, unsigned((float)w / SCREEN_RATIO), unsigned((float)h / SCREEN_RATIO));
	m_buffer2->resizeColour(0, unsigned((float)w / SCREEN_RATIO), unsigned((float)h / SCREEN_RATIO));


	//glFrustum(0, w, 0, h, 0, h);//eye view
	//glOrtho(0, 1, 0, 1, 0, 1);//box view
}