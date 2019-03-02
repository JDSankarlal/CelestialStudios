
#include "GameEmGine.h"
#include "EmGineAudioPlayer.h"

#pragma region Static Variables
void(*GameEmGine::m_compileShaders)();
//std::function<void()>GameEmGine::m_render;
std::function<void(double)>GameEmGine::m_gameLoop;
Camera* GameEmGine::m_mainCamera;
//GLuint GameEmGine::fsQuadVAO_ID, GameEmGine::fsQuadVBO_ID;
//GLuint fsQuadVAO_ID, fsQuadVBO_ID;
std::vector<Camera*>GameEmGine::m_cameras;
Shader* GameEmGine::m_modelShader, * GameEmGine::m_grayScalePost, * GameEmGine::m_bloomHighPass, * GameEmGine::m_blurHorizontal,
* GameEmGine::m_blurVertical, * GameEmGine::m_blurrComposite, * GameEmGine::m_deferredRender;
GLuint GameEmGine::m_fsQuadVAO_ID, GameEmGine::m_fsQuadVBO_ID;
InputManager* GameEmGine::m_inputManager;
WindowCreator* GameEmGine::m_window;	//must be init in the constructor
ColourRGBA GameEmGine::m_colour{123,123,123};
//ModelBatch *GameEmGine::m_modelBatch;
FrameBuffer* GameEmGine::m_mainFrameBuffer, * GameEmGine::m_buffer1, * GameEmGine::m_buffer2, * GameEmGine::m_greyscaleBuffer, * GameEmGine::m_deferredRenderBuffer;
std::unordered_map<std::string, FrameBuffer*> GameEmGine::m_frameBuffers;
std::vector<Model*> GameEmGine::m_models;
bool GameEmGine::exitGame = false;
float GameEmGine::m_fps;
short GameEmGine::m_fpsLimit;
Scene* GameEmGine::m_mainScene;

//uniform vec4 LightPosition;
//1uniform vec3 LightAmbient;

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

void GameEmGine::init(std::string name, int width, int height, int x, int y, int monitor, bool fullScreen, bool visable)
{
	createNewWindow(name, width, height, x, y, monitor, fullScreen, visable);
	m_mainFrameBuffer->setPostProcess([&]()->void {
		//main frame buffer render
		m_bloomHighPass->enable();
		glUniform1i(m_bloomHighPass->getUniformLocation("uTex"), 0);
		glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(0));
		drawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		m_bloomHighPass->disable();
		});
}

