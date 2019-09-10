#include "Model.h" 
#include "ResourceManager.h"
#include <ctime>

Model::Model():Transformer()
{
	m_type = MODEL;
}

Model::Model(Model& model, const char* tag) :
	Transformer(model),
	m_mesh(model.m_mesh),
	m_colour(model.m_colour),
	m_transBB(glm::mat4(1)),
	m_render(model.m_render),
	m_tag(tag),
	m_copy(true)
{
	m_type = MODEL;
	m_shaderBB = ResourceManager::getShader("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");

	float top = m_mesh->top.y,
		bottom = m_mesh->bottom.y,
		left = m_mesh->left.x,
		right = m_mesh->right.x,
		front = m_mesh->front.z,
		back = m_mesh->back.z;

	(m_topLeftBack = {left,top,back}),
		(m_topRightBack = {right,top,back}),
		(m_topLeftFront = {left,top,front}),
		(m_topRightFront = {right,top,front}),
		(m_bottomLeftBack = {left,bottom,back}),
		(m_bottomRightBack = {right,bottom,back}),
		(m_bottomLeftFront = {left,bottom,front}),
		(m_bottomRightFront = {right,bottom,front});


	boundingBoxInit();
	boundingBoxUpdate();

}

Model::Model(primitiveMesh* model, const char* tag):
	Transformer(),
	m_transBB(glm::mat4(1)), m_tag(tag)
{
	m_type = MODEL;
	m_mesh = new Mesh;
	if(m_mesh->loadPrimitive(model))
	{
		m_shaderBB = ResourceManager::getShader("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");


		float top = m_mesh->top.y,
			bottom = m_mesh->bottom.y,
			left = m_mesh->left.x,
			right = m_mesh->right.x,
			front = m_mesh->front.z,
			back = m_mesh->back.z;

		(m_topLeftBack = {left,top,back}),
			(m_topRightBack = {right,top,back}),
			(m_topLeftFront = {left,top,front}),
			(m_topRightFront = {right,top,front}),
			(m_bottomLeftBack = {left,bottom,back}),
			(m_bottomRightBack = {right,bottom,back}),
			(m_bottomLeftFront = {left,bottom,front}),
			(m_bottomRightFront = {right,bottom,front});


		boundingBoxInit();
		boundingBoxUpdate();
	}
}

Model::Model(const char* path, const char* tag):
	Transformer(),
	m_transBB(glm::mat4(1)), m_tag(tag)
{
	m_type = MODEL;

	m_mesh = new Mesh;
	if(loadModel(path))
	{
		m_shaderBB = ResourceManager::getShader("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");


		float top = m_mesh->top.y,
			bottom = m_mesh->bottom.y,
			left = m_mesh->left.x,
			right = m_mesh->right.x,
			front = m_mesh->front.z,
			back = m_mesh->back.z;

		(m_topLeftBack = {left,top,back}),
			(m_topRightBack = {right,top,back}),
			(m_topLeftFront = {left,top,front}),
			(m_topRightFront = {right,top,front}),
			(m_bottomLeftBack = {left,bottom,back}),
			(m_bottomRightBack = {right,bottom,back}),
			(m_bottomLeftFront = {left,bottom,front}),
			(m_bottomRightFront = {right,bottom,front});


		boundingBoxInit();
		boundingBoxUpdate();
	}
}


Model::~Model()
{
	if(!m_copy)
		delete m_mesh;
}

/// - Collision Function - ///

bool Model::collision2D(Model* box2, Coord3D<>ignore)
{
	return collision2D(this, box2, ignore);
}

bool Model::collision2D(Model* box1, Model* box2, Coord3D<>RPos)
{

	RPos.normalize();
	RPos = (Coord3D<>{1, 1, 1}-RPos);

	RPos = (box1->m_center - box2->m_center) * RPos;
	Coord3D<> AxisX{1,0,0}, AxisY{0,1,0}, AxisZ{0,0,1};

	glm::mat4
		* rot1 = &box1->getRotationMatrix(),
		* rot2 = &box2->getRotationMatrix();

	static Coord3D<> AxisX1, AxisY1, AxisZ1, AxisX2, AxisY2, AxisZ2;
	AxisX1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	AxisY1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	AxisZ1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	AxisX2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	AxisY2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	AxisZ2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	return !(
		getSeparatingPlane(RPos, AxisX1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisX2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ2, *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisZ2), *box1, *box2));

}


///~ 3D Collision Function ~///

bool Model::collision3D(Model* k)
{
	return collision3D(this, k);
}

