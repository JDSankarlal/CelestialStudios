#pragma once
#include <windows.h>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "FrameBuffer.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Timer.h"

#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600
#define FRAMES_PER_SECOND		60

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	ShaderProgram PassThrough;
	ShaderProgram GreyScalePost;
	Mesh Monkey;
	Texture GrassTexture;
	Camera camera;
	FrameBuffer MainBuffer;

	mat4 CameraTransform;
	mat4 CameraProjection;
	mat4 MonkeyTransform;

private:
	unsigned int VAO = GL_NONE;
	//unsigned int VBO = GL_NONE;
	unsigned int shaderProgram = GL_NONE;
};

//
class GameObject
{
public:
	// Returns the Model matrix for the GameObject.
	// It will be able to transform the object into world-space.
	mat4 GameObject::GetWorldTransform() const
	{
		mat4 transform(localRotation, localPosition, localScale);
		// This is the base case that stops recursion at the root GameObject.
		if (parentEntity != nullptr)
		{
			// Recursively call the parent's function.
			transform = parentEntity->GetWorldTransform() * transform;
		}

		return transform;
	}

	std::vector<GameObject*> children;
	GameObject* parentEntity = nullptr;
	mat3 localRotation;
	vec3 localPosition;
	vec3 localScale = vec3(1.0f, 1.0f, 1.0f);
};