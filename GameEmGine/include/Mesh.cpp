#include "Mesh.h"
#include <functional>
Mesh::Mesh()
{}

Mesh::Mesh(Mesh &mesh) :
	m_vaoID(mesh.m_vaoID),
	m_vboID(mesh.m_vboID),
	m_numVerts(mesh.m_numVerts),
	m_textures(mesh.m_textures),
	top(mesh.top),
	bottom(mesh.bottom),
	left(mesh.left),
	right(mesh.right),
	front(mesh.front),
	back(mesh.back)
{}

Mesh::~Mesh()
{
#if _DEBUG
	printf("Deleted Mesh\n");
#endif // _DEBUG

	unload();
}

//converts directory paths
void cDir(char *dir)
{
	char* tmp;
	if(strlen(dir) > 0)
		while(tmp = strchr(dir, '\\'))
		{
			tmp[0] = '/';
		}

	if(strlen(dir) > 1)
		while(tmp = strstr(dir, "//"))
		{
			memmove_s(tmp, strlen(tmp), tmp + 1, strlen(tmp + 1));
			dir[strlen(dir) - 1] = 0;
		}
}

std::string substr(const char* str,const char* find)
{
	char tmp[CHAR_BUFF_SIZE];
	unsigned count = strlen(str);
	for(; count >= 0; --count)
		if(strstr(str + count, find))
			break;

	memset(tmp, 0, CHAR_BUFF_SIZE);
	memmove_s(tmp, CHAR_BUFF_SIZE, str, count+1);
	return std::string(tmp);
}

