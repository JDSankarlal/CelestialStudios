#include "SpriteBatch.h"

SpriteBatch::SpriteBatch():_vboID(0), _vaoID(0)
{
	init();
}

SpriteBatch::~SpriteBatch()
{}

void SpriteBatch::init()
{
	createVertArrayObject();
}

void SpriteBatch::begin(GlyphSort type)
{
	_sortType = type;
	for(int a = 0; a < _numGlyphs; a++)
		delete _glyphs[a];
	delete[] _glyphs;
	delete[] _renderBatches;
	_glyphs = new Glyph2D*[0];
	_renderBatches = new RenderBatch2D[0];

	_numGlyphs = 0;
	_numRenderBatches = 0;
}

void SpriteBatch::end()
{
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(VboInfo2D* info, float depth, GLuint texture, const ColourRGBA * colour)
{
	//new Glyph2D();

	addGlyph(new Glyph2D(info, depth, texture, colour));
	//delete colour;
}

void SpriteBatch::render(GLSLCompiler& shader, Camera3D& cam)
{
	shader.enable();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_vaoID);
	for(int a = 0; a < _numRenderBatches; a++)
	{
		glUniformMatrix4fv(shader.getUniformLocation("camera"), 1, GL_FALSE, &(cam.getCameraMatrix()[0][0]));
		glUniformMatrix4fv(shader.getUniformLocation("object"), 1, GL_FALSE, &(cam.getObjectMatrix()[0][0]));

		glBindTexture(GL_TEXTURE_2D, _renderBatches[a].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[a].offset, _renderBatches[a].numVerts);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader.disable();
}

void SpriteBatch::createRenderBatches()
{
	static Vertex2D* verts;
	delete[] verts;
	verts = new Vertex2D[_numGlyphs * 6];
	if(!_numGlyphs)
		return;

	GLuint	offset = 0,
		cv = 0;

	addRenderBatch({(GLuint) offset,(GLuint) 6,_glyphs[0]->texture});
	verts[cv++] = _glyphs[0]->bottomLeft;
	verts[cv++] = _glyphs[0]->topLeft;
	verts[cv++] = _glyphs[0]->topRight;
	verts[cv++] = _glyphs[0]->topRight;
	verts[cv++] = _glyphs[0]->bottomRight;
	verts[cv++] = _glyphs[0]->bottomLeft;
	offset += 6;

	for(int cg = 1; cg < _numGlyphs; cg++)
	{
		if(_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
			addRenderBatch({(GLuint) offset,(GLuint) 6,_glyphs[cg]->texture});
		else
			_renderBatches[_numRenderBatches - 1].numVerts += 6;

		verts[cv++] = _glyphs[cg]->bottomLeft;
		verts[cv++] = _glyphs[cg]->topLeft;
		verts[cv++] = _glyphs[cg]->topRight;
		verts[cv++] = _glyphs[cg]->topRight;
		verts[cv++] = _glyphs[cg]->bottomRight;
		verts[cv++] = _glyphs[cg]->bottomLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//orphan data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D)*(_numGlyphs * 6), nullptr, GL_STATIC_DRAW);
	//upload data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex2D)*(_numGlyphs * 6), verts);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertArrayObject()
{
	if(!_vaoID)
		glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	if(!_vboID)
		glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	for(int a = 0; a < 3; a++)
		glEnableVertexAttribArray(a);

	//vertex     atributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, coord));
	//ColourRGBA atributes
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, colour));
	//UV         atributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, uv));

	glBindVertexArray(0);//Unbind Vertex Array
}

void SpriteBatch::addGlyph(Glyph2D *add)
{
	//Glyph2D** newer = new Glyph2D*[++_numGlyphs];
	//memcpy(newer, _glyphs, sizeof(Glyph2D*)*(_numGlyphs - 1));
	_glyphs = (Glyph2D**) realloc(_glyphs, sizeof(Glyph2D*)*(++_numGlyphs));
	_glyphs[_numGlyphs - 1] = add;
}

void SpriteBatch::addRenderBatch(RenderBatch2D add)
{
	//RenderBatch* newer = new RenderBatch[++_numRenderBatches];
	//memcpy(newer, _renderBatches, sizeof(RenderBatch)*(_numRenderBatches - 1));
	_renderBatches = (RenderBatch2D*) realloc(_renderBatches, sizeof(RenderBatch2D)*(++_numRenderBatches));
	_renderBatches[_numRenderBatches - 1] = add;
	//_renderBatches = newer;
}

void SpriteBatch::sortGlyphs()
{

	switch(_sortType)
	{
	case NONE:
		break;
	case BACK_TO_FRONT:
		std::qsort(_glyphs, _numGlyphs, sizeof(Glyph2D**), backToFrontSort);
		break;
	case FRONT_TO_BACK:
		std::qsort(_glyphs, _numGlyphs, sizeof(Glyph2D**), frontToBackSort);
		break;
	case BY_TEXTURE:
		std::qsort(_glyphs, _numGlyphs, sizeof(Glyph2D**), byTextureSort);
	}
}

int SpriteBatch::frontToBackSort(const void * a, const void * b)
{
	return ((Glyph2D**) a)[0]->depth - ((Glyph2D**) b)[0]->depth;
}

int SpriteBatch::backToFrontSort(const void * a, const void * b)
{
	return -(((Glyph2D**) a)[0]->depth - ((Glyph2D**) b)[0]->depth);
}

int SpriteBatch::byTextureSort(const void * a, const void * b)
{
	return ((Glyph2D**) a)[0]->texture - ((Glyph2D**) b)[0]->texture;
}
