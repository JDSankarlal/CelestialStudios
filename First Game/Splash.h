#pragma once
#include <GameEmGine.h>
#include <functional>
#include "Intro.h"

class Splash:public Scene
{
public:
	~Splash() {
		delete gray;
	}

	// Set intro screen
	void init()
	{
		//GAME::m_grayScalePost->enable();
		//glUniform1f(GAME::m_grayScalePost->getUniformLocation("uTime"), 0.f);
		//GAME::m_grayScalePost->disable();
		GameEmGine::m_modelShader->sendUniform("darken", 1);

		gray = new Shader;
		grayPost = new FrameBuffer("Gray Scale", 1);
		
		int width = GameEmGine::getWindowWidth(), 
			height = GameEmGine::getWindowHeight();

		grayPost->initDepthTexture(width, height);
		grayPost->initColourTexture(0, width, height, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);

		if(!grayPost->checkFBO())
		{
			puts("FBO failed Creation");
			system("pause");
			return;
		}

		srand(clock());
		gray= ResourceManager::getShader("Shaders/Main Buffer.vtsh", "Shaders/Grayscale.fmsh");
		grayPost->setPostProcess(
			[&]()->void
			{
				gray->enable();
				glUniform1i(gray->getUniformLocation("uTex"), 0);
				glBindTexture(GL_TEXTURE_2D, grayPost->getColorHandle(0));
				GameEmGine::drawFullScreenQuad();
				glBindTexture(GL_TEXTURE_2D, GL_NONE);
				gray->disable();
			});


		mod.push_back(new Model("Models/Scene/Splash/splashScreen.obj"));
		GameEmGine::addModel(mod.back());
		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);
		//mod[0]->addFrameBuffer(grayPost);

		LightSource::setSceneAmbient({0,0,0,255});

		GameEmGine::setCameraType(ORTHOGRAPHIC);
		GameEmGine::setCameraPosition({0,0,-100});

		//engine stuff
		GameEmGine::setFPSLimit(60);
		GameEmGine::setBackgroundColour(0.05f, 0.0f, 0.1f);


	}

	// doing the update for splash screen
	void updateSplash()
	{
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(0, 255, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient >= 250)
			{
				fadein = false;
				fadeout = true;
				splashT = 0;
				splashAmbient = 255;
			}
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,1});
			if(splashAmbient <= 5)
			{
				//fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;


				GameEmGine::removeModel(mod[0]);
				mod.clear();


				//IntroInite();
				GameEmGine::setScene(new Intro);
			}
		}
	}

	//updates within game loop
	void update(double dt)
	{
		dt;
		updateSplash();
	}

private:

	std::vector<Model*> mod;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
	FrameBuffer *grayPost;
	Shader *gray;
};
