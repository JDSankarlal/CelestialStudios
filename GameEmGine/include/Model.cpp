#include "Model.h" 

using namespace std;
Model::Model()
{}

Model::Model(const char * path)
{
	loadModel(path);
}

Model::~Model()
{}

void Model::render(GLSLCompiler& shader, Camera& cam)
{
	shader.enable();
	// update the position of the object
	glUniformMatrix4fv(shader.getUniformLocation("camera"), 1, GL_FALSE, &(cam.getCameraMatrix()[0][0]));
	glUniformMatrix4fv(shader.getUniformLocation("object"), 1, GL_FALSE, &((cam.getObjectMatrix()*transform.getTransformation())[0][0]));
	for(GLuint a = 0; a < meshes.size(); a++)
	{

		//render the mesh
		meshes[a].render(shader);
	}
	shader.disable();
}

Transformer& Model::getTransformer()
{
	return transform;
}

void Model::loadModel(const char * path)
{

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("Load Model Failed: %s\n", importer.GetErrorString());
		return;
	}

	dir = string(path).substr(0, strrchr(path, '/') - path);
	printf("%s\n", dir.c_str());

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	Mesh tmp;
	for(GLuint a = 0; a < node->mNumMeshes; a++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[a]];
		tmp = processMesh(mesh, scene);
		//bool add = false;
		//
		//for(auto &b : meshes)
		//	if(b.m_textures == tmp.m_textures)
		//	{
		//		b.m_indicies.insert(b.m_indicies.end(), tmp.m_indicies.begin(), tmp.m_indicies.end());
		//		b.m_verts.insert(b.m_verts.end(), tmp.m_verts.begin(), tmp.m_verts.end());
		//		break;
		//	}
		//
		//if(add)
			meshes.push_back(tmp);
	}

	for(GLuint a = 0; a < node->mNumChildren; a++)
		processNode(node->mChildren[a], scene);

}

vector< Texture2D> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType texType, string typeName)
{
	vector<Texture2D> textures;
	for(unsigned int a = 0; a < mat->GetTextureCount(texType); a++)
	{
		aiString str;
		mat->GetTexture(texType, a, &str);

		Texture2D texture;
		texture = ResourceManager::getTexture2D((string(dir) + '/' + str.C_Str()).c_str());
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
		printf("loaded Material: %s\n", str.C_Str());
		loadedTextures.push_back(texture);

	}

	return textures;
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)

{
	std::vector<Vertex3D> verts;
	std::vector<GLuint> indecies;
	std::vector<Texture2D> textures;

	for(int a = 0; a < mesh->mNumVertices; a++)
	{
		Vertex3D vert;
		Coord3D vec;

		vec.x = mesh->mVertices[a].x;
		vec.y = mesh->mVertices[a].y;
		vec.z = mesh->mVertices[a].z;
		//glm::vec4 tmp = getTransformer().getTransformation()* glm::vec4(mesh->mVertices[a].x, mesh->mVertices[a].y, mesh->mVertices[a].z, 1);
		//vec = {tmp.x,tmp.y,tmp.z};
		vert.coord = vec;


		vec.x = mesh->mNormals[a].x;
		vec.y = mesh->mNormals[a].y;
		vec.z = mesh->mNormals[a].z;
		vert.norm = vec;

		if(mesh->mTextureCoords[0])
		{
			UV uv;

			uv.u = mesh->mTextureCoords[0][a].x;
			uv.v = mesh->mTextureCoords[0][a].y;

			vert.uv = uv;

		}

		verts.push_back(vert);
	}
	for(int a = 0; a < mesh->mNumFaces; a++)
	{
		aiFace norm = mesh->mFaces[a];
		for(int b = 0; b < norm.mNumIndices; b++)
			indecies.push_back(norm.mIndices[b]);
	}

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		//some stuff about speular and dufused textures
		std::vector<Texture2D>
			diffuseMaps = loadMaterialTextures(material,
											   aiTextureType_DIFFUSE, "diffuseTexture");

		std::vector<Texture2D>
			specularMaps = loadMaterialTextures(material,
												aiTextureType_SPECULAR, "specularTexture");


		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(verts, indecies, textures);
}

GLuint Model::textureFromFile(const char * path)
{
	GLuint texID;
	int width, height;

	unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

	if(image == nullptr)
	{
		printf("failed\n");
		return NULL;
	}

	//Bind texture to model
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return texID;
}
