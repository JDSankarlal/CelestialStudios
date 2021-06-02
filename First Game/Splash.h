#pragma once
#include <GameEmGine.h>
#include <functional>
#include "Intro.h"



class Splash:public Scene
{
public:
	void onSceneExit() {}

	// Set intro screen
	void init()
	{
		LightManager::enableShadows(false);
		

		//GameEmGine::m_modelShader->sendUniform("darken", false);

		setSkyBox("skyboxes/skybox");
		//enableSkyBox(true);

		//mod.push_back(Model("Models/Scene/Splash/splashScreen.obj"));
		Model tmp(new PrimitivePlane(115, 65), "Splash Scene");
		tmp.replaceTexture(0, 0, ResourceManager::getTexture2D("textures/scene/splash/splashScreen.png"));
		mod.push_back(tmp);
		mod[0].scale(16);
		mod[0].setTransparent(true);
		mod[0].setColour(1, 1, 1, 0);

		GameEmGine::addModel(&mod.back());
		//mod[0]->addFrameBuffer(grayPost);


		LightManager::addLight(&sceneLight);
		sceneLight.setLightType(Light::DIRECTIONAL);
		sceneLight.rotate({-45,0,0});
		sceneLight.setAmbient({0,0,0,255});
		sceneLight.setDiffuse({0,0,0,255});
		sceneLight.setSpecular({0,0,0,255});


		GameEmGine::setCameraType(Camera::ORTHOGRAPHIC);
		GameEmGine::getMainCamera()->reset();

		//engine stuff
		GameEmGine::setFPSLimit(60);
		GameEmGine::setBackgroundColour(0.05f, 0.0f, 0.1f);


	}

	// doing the update for splash screen
	void updateSplash()
	{
		GLubyte maxi = GLubyte(255 * .3f);
		GLubyte mini = 0;
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(mini, maxi, splashT);
			sceneLight.setAmbient({splashAmbient,splashAmbient,splashAmbient,255});
		
			mod[0].setColour(1,1,1,splashT);
			if(splashT >= 1)
			{
				fadein = false;
				fadeout = true;
				splashT = 0;
				splashAmbient = maxi;
			}
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(maxi, mini, splashT);
			sceneLight.setAmbient({splashAmbient,splashAmbient,splashAmbient,255});
		
			mod[0].setColour(1, 1, 1, 1-splashT);
			if(splashT >= 1)
			{
				//fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = mini;

				GameEmGine::removeModel(&mod[0]);
				mod.clear();

				static Intro intro;
				GameEmGine::setScene(&intro);
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

	std::vector<Model> mod;
	Light sceneLight;
	bool fadein = true;
	bool fadeout = false;
	float splashT = 0;
	GLubyte splashAmbient = 0;
	FrameBuffer* grayPost;
	Shader gray;
};
