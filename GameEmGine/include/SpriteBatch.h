#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Camera.h"
#include "GLSLCompiler.h"
#include "StructInfo.h"
#include "ResourceManager.h"

struct Glyph2D
{
	Glyph2D()
	{}
	Glyph2D(VboInfo2D* info, float depth, GLuint texture, const ColourRGBA * colour) :
		texture(texture), depth(depth)
	{

		int w, h;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);

		bottomLeft.colour = *colour;
		bottomLeft.coord = info->position / Coord2D {(float)w, (float)h};
		bottomLeft.setUV(0, 0);

		topLeft.colour = *colour;
		topLeft.coord = (Coord2D {0.f,info->size.height} +info->position) / Coord2D {(float)w, (float)h};
		topLeft.setUV(0, 1);

		topRight.colour = *colour;
		topRight.coord = (Coord2D {info->size.width,info->size.height}+info->position) / Coord2D {(float)w, (float)h};
		topRight.setUV(1, 1);

		bottomRight.colour = *colour;
		bottomRight.coord = (Coord2D {info->size.width,0.f}+info->position) / Coord2D {(float)w, (float)h};
		bottomRight.setUV(1, 0);
	}

	Vertex2D
		bottomLeft,
		topLeft,
		topRight,
		bottomRight;

	GLuint texture;

	float depth = 0;


};

enum GlyphSort
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	BY_TEXTURE
};

struct RenderBatch2D
{
	GLuint
		offset,
		numVerts,
		texture;
};

struct SpriteInfo
{
	SpriteInfo()
	{}
	SpriteInfo(VboInfo2D info, std::string texture, float depth) :objectInfo(info), texture(ResourceManager::getTexture2D(texture.c_str()).id), depth(depth)
	{}
	VboInfo2D objectInfo;
	GLuint texture;
	float depth = 0;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin(GlyphSort type = BY_TEXTURE);
	void end();

	void draw(VboInfo2D* info, float depth, GLuint texture, const ColourRGBA* colour = new ColourRGBA);
	void render(GLSLCompiler & shader, Camera & cam);

private:
	void createRenderBatches();
	void createVertArrayObject();
	void addGlyph(Glyph2D*);
	void addRenderBatch(RenderBatch2D);
	void sortGlyphs();

	//sorting functions//

	static int frontToBackSort(const void* a, const void *b);
	static int backToFrontSort(const void* a, const void *b);
	static int byTextureSort(const void* a, const void *b);

	GLuint _vboID, _vaoID;
	Glyph2D** _glyphs;
	RenderBatch2D* _renderBatches;
	GlyphSort _sortType;
	unsigned int _numGlyphs, _numRenderBatches;
};