bool Model::collision3D(Model* box1, Model* box2)
{
	static Coord3D<> RPos;
	RPos = box1->m_center - box2->m_center;
	Coord3D<> AxisX{1,0,0}, AxisY{0,1,0}, AxisZ{0,0,1};

	glm::mat4
		* rot1 = &box1->getRotationMatrix(),
		* rot2 = &box2->getRotationMatrix();

	static Coord3D<> AxisX1, AxisY1, AxisZ1, AxisX2, AxisY2, AxisZ2;
	AxisX1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	AxisY1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	AxisZ1 = *(Coord3D<>*) & (*rot1 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	AxisX2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	AxisY2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	AxisZ2 = *(Coord3D<>*) & (*rot2 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	return !(
		getSeparatingPlane(RPos, AxisX1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisX2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ2, *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisX1, AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisY1, AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, Coord3D<>::crossProduct(AxisZ1, AxisZ2), *box1, *box2));
}

bool Model::getSeparatingPlane(const Coord3D<>& RPos, const Coord3D<>& plane, Model& box1, Model& box2)
{

	RPos;
	Coord3D<> AxisX{1,0,0}, AxisY{0,1,0}, AxisZ{0,0,1};

	glm::mat4
		* trans1 = &box1.getRotationMatrix(),
		* trans2 = &box2.getRotationMatrix();

	Coord3D<> AxisX1 = *(Coord3D<>*) & (*trans1 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	Coord3D<> AxisY1 = *(Coord3D<>*) & (*trans1 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	Coord3D<> AxisZ1 = *(Coord3D<>*) & (*trans1 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	Coord3D<> AxisX2 = *(Coord3D<>*) & (*trans2 * glm::vec4(*(glm::vec3*) & AxisX, 1));
	Coord3D<> AxisY2 = *(Coord3D<>*) & (*trans2 * glm::vec4(*(glm::vec3*) & AxisY, 1));
	Coord3D<> AxisZ2 = *(Coord3D<>*) & (*trans2 * glm::vec4(*(glm::vec3*) & AxisZ, 1));

	//float w, h;
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);

	return (fabs(Coord3D<>::dotProduct(RPos, plane)) >
		(
			fabs(Coord3D<>::dotProduct((AxisX1 * (box1.m_width / 2)), plane)) +
			fabs(Coord3D<>::dotProduct((AxisY1 * (box1.m_height / 2)), plane)) +
			fabs(Coord3D<>::dotProduct((AxisZ1 * (box1.m_depth / 2)), plane)) +

			fabs(Coord3D<>::dotProduct((AxisX2 * (box2.m_width / 2)), plane)) +
			fabs(Coord3D<>::dotProduct((AxisY2 * (box2.m_height / 2)), plane)) +
			fabs(Coord3D<>::dotProduct((AxisZ2 * (box2.m_depth / 2)), plane))
			));


	//std::vector<Coord3D> boxVert1
	//{
	//	box1.m_topLeftBack,
	//	box1.m_topRightBack,
	//	box1.m_topLeftFront,
	//	box1.m_topRightFront,
	//	box1.m_bottomLeftBack,
	//	box1.m_bottomRightBack,
	//	box1.m_bottomLeftFront,
	//	box1.m_bottomRightFront
	//};
	//
	//std::vector<Coord3D> boxVert2
	//{
	//	box2.m_topLeftBack,
	//	box2.m_topRightBack,
	//	box2.m_topLeftFront,
	//	box2.m_topRightFront,
	//	box2.m_bottomLeftBack,
	//	box2.m_bottomRightBack,
	//	box2.m_bottomLeftFront,
	//	box2.m_bottomRightFront
	//};
	//
	//float min1, max1, min2, max2;
	//min1 = max1 = dotProduct(boxVert1[0], plane);
	//for(int i = 1; i < 8; i++)
	//{
	//	float value = dotProduct(boxVert1[i], plane);
	//	min1 = std::min(min1, value);
	//	max1 = std::max(max1, value);
	//}
	//
	//min2 = max2 = dotProduct(boxVert2[0], plane);
	//for(int i = 1; i < 8; i++)
	//{
	//	float value = dotProduct(boxVert2[i], plane);
	//	min2 = std::min(min2, value);
	//	max2 = std::max(max2, value);
	//}
	//
	//return (max1 < min2 || max2 < min1);
}


void Model::render(Shader& shader, Camera* cam)
{
	float colour[4]{(float)m_colour.r / 255,(float)m_colour.g / 255,(float)m_colour.b / 255,(float)m_colour.a / 255};
	m_camera = cam;
	m_shader = &shader;
	//glm::mat4 transformationMat(1);
	shader.enable();
	glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &(getTransformation()[0][0]));

	glUniform4fv(shader.getUniformLocation("colourMod"), 1, colour);
	shader.disable();

	if(m_animations[m_animation])
		m_animations[m_animation]->update(&shader, m_mesh);

	// update the position of the object
	m_transBB = getTransformation();
	boundingBoxUpdate();

	if(m_render)
	{
		//render the mesh
		m_mesh->render(shader);

		if(m_enableBB)
			drawBoundingBox();

		static Shader* shader2;
		//render child meshes
		for(auto& a : getChildren())
			switch(a->getType())
			{
			case MODEL:
				reclass(Model*, a)->render(shader, cam);
				break;
			case TEXT:
				shader2 = ResourceManager::getShader("shaders/freetype.vtsh", "shaders/freetype.fmsh");
				reclass(Text*, a)->render(*shader2, cam);
				break;

			}
		resetUpdated();
	}
}

void Model::drawBoundingBox()
{

	m_shaderBB->enable();
	m_shaderBB->sendUniform("colourMod", {0, 0, 0, 1});
	//m_shaderBB->disable();

	glBindVertexArray(m_BBVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);

	m_shaderBB->disable();
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

bool Model::loadModel(const char* path)
{
	return m_mesh->loadMesh(path);
}

void Model::enableBoundingBox(bool enable)
{
	m_enableBB = enable;
}

void Model::addAnimation(std::string tag, Animation* animation)
{
	m_animations[tag] = animation;
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

Coord3D<> Model::getSize()
{

	return {m_width,m_height,m_depth};
}

Coord3D<> Model::getCenter()
{

	return m_center;
}

void Model::boundingBoxUpdate()
{
	if(m_enableBB && m_shaderBB)
	{
		m_shaderBB->enable();
		glUniformMatrix4fv(m_shaderBB->getUniformLocation("uModel"), 1, GL_FALSE, &(m_transBB[0][0]));
		glUniformMatrix4fv(m_shaderBB->getUniformLocation("uView"), 1, GL_FALSE, &((m_camera->getObjectMatrix() * m_camera->getViewMatrix())[0][0]));
		glUniformMatrix4fv(m_shaderBB->getUniformLocation("uProj"), 1, GL_FALSE, &(m_camera->getProjectionMatrix()[0][0]));
		m_shaderBB->disable();
	}

	std::vector<glm::vec4> bounds =
	{
	{*(glm::vec3*) & m_mesh->right,1},
	{*(glm::vec3*) & m_mesh->left,1},
	{*(glm::vec3*) & m_mesh->top,1},
	{*(glm::vec3*) & m_mesh->bottom,1},
	{*(glm::vec3*) & m_mesh->front,1},
	{*(glm::vec3*) & m_mesh->back,1}
	};


	for(auto& a : bounds)
		a = getScaleMatrix() * a;


	m_width = abs(bounds[0].x - bounds[1].x);
	m_height = abs(bounds[2].y - bounds[3].y);
	m_depth = abs(bounds[4].z - bounds[5].z);


	bounds =
	{
	{*(glm::vec3*) & m_mesh->right,1},
	{*(glm::vec3*) & m_mesh->left,1},
	{*(glm::vec3*) & m_mesh->top,1},
	{*(glm::vec3*) & m_mesh->bottom,1},
	{*(glm::vec3*) & m_mesh->front,1},
	{*(glm::vec3*) & m_mesh->back,1}
	};


	for(auto& a : bounds)
		a = getTransformation() * a;

	m_center =
		(Coord3D<>(
			bounds[0].x + bounds[1].x,
			bounds[2].y + bounds[3].y,
			bounds[4].z + bounds[5].z) / 2);
}

Animation* Model::getAnimation(const char* tag)
{
	return m_animations[tag];
}

Animation* Model::getCurrentAnimation()
{
	return m_animations[m_animation];
}

void Model::setAnimation(const char* tag)
{
	m_animation = tag;
}

Mesh* Model::getMesh()
{
	return m_mesh;
}

Shader* Model::getShader()
{
	return m_shader;
}

void Model::replaceTexture(int mesh, int index, GLuint tex)
{
	m_mesh->replaceTexture(mesh, index, tex);
}

void Model::setToRender(bool render)
{
	m_render = render;
}

void Model::setTransparent(bool trans)
{
	m_transparent = trans;
}

bool Model::isTransparent()
{
	return m_transparent;
}

void Model::boundingBoxInit()
{
	if(!m_BBVaoID)
		glGenVertexArrays(1, &m_BBVaoID);
	if(!m_BBVboID)
		glGenBuffers(1, &m_BBVboID);



	Vertex3D
		topLeftBack{m_topLeftBack},
		topRightBack{m_topRightBack},
		topLeftFront{m_topLeftFront},
		topRightFront{m_topRightFront},
		bottomLeftBack{m_bottomLeftBack},
		bottomRightBack{m_bottomRightBack},
		bottomLeftFront{m_bottomLeftFront},
		bottomRightFront{m_bottomRightFront};


	Vertex3D tmp[12 * 3]{
		//top
		topLeftBack,topRightBack,topRightFront,
		topLeftBack,topRightFront,topLeftFront,
		//bottom
		 bottomRightFront, bottomRightBack,bottomLeftBack,
		bottomLeftFront,bottomRightFront,bottomLeftBack,
		//front
		topLeftFront,topRightFront,bottomRightFront,
		topLeftFront,bottomRightFront,bottomLeftFront,
		//back
		bottomRightBack, topRightBack, topLeftBack,
		bottomLeftBack, bottomRightBack, topLeftBack,
		//left
		topLeftBack,topLeftFront,bottomLeftFront,
		topLeftBack,bottomLeftFront,bottomLeftBack,
		//right
		bottomRightFront,topRightFront,topRightBack,
		bottomRightBack,bottomRightFront,topRightBack
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

void Model::print()
{
	printf(
		"Tag: %s\n"
		"Width: %f\n"
		"Height: %f\n"
		"Depth: %f\n"
		"Center: (%f, %f, %f)\n"
		, m_tag, m_width, m_height, m_depth, m_center.x, m_center.y, m_center.z);
}