void Mesh::loadMaterials(const char* path)
{
	FILE* f;
	cDir((char*)path);
	fopen_s(&f, path, "r");
	char str[CHAR_BUFF_SIZE];
	char* MeshCheck;
	std::string tmpDir;
	while(MeshCheck = fgets(str, CHAR_BUFF_SIZE, f),
		  //this part takes out the '\n' from the string
		(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
		  MeshCheck)
		if(strstr(str, "mtllib"))
		{
			//find material
			char str2[CHAR_BUFF_SIZE];
			memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

			//find material location
			char tmp[CHAR_BUFF_SIZE];
			unsigned count = strrchr(path, '/') - path + 1;
			memset(tmp, 0, CHAR_BUFF_SIZE);
			memmove_s(tmp, CHAR_BUFF_SIZE, path, count);
			tmpDir = std::string(tmp) + std::string(str2);

			//set material path
			path = tmpDir.c_str();
			break;
		}

	fclose(f);

	fopen_s(&f, path, "r");


	if(f)
		while(MeshCheck = fgets(str, CHAR_BUFF_SIZE, f),
			  //this part takes out the '\n' from the string
			(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
			  MeshCheck)
		{

			if(strchr(str, '#'))
				memset(strchr(str, '#'), '\0', sizeof(char));

			if(strstr(str, "newmtl"))
			{
				char str2[CHAR_BUFF_SIZE];
				sscanf_s(str, "newmtl %s", str2, CHAR_BUFF_SIZE);
				m_textures.push_back({ std::string(str2), std::vector<Texture2D>() });

				if(strstr(str, "None"))
					return;
			}

			else if(strstr(str, "illum"))
			{
				continue;
			} else if(strstr(str, "map_Kd"))
			{
				char str2[CHAR_BUFF_SIZE];
				sscanf_s(str, "map_Kd %s", &str2, (unsigned)_countof(str2));
				//path.resize(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::DIFFUSE;

			} else if(strstr(str, "map_Ks"))
			{
				char str2[CHAR_BUFF_SIZE];
				sscanf_s(str, "map_Ks %s", &str2, (unsigned)_countof(str2));
				//path.resize(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::SPECULAR;

			} else if(strstr(str, "Ns"))
			{
				continue;
			} else if(strstr(str, "Ka"))
			{
				float a[3];
				sscanf_s(str, "Ka %f %f %f", &a[0], &a[1], &a[2]);
				for(auto &b : m_textures.back().second)
					b.colour.colorA = (GLubyte)(255 * a[0] * a[1] * a[2]);
			} else if(strstr(str, "Kd"))
			{
				float r, g, b;
				sscanf_s(str, "Kd %f %f %f", &r, &g, &b);
				for(auto &a : m_textures.back().second)
					if(a.type == TEXTURE_TYPE::DIFFUSE)
						a.colour.set((GLubyte)r * 255, (GLubyte)g * 255, (GLubyte)b * 255);
			} else if(strstr(str, "Ks"))
			{
				continue;
			} else if(strstr(str, "Ke"))
			{
				continue;
			} else if(strstr(str, "Ni"))
			{
				continue;
			} else if(strchr(str, 'd'))
			{
				continue;
			}
		}

	if(f)
		fclose(f);
}

bool Mesh::loadMesh(std::string path)
{
	if(!strstr(path.c_str(), ".obj"))return false;

	unload();

	FILE* f;

	fopen_s(&f, path.c_str(), "r");

	loadMaterials(path.c_str());

	if(!f)
	{
		printf("unknown file\n");
		return false;
	}

	char inputBuff[CHAR_BUFF_SIZE];

	std::vector<Coord3D> verts;
	std::vector<UV> uvs;
	std::vector<Coord3D> norms;

	std::vector < std::pair<std::string, std::vector<Vertex3D>>> faces;

	char *MeshCheck = nullptr;
	bool initFace = true;
	while(MeshCheck = fgets(inputBuff, CHAR_BUFF_SIZE, f),
		  //this part takes out the '\n' from the string
		(inputBuff == nullptr ? "" : (inputBuff[strlen(inputBuff) - 1] = (inputBuff[strlen(inputBuff) - 1] == '\n' ? ' ' : inputBuff[strlen(inputBuff) - 1]), inputBuff)),
		  MeshCheck)
	{
		//checks for comments
		if(strchr(inputBuff, '#'))
			memset(strchr(inputBuff, '#'), '\0', sizeof(char));

		if(strstr(inputBuff, "usemtl"))
		{

			char str[CHAR_BUFF_SIZE];
			sscanf_s(inputBuff, "usemtl %s", str, CHAR_BUFF_SIZE);
			faces.push_back({ std::string(str),std::vector< Vertex3D>() });
		} else if(strstr(inputBuff, "vt"))
		{
			//UV Dat

			UV tmp;
			sscanf_s(inputBuff, "vt %f %f", &tmp.uv_u, &tmp.uv_v);
			uvs.push_back(tmp);
		} else if(strstr(inputBuff, "vn"))
		{
			//Normal data
			Coord3D tmp;
			sscanf_s(inputBuff, "vn %f %f %f", &tmp.coordX, &tmp.coordY, &tmp.coordZ);
			norms.push_back(tmp);
		} else if(strchr(inputBuff, 'o'))
			continue;
		else if(strchr(inputBuff, 's'))
			continue;
		else if(strchr(inputBuff, 'f'))//Collect Face Data
		{
			//Face Dat

			Vertex3D tmp;

			char check = 0;
			unsigned counter = 0, count = 0;
			while(check = inputBuff[counter++])
				count += (check == '/');

			count /= 2;
			std::string	faceTmp[2][2]
			{ { " %f/%f/%f"," %*f/%*f/%*f" },
				{ " %f//%f"," %*f//%*f" } };

			std::vector<std::string > format = std::vector<std::string>(count);
			std::string formatStr;
			std::function<void(int)> reformat = [&](int type)
			{
				for(unsigned a = 0; a < count; a++)
					if(a < 3)
						format[a] = faceTmp[type][0];
					else
						format[a] = faceTmp[type][1];
			};
			short type = 0;
			reformat(type);
			formatStr = "f";

			for(unsigned a = 0; a < count; a++)
				formatStr += format[a];

			sscanf_s(inputBuff, formatStr.c_str(),
					 &tmp.coord[0], &tmp.uv[0], &tmp.norm[0],
					 &tmp.coord[1], &tmp.uv[1], &tmp.norm[1],
					 &tmp.coord[2], &tmp.uv[2], &tmp.norm[2]);

			if(!tmp.coord[1])
			{
				reformat(++type);
				formatStr = "f";
				for(unsigned a = 0; a < count; a++)
					formatStr += format[a];
				sscanf_s(inputBuff, formatStr.c_str(),
						 &tmp.coord[0], &tmp.norm[0],
						 &tmp.coord[1], &tmp.norm[1],
						 &tmp.coord[2], &tmp.norm[2]);
			}
			faces.back().second.push_back(tmp);

			for(unsigned a = 1; a < count - 2; a++)
			{
				formatStr = "f";
				swap(format[a], format[a + 2]);
				for(unsigned i = 0; i < count; i++)
					formatStr += format[i];
				if(type == 0)
					sscanf_s(inputBuff, formatStr.c_str(),
							 &tmp.coord[0], &tmp.uv[0], &tmp.norm[0],
							 &tmp.coord[1], &tmp.uv[1], &tmp.norm[1],
							 &tmp.coord[2], &tmp.uv[2], &tmp.norm[2]);
				else
					sscanf_s(inputBuff, formatStr.c_str(),
							 &tmp.coord[0], &tmp.norm[0],
							 &tmp.coord[1], &tmp.norm[1],
							 &tmp.coord[2], &tmp.norm[2]);

				faces.back().second.push_back(tmp);
			}

		} else if(strchr(inputBuff, 'v'))//Collects Vertex Data
		{
			//Vertex Data

			Coord3D tmp;
			sscanf_s(inputBuff, "v %f %f %f", &tmp.coordX, &tmp.coordY, &tmp.coordZ);
			verts.push_back(tmp);
			if(initFace)
			{
				front = back = left = right = top = bottom = tmp;
				initFace = false;
			} else
			{
				front = tmp.coordZ > front.coordZ ? tmp : front;
				back = tmp.coordZ < back.coordZ ? tmp : back;
				left = tmp.coordX < left.coordX ? tmp : left;
				right = tmp.coordX > right.coordX ? tmp : right;
				top = tmp.coordY > top.coordY ? tmp : top;
				bottom = tmp.coordY < bottom.coordY ? tmp : bottom;
			}
		}

	}
	fclose(f);


	//unpacked data
	for(unsigned int a = 0; a < faces.size(); a++)
	{
		m_unpackedData.push_back({ faces[a].first,std::vector<Vertex3D>() });

		for(unsigned int c = 0; c < faces[a].second.size(); c++)
			for(unsigned int b = 0; b < 3; b++)
			{
				Vertex3D tmp;

				//set Verticies
				tmp.setCoord(
					verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].coordX,
					verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].coordY,
					verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].coordZ);

				//set UV's
				if(faces[a].second[c].uv[0])
				{
					tmp.setUV(uvs[(unsigned int)(faces[a].second[c].uv[b] - 1)].uv_u, uvs[(unsigned int)(faces[a].second[c].uv[b] - 1)].uv_v);
				}

				//set Normals
				if(faces[a].second[c].norm[0])
				{
					tmp.setNorm(
						norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].coordX,
						norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].coordY,
						norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].coordZ);
				}

				m_unpackedData.back().second.push_back(tmp);
			}
		m_numFaces.push_back((unsigned int)faces[a].second.size());
		m_numVerts.push_back((unsigned int)m_numFaces[a] * 3);
	}


	init();

	verts.clear();
	uvs.clear();
	faces.clear();
	for(auto &a : m_unpackedData)
		a.second.clear();
	m_unpackedData.clear();

	return true;
}

void Mesh::render(Shader& shader)
{
	for(unsigned a = 0; a < m_vaoID.size(); a++)
	{
		bool textured = false;
		int c = 0;
		for(unsigned b = 0; b < m_textures.size(); b++)
		{
			if(m_textures[b].first == m_vaoID[a].first)
			{
				glActiveTexture(GL_TEXTURE0 + c);

				for(auto &d : m_textures[b].second)
					if(d.type == TEXTURE_TYPE::DIFFUSE)
						textured = true,
						glUniform1i(shader.getUniformLocation("uTex"), c),
						glBindTexture(GL_TEXTURE_2D, d.id);

				c++;
			}
		}

		glUniform1i(shader.getUniformLocation("textured"), textured);

		glBindVertexArray(m_vaoID[a].second);
		glDrawArrays(GL_TRIANGLES, 0, m_numVerts[a]);
		glBindVertexArray(0);

		for(; c >= 0; c--)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D, 0);
			//glActiveTexture(0);
		}
	}
}

GLuint Mesh::getNumFaces(int index) const
{
	return m_numFaces[index];
}

GLuint Mesh::getNumVerticies(int index) const
{
	return m_numVerts[index];
}

void Mesh::init()
{
	for(unsigned a = 0; a < m_numFaces.size(); a++)
	{
		m_vaoID.push_back({ m_unpackedData[a].first ,0 });
		m_vboID.push_back(0);

		glGenVertexArrays(1, &m_vaoID[a].second);
		glGenBuffers(1, &m_vboID[a]);

		glBindVertexArray(m_vaoID[a].second);


		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a]);
		glBufferData(GL_ARRAY_BUFFER, m_unpackedData[a].second.size() * sizeof(Vertex3D), m_unpackedData[a].second.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//vertex     atributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

		//UV         atributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		//normal     atributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::unload()
{
	for(unsigned a = 0; a < m_numFaces.size(); a++)
	{
		if(m_vboID[a])
			glDeleteBuffers(1, &m_vboID[a]);
		if(m_vaoID[a].second)
			glDeleteVertexArrays(1, &m_vaoID[a].second);
	}
	m_vboID.clear();
	m_vaoID.clear();
	m_numVerts.clear();
	m_numFaces.clear();
}