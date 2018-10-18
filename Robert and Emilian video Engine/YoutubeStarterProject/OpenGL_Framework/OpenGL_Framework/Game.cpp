#include "Game.h"
#include "Utilities.h"

Game::Game()
	: MainBuffer(1), WorkBuffer1(1), WorkBuffer2(1)
{

}

Game::~Game()
{
	delete updateTimer;

	StaticGeometry.UnLoad();
	Monkey.Unload();
	GrassTexture.Unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);

	if (!StaticGeometry.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/Phong.frag"))
	{
		std::cout << "Shaders failed to initialize. \n";
		system("pause");
		exit(0);
	}

	if (!BloomHighPass.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/Bloom/BloomHighPass.frag"))
	{
		std::cout << "Shaders failed to initialize. \n";
		system("pause");
		exit(0);
	}

	if (!BlurHorizontal.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/Bloom/BlurHorizontal.frag"))
	{
		std::cout << "Shaders failed to initialize. \n";
		system("pause");
		exit(0);
	}

	if (!BlurVertical.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/Bloom/BlurVertical.frag"))
	{
		std::cout << "Shaders failed to initialize. \n";
		system("pause");
		exit(0);
	}

	if (!BloomComposite.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/Bloom/BloomComposite.frag"))
	{
		std::cout << "Shaders failed to initialize. \n";
		system("pause");
		exit(0);
	}

	if (!Monkey.LoadFromFile("./Assets/Models/Monkey.obj"))
	{
		std::cout << "Model failed to load. \n";
		system("pause");
		exit(0);
	}

	if (!GrassTexture.Load("./Assets/Textures/Grass.png"))
	{
		system("Pause");
		exit(0);
	}

	MainBuffer.InitDepthTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	MainBuffer.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	//MainBuffer.InitColorTexture(1, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!MainBuffer.CheckFBO())
	{
		std::cout << "FBO failed to initialize. \n";
		system("Pause");
		exit(0);
	}

	WorkBuffer1.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer1.CheckFBO())
	{
		std::cout << "FBO failed to initialize. \n";
		system("Pause");
		exit(0);
	}

	WorkBuffer2.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer2.CheckFBO())
	{
		std::cout << "FBO failed to initialize. \n";
		system("Pause");
		exit(0);
	}

	CameraTransform.Translate(vec3(0.0f, 0.0f, 9.0f));
	CameraProjection = mat4::PerspectiveProjection(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	
	MonkeyTransform.RotateY(deltaTime * 45.0f);
}

void Game::draw()
{
	/// Clear Buffers ///
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MainBuffer.Clear();
	WorkBuffer1.Clear();
	WorkBuffer2.Clear();


	/// Render the Scene ///
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	StaticGeometry.Bind();
	//glEnable(GL_BLEND);

	StaticGeometry.SendUniformMat4("uModel", MonkeyTransform.data, true);
	StaticGeometry.SendUniformMat4("uView", CameraTransform.GetInverse().data, true);
	StaticGeometry.SendUniformMat4("uProj", CameraProjection.data, true);
	StaticGeometry.SendUniform("uTex", 0);
	StaticGeometry.SendUniform("LightPosition", CameraTransform.GetInverse() * vec4(4.0f, 0.0f, 0.0f, 1.0f));
	StaticGeometry.SendUniform("LightAmbient", vec3(0.2f, 0.2f, 0.2f));
	StaticGeometry.SendUniform("LightDiffuse", vec3(1.0f, 0.5f, 0.5f));
	StaticGeometry.SendUniform("LightSpecular", vec3(0.8f, 0.2f, 0.2f));
	StaticGeometry.SendUniform("LightSpecularExponent", 50.0f);
	StaticGeometry.SendUniform("Attenuation_Constant", 1.0f);
	StaticGeometry.SendUniform("Attenuation_Linear", 0.1f);
	StaticGeometry.SendUniform("Attenuation_Quadratic", 0.01f);

	GrassTexture.Bind();
	MainBuffer.Bind();

	glBindVertexArray(Monkey.VAO);
	glDrawArrays(GL_TRIANGLES, 0, Monkey.GetNumVertices());
	glBindVertexArray(0);

	MainBuffer.UnBind();
	GrassTexture.UnBind();
	StaticGeometry.UnBind();

	/// Compute High Pass ///
	glViewport(0, 0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE);

	BloomHighPass.Bind();
	BloomHighPass.SendUniform("uTex", 0);
	BloomHighPass.SendUniform("uThreshold", BLOOM_THRESHOLD);

	WorkBuffer1.Bind();

	glBindTexture(GL_TEXTURE_2D, MainBuffer.GetColorHandle(0));
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	WorkBuffer1.UnBind();
	BloomHighPass.UnBind();

	/// COMPUTE BLUR ///
	glViewport(0, 0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE);
	for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
	{
		// Horizontal
		BlurHorizontal.Bind();
		BlurHorizontal.SendUniform("uTex", 0);
		BlurHorizontal.SendUniform("uPixelSize", 1.0f / WINDOW_WIDTH);

		WorkBuffer2.Bind();

		glBindTexture(GL_TEXTURE_2D, WorkBuffer1.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		WorkBuffer2.UnBind();
		BlurHorizontal.UnBind();

		//Vertical
		BlurVertical.Bind();
		BlurVertical.SendUniform("uTex", 0);
		BlurVertical.SendUniform("uPixelSize", 1.0f / WINDOW_HEIGHT);

		WorkBuffer1.Bind();

		glBindTexture(GL_TEXTURE_2D, WorkBuffer2.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		WorkBuffer1.UnBind();
		BlurVertical.UnBind();
	}

	/// Composite to back buffer ///
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	BloomComposite.Bind();
	BloomComposite.SendUniform("uScene", 0);
	BloomComposite.SendUniform("uBloom", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, MainBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, WorkBuffer1.GetColorHandle(0));
		DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	BloomComposite.UnBind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}

}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		//Switch between cameras here
		//if orthographic make perspective and vice versa
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}
