#include "Model.h" 
#include "ResourceManager.h"
#include <ctime>
Model::Model()
{}

Model::Model(Model& model) :
	m_transform(model.m_transform),
	m_mesh(model.m_mesh),
	m_colour(model.m_colour),
	m_transBB(glm::mat4(1))
{
	m_shaderBB.create("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");
	//boundingBoxInit();
}

Model::Model(const char * path) :
	m_transBB(glm::mat4(1))
{
	if(loadModel(path))
	{
		m_shaderBB.create("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");
		m_left = m_mesh.left;
		m_right = m_mesh.right;
		m_top = m_mesh.top;
		m_bottom = m_mesh.bottom;
		m_front = m_mesh.front;
		m_back = m_mesh.back;
		//	boundingBoxInit();
	}
}

Model::~Model()
{}

void Model::render(Shader& shader, Camera& cam)
{
	float colour[4]{ (float)m_colour.colorR / 255,(float)m_colour.colorG / 255,(float)m_colour.colorB / 255,(float)m_colour.colorA / 255 };

	shader.enable();

	if(m_parent)
		glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_parent->m_transform.getTransformation() * m_transform.getTransformation())[0][0]));

	glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_transform.getTransformation())[0][0]));

	glUniform4fv(shader.getUniformLocation("colourMod"), 1, colour);


	//render the mesh
	m_mesh.render(shader);
	shader.disable();

	// update the position of the object
	m_transBB = cam.getCameraMatrix() * (m_transform.getTranslationMatrix());
	boundingBoxUpdate();

	if(m_enableBB)
		drawBoundingBox();

	m_transform.resetUpdated();

	for(auto&a : m_children)
		a->render(shader, cam);
}

void Model::drawBoundingBox()
{

	m_shaderBB.enable();
	float colour[4]{ (float)m_colour.colorR / 255,(float)m_colour.colorG / 255,(float)m_colour.colorB / 255,(float)m_colour.colorA / 255 };

	glUniform4fv(m_shaderBB.getUniformLocation("colourMod"), 1, colour);

	glBindVertexArray(m_BBVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);

	m_shaderBB.disable();
	//shader.enable();

}

Transformer& Model::getTransformer()
{
	return m_transform;
}

void Model::removeChild(Model* child)
{
	auto ref = std::find(m_children.begin(), m_children.end(), child);
	if(ref != m_children.end())
		m_children.erase(ref);
}

void Model::addChild(Model * child)
{
	m_children.push_back(child);
}

void Model::setColour(float r, float g, float b, float a)
{
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b), (GLubyte)(255 * a));
}

void Model::setColour(float r, float g, float b)
{
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b));
}
void Model::setColour(ColourRGBA colour)
{
	m_colour = colour;
}
ColourRGBA Model::getColour() 
{
	return m_colour;
}
bool Model::loadModel(const char * path)
{
	return m_mesh.loadMesh(path);
}

void Model::enableBoundingBox(bool enable)
{
	m_enableBB = enable;
}

float Model::getWidth()
{
	return m_width;
}

float Model::getHeight()
{
	return m_height;
}

float Model::getDepth()
{

	return m_depth;
}

Coord3D Model::getCenter()
{

	return m_center;
}

void Model::boundingBoxUpdate()
{
	m_front = m_back = m_top = m_bottom = m_left = m_right = Coord3D(0, 0, 0);
	std::vector<glm::vec4> thing
	{
	glm::vec4(m_mesh.right.coordX, m_mesh.right.coordY, m_mesh.right.coordZ, 1),
	glm::vec4(m_mesh.left.coordX,   m_mesh.left.coordY,   m_mesh.left.coordZ, 1),
	glm::vec4(m_mesh.top.coordX,   m_mesh.top.coordY,   m_mesh.top.coordZ, 1),
	glm::vec4(m_mesh.bottom.coordX, m_mesh.bottom.coordY, m_mesh.bottom.coordZ, 1),
	glm::vec4(m_mesh.front.coordX, m_mesh.front.coordY, m_mesh.front.coordZ, 1),
	glm::vec4(m_mesh.back.coordX,   m_mesh.back.coordY,   m_mesh.back.coordZ, 1)
	};

	bool first = true;

	if(m_enableBB)
	{
		m_shaderBB.enable();
		glUniformMatrix4fv(m_shaderBB.getUniformLocation("trans"), 1, false, &(m_transBB)[0][0]);
		m_shaderBB.disable();
	}

	for(auto &a : thing)
	{

		a = (m_transform.getRotationMatrix()*m_transform.getScaleMatrix() * a);


		if(first)
		{
			m_front = m_back = m_left = m_right = m_top = m_bottom = Coord3D(a.x, a.y, a.z);
			first = false;
		} else
		{
			m_front = a.z > m_front.coordZ ? Coord3D(a.x, a.y, a.z) : m_front;
			m_back = a.z < m_back.coordZ ? Coord3D(a.x, a.y, a.z) : m_back;
			m_left = a.x < m_left.coordX ? Coord3D(a.x, a.y, a.z) : m_left;
			m_right = a.x > m_right.coordX ? Coord3D(a.x, a.y, a.z) : m_right;
			m_top = a.y > m_top.coordY ? Coord3D(a.x, a.y, a.z) : m_top;
			m_bottom = a.y < m_bottom.coordY ? Coord3D(a.x, a.y, a.z) : m_bottom;
		}
	}
	glm::vec4
		top = m_transBB * glm::vec4(m_top.coordX, m_top.coordY, m_top.coordZ, 1),
		bottom = m_transBB * glm::vec4(m_bottom.coordX, m_bottom.coordY, m_bottom.coordZ, 1),
		left = m_transBB * glm::vec4(m_left.coordX, m_left.coordY, m_left.coordZ, 1),
		right = m_transBB * glm::vec4(m_right.coordX, m_right.coordY, m_right.coordZ, 1),
		front = m_transBB * glm::vec4(m_front.coordX, m_front.coordY, m_front.coordZ, 1),
		back = m_transBB * glm::vec4(m_back.coordX, m_back.coordY, m_back.coordZ, 1);

	m_width = abs(right.x - left.x);
	m_height = abs(top.y - bottom.y);
	m_depth = abs(front.z - back.z);
	m_center = Coord3D((right.x + left.x) / 2, (top.y + bottom.y) / 2, (front.z + back.z) / 2);
	if(m_enableBB)
		boundingBoxInit();
}

void Model::boundingBoxInit()
{
	if(!m_BBVaoID)
		glGenVertexArrays(1, &m_BBVaoID);
	if(!m_BBVboID)
		glGenBuffers(1, &m_BBVboID);

	float
		top = m_top.coordY,
		bottom = m_bottom.coordY,
		left = m_left.coordX,
		right = m_right.coordX,
		front = m_front.coordZ,
		back = m_back.coordZ;

	Vertex3D
		topLeftBack{ { left,top,back } },
		topRightBack{ { right,top,back } },
		topLeftFront{ { left, top,front } },
		topRightFront{ { right,top,front } },
		bottomLeftBack{ { left, bottom,back } },
		bottomRightBack{ { right,bottom,back } },
		bottomLeftFront{ { left, bottom,front } },
		bottomRightFront{ { right,bottom,front } };


	Vertex3D tmp[12 * 3]{
		//top
		topLeftBack,topRightBack,topRightFront,
		topLeftBack,topRightFront,topLeftFront,
		//bottom
		bottomLeftBack,bottomRightBack, bottomRightFront,
		bottomLeftBack,bottomRightFront,bottomLeftFront,
		//front
		topLeftFront,topRightFront,bottomRightFront,
		topLeftFront,bottomRightFront,bottomLeftFront,
		//back
		topLeftBack,  topRightBack,bottomRightBack,
		topLeftBack,bottomRightBack,bottomLeftBack,
		//left
		topLeftBack,topLeftFront,bottomLeftFront,
		topLeftBack,bottomLeftFront,bottomLeftBack,
		//right
		topRightBack,   topRightFront,bottomRightFront,
		topRightBack,bottomRightFront,bottomRightBack
	};

	memcpy_s(m_vertBBDat, sizeof(Vertex3D) * 12 * 3, tmp, sizeof(Vertex3D) * 12 * 3);

	glBindVertexArray(m_BBVaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BBVboID);
	glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(Vertex3D), m_vertBBDat, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//vertex     atributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

	//UV         atributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

	//normal     atributes
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