//GameEmGine::~GameEmGine()
//{
//	glfwTerminate();
//}

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

	m_mainFrameBuffer = new FrameBuffer("Main Buffer", 1);
	m_greyscaleBuffer = new FrameBuffer("Greyscale", 1);
	m_buffer1 = new FrameBuffer("Test1", 1);
	m_buffer2 = new FrameBuffer("Test2", 1);
	m_deferredRenderBuffer = new FrameBuffer("Differed Render", 3);




	m_mainFrameBuffer->initDepthTexture(getWindowWidth(), getWindowHeight());
	m_mainFrameBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);

	if(!m_mainFrameBuffer->checkFBO())
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

	m_buffer1->initColourTexture(0, getWindowWidth() / 4, getWindowHeight() / 4, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!m_buffer1->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_buffer2->initColourTexture(0, getWindowWidth() / 4, getWindowHeight() / 4, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!m_buffer2->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	m_deferredRenderBuffer->initColourTexture(0, getWindowWidth(), getWindowHeight(), GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_deferredRenderBuffer->initColourTexture(1, getWindowWidth(), getWindowHeight(), GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_deferredRenderBuffer->initColourTexture(2, getWindowWidth(), getWindowHeight(), GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	m_deferredRenderBuffer->initDepthTexture(getWindowWidth(), getWindowHeight());
	if(!m_deferredRenderBuffer->checkFBO())
	{
		puts("FBO failed Creation");
		system("pause");
		return;
	}

	initFullScreenQuad();

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

bool GameEmGine::isControllerConnected(int index)
{
	return m_inputManager->isControllerConnected(index);
}

Xinput& GameEmGine::getController(int index)
{
	return m_inputManager->getController(index);
}

WindowCreator* GameEmGine::getWindow()
{
	return m_window;
}

void GameEmGine::shaderInit()
{
	//m_cameraShader = new GLSLCompiler;
	//m_cameraShader->create("Shaders/Texture.vtsh", "Shaders/Texture.fmsh");
	m_modelShader = new Shader;
	m_modelShader->create("Shaders/PassThrough.vert", "Shaders/PassThrough.frag");
	m_deferredRender = new Shader;
	m_deferredRender->create("Shaders/DeferredRender.vtsh", "Shaders/DeferredRender.fmsh");

	m_bloomHighPass = new Shader;
	m_bloomHighPass->create("Shaders/Main Buffer.vtsh", "Shaders/BloomHighPass.fmsh");
	m_blurHorizontal = new Shader;
	m_blurHorizontal->create("Shaders/Main Buffer.vtsh", "Shaders/BlurHorizontal.fmsh");
	m_blurVertical = new Shader;
	m_blurVertical->create("Shaders/Main Buffer.vtsh", "Shaders/BlurVertical.fmsh");
	m_blurrComposite = new Shader;
	m_blurrComposite->create("Shaders/Main Buffer.vtsh", "Shaders/BloomComposite.fmsh");


	m_grayScalePost = new Shader;
	m_grayScalePost->create("Shaders/Main Buffer.vtsh", "Shaders/GrayscalePost.fmsh");
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

void GameEmGine::initFullScreenQuad()
{
	float vboData[] =
	{
		-1.0f,-1.0f,0.0f,
		 1.0f,-1.0f,0.0f,
		-1.0f, 1.0f,0.0f,

		 1.0f, 1.0f,0.0f,
		-1.0f, 1.0f,0.0f,
		 1.0f,-1.0f,0.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,

		1.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f
	};

	if(!m_fsQuadVAO_ID)
		glGenVertexArrays(1, &m_fsQuadVAO_ID);

	glBindVertexArray(m_fsQuadVAO_ID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	if(!m_fsQuadVBO_ID)
		glGenBuffers(1, &m_fsQuadVBO_ID);

	glBindBuffer(GL_ARRAY_BUFFER, m_fsQuadVBO_ID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 + sizeof(float) * 12, vboData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 18));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindVertexArray(GL_NONE);
}

void GameEmGine::drawFullScreenQuad()
{
	glBindVertexArray(m_fsQuadVAO_ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);

}

void GameEmGine::setScene(Scene * scene)
{
	m_models.clear();
	m_frameBuffers.clear();
	m_frameBuffers[m_mainFrameBuffer->getTag()] = m_mainFrameBuffer;
	m_mainScene = scene;
	scene->init();
	m_inputManager->keyPressedCallback(scene->keyPressed);
	m_inputManager->keyReleasedCallback(scene->keyReleased);
	m_inputManager->mouseButtonPressCallback(scene->mousePressed);
	m_inputManager->mouseButtonReleaseCallback(scene->mouseReleased);
	//m_render = scene->render;
	m_gameLoop = [](double a)->void {m_mainScene->update(a); };
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
	for(unsigned a = 0; a < m_models.size(); a++)
		if(m_models[a] == model)
			m_models.erase(m_models.begin() + a);
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
	m_deferredRenderBuffer->clear();
	m_buffer1->clear();
	m_buffer2->clear();


	m_mainCamera->update();


	//if(m_render != nullptr)
	//	m_render();

	LightSource::setCamera(m_mainCamera);
	LightSource::setShader(m_modelShader);
	LightSource::update();

	if(m_mainCamera->getTransformer().isUpdated())
	{
		m_deferredRender->enable();
		glUniformMatrix4fv(m_deferredRender->getUniformLocation("uView"), 1, GL_FALSE, &((m_mainCamera->getViewMatrix())[0][0]));
		glUniformMatrix4fv(m_deferredRender->getUniformLocation("uObject"), 1, GL_FALSE, &((m_mainCamera->getObjectMatrix())[0][0]));
		glUniformMatrix4fv(m_deferredRender->getUniformLocation("uProj"), 1, GL_FALSE, &(m_mainCamera->getProjectionMatrix()[0][0]));
		m_deferredRender->disable();

		m_modelShader->enable();
		glUniformMatrix4fv(m_modelShader->getUniformLocation("uView"), 1, GL_FALSE, &((glm::mat4(1))[0][0]));
		glUniformMatrix4fv(m_modelShader->getUniformLocation("uProj"), 1, GL_FALSE, &(glm::mat4(1)[0][0]));
		m_modelShader->disable();
	}

	
	

	///~ 3D-Graphics 1 ~///
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	m_deferredRenderBuffer->enable();
	m_mainCamera->addModels(&m_models);
	m_mainCamera->render(m_deferredRender);

	m_deferredRenderBuffer->disable();

	//m_frameBuffers["Main Buffer"]->enable();
	m_modelShader->enable();
	glUniformMatrix4fv(m_modelShader->getUniformLocation("uModel"), 1, false, &glm::mat4(1)[0][0]);
	//glUniform1i(m_modelShader->getUniformLocation("textured"), 1);
	//glUniform4f(m_modelShader->getUniformLocation("colourMod"), 1, 1, 1, 1);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_modelShader->getUniformLocation("uPos"), 0);
	glBindTexture(GL_TEXTURE_2D, m_deferredRenderBuffer->getColorHandle(0));

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(m_modelShader->getUniformLocation("uNorm"), 1);
	glBindTexture(GL_TEXTURE_2D, m_deferredRenderBuffer->getColorHandle(1));

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(m_modelShader->getUniformLocation("uAlbedo"), 2);
	glBindTexture(GL_TEXTURE_2D, m_deferredRenderBuffer->getColorHandle(2));

	drawFullScreenQuad();
	
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	m_modelShader->disable();
	//m_frameBuffers["Main Buffer"]->disable();


	//glViewport(0, 0, getWindowWidth() / 4, getWindowHeight() / 4);
	//
	//m_buffer1->enable();
	//m_bloomHighPass->enable();
	//glUniform1i(m_bloomHighPass->getUniformLocation("uTex"), 0);
	//glUniform1f(m_bloomHighPass->getUniformLocation("uThresh"), 0.25f);
	//glBindTexture(GL_TEXTURE_2D, m_frameBuffers["Main Buffer"]->getColorHandle(2));
	//drawFullScreenQuad();
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//m_bloomHighPass->disable();
	//m_buffer1->disable();
	//
	//glViewport(0, 0, getWindowWidth() / 4, getWindowHeight() / 4);
	//for(int a = 0; a < 4; a++)
	//{
	//	m_buffer2->enable();
	//	m_blurHorizontal->enable();
	//	m_blurHorizontal->sendUniform("uTex", 0);
	//	m_blurHorizontal->sendUniform("uPixleSize", 1.0f / getWindowHeight());
	//	glBindTexture(GL_TEXTURE_2D, m_buffer1->getColorHandle(0));
	//	drawFullScreenQuad();
	//
	//	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//	m_blurHorizontal->disable();
	//
	//
	//
	//	m_buffer1->enable();
	//	m_blurVertical->enable();
	//	m_blurHorizontal->sendUniform("uTex", 0);
	//	m_blurHorizontal->sendUniform("uPixleSize", 1.0f / getWindowWidth());
	//	glBindTexture(GL_TEXTURE_2D, m_buffer2->getColorHandle(0));
	//	drawFullScreenQuad();
	//
	//	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//	m_blurVertical->disable();
	//
	//
	//}
	//FrameBuffer::disable();
	//
	//glViewport(0, 0, getWindowWidth(), getWindowHeight());
	//
	//
	//m_greyscaleBuffer->enable();
	//m_blurrComposite->enable();
	//glActiveTexture(GL_TEXTURE0);
	//m_blurrComposite->sendUniform("uScene", 0);
	//glBindTexture(GL_TEXTURE_2D, m_mainFrameBuffer->getColorHandle(0));
	//
	//glActiveTexture(GL_TEXTURE1);
	//m_blurrComposite->sendUniform("uBloom", 1);
	//glBindTexture(GL_TEXTURE_2D, m_buffer1->getColorHandle(0));
	//drawFullScreenQuad();
	//
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//m_blurrComposite->disable();
	//m_greyscaleBuffer->disable();
	//
	//m_grayScalePost->enable();
	//m_grayScalePost->sendUniform("uTex", 0);
	//glBindTexture(GL_TEXTURE_2D, m_greyscaleBuffer->getColorHandle(0));
	//drawFullScreenQuad();
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//m_grayScalePost->disable();

	///~ will never be used but I'll keep it here anyways ~///
	////2D-Graphics 
	//m_spriteBatch->begin();
	//for(int a = 0; a < m_sprites.size(); a++)
	//{
	//	m_spriteBatch->draw(&m_sprites[a]->objectInfo, m_sprites[a]->depth, m_sprites[a]->texture);
	//	if(a + 1 < m_sprites.size())
	//		a++,
	//		m_spriteBatch->draw(&m_sprites[a]->objectInfo, m_sprites[a]->depth, m_sprites[a]->texture);
	//}
	//m_spriteBatch->end();
	//m_spriteBatch->render(*m_cameraShader,*m_mainCamera);

	glViewport(0, 0, getWindowWidth(), getWindowHeight());
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
	m_frameBuffers["Main Buffer"]->initDepthTexture(w, h);
	m_frameBuffers["Main Buffer"]->initColourTexture(w, h, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, 0);

	glViewport(0, 0, w, h);
	//glFrustum(0, w, 0, h, 0, h);//eye view
	//glOrtho(0, 1, 0, 1, 0, 1);//box view
}