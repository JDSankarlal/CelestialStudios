#include "Mesh.h"
#include "Texture.h"
#include "ResourceManager.h"

Mesh::Mesh()
{}

Mesh::Mesh(Mesh &mesh) :
	m_vaoID(mesh.m_vaoID),
	m_vboID(mesh.m_vboID),
	m_numVerts(mesh.m_numVerts),
	m_textures(mesh.m_textures)
{
}

Mesh::~Mesh()
{
	printf("Deleated Mesh\n");
	unload();
}

void cDir(char *dir)
{
	int a = 0;
	if (strlen(dir) > 0)
		while (dir[a])
		{
			if (dir[a] == '\\')
			{
				dir[a] = '/';
			}
			a++;
		}
	a = 1;
	if (strlen(dir) > 1)
		while (dir[a])
		{
			if (std::string((const char *)dir + a - 1,(size_t) 2) == "//")
			{
				memmove_s(dir + a - 1, strlen(dir + a), dir + a, strlen(dir + a));
				dir[strlen(dir) - 1] = 0;
			}
			a++;
		}
}

void Mesh::loadMaterials(std::string path)
{
	FILE* f;
	fopen_s(&f, path.c_str(), "r");
	char str[CHAR_BUFF_SIZE];
	char* check;

	while (check = fgets(str, CHAR_BUFF_SIZE, f),
		//this part takes out the '\n' from the string
		(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
		check)
		if (strstr(str, "mtllib"))
		{
			char str2[CHAR_BUFF_SIZE];
			memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);
			path = (path.substr(0, path.rfind('/') + 1) + str2);
			break;
		}

	fclose(f);

	fopen_s(&f, path.c_str(), "r");



	while (check = fgets(str, CHAR_BUFF_SIZE, f),
		//this part takes out the '\n' from the string
		(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
		check)
	{

		if (strchr(str, '#'))
			memset(strchr(str, '#'), '\0', sizeof(char));

		if (strstr(str, "newmtl"))
		{
			char str2[CHAR_BUFF_SIZE];
			sscanf_s(str, "newmtl %s", str2, CHAR_BUFF_SIZE);
			m_textures.push_back({ std::string(str2), Texture2D() });

			if (strstr(str, "None"))
				return;
		}

		else if (strstr(str, "illum"))
		{
			continue;
		}
		else if (strstr(str, "map_Kd"))
		{
			char str2[CHAR_BUFF_SIZE];
			sscanf_s(str, "map_Kd %s", &str2, (unsigned)_countof(str2));
			cDir((char *)path.c_str());
			//path.resize(path.c_str());
			cDir(str2);
			std::string tmpStr(path.substr(0, path.find_last_of('/') + 1) + str2);
			m_textures.back().second.id = ResourceManager::getTexture2D(tmpStr.c_str()).id;
			m_textures.back().second.type = TextureType::DIFFUSE;

		}
		else if (strstr(str, "map_Ks"))
		{
			//float r, g, b;
			//sscanf_s(str, "Kd %f %f %f", &r, &g, &b);
			//textures.back().second.colourSpec.set(255 * r, 255 * g, 255 * b);
		}
		else if (strstr(str, "Ns"))
		{
			continue;
		}
		else if (strstr(str, "Ka"))
		{
			float a[3];
			sscanf_s(str, "Ka %f %f %f", &a[0], &a[1], &a[2]);
			m_textures.back().second.colour.a = 255 * a[0] * a[1] * a[2];
		}
		else if (strstr(str, "Kd"))
		{
			float r, g, b;
			sscanf_s(str, "Kd %f %f %f", &r, &g, &b);
			m_textures.back().second.colour.set(255 * r, 255 * g, 255 * b);
		}
		else if (strstr(str, "Ks"))
		{
			continue;
		}
		else if (strstr(str, "Ke"))
		{
			continue;
		}
		else if (strstr(str, "Ni"))
		{
			continue;
		}
		else if (strchr(str, 'd'))
		{
			continue;
		}


	}
	fclose(f);

}

bool Mesh::loadMesh(std::string path)
{
	if (!strstr(path.c_str(), ".obj"))return false;

	unload();

	loadMaterials(path);

	FILE* f;

	fopen_s(&f, path.c_str(), "r");

	if (!f)
	{
		printf("unknown file\n");
		return false;
	}

	char inputBuff[CHAR_BUFF_SIZE];

	std::vector<Coord3D> verts;
	std::vector<UV> uvs;
	std::vector<Coord3D> norms;

	std::vector < std::pair<std::string, std::vector<Vertex3D>>> faces;

	char *check = nullptr;
	while (check = fgets(inputBuff, CHAR_BUFF_SIZE, f),
		//this part takes out the '\n' from the string
		(inputBuff == nullptr ? "" : (inputBuff[strlen(inputBuff) - 1] = (inputBuff[strlen(inputBuff) - 1] == '\n' ? ' ' : inputBuff[strlen(inputBuff) - 1]), inputBuff)),
		check)
	{

		if (strchr(inputBuff, '#'))
			memset(strchr(inputBuff, '#'), '\0', sizeof(char));

		if (strstr(inputBuff, "usemtl")) {

			char str[CHAR_BUFF_SIZE];
			sscanf_s(inputBuff, "usemtl %s", str, CHAR_BUFF_SIZE);
			faces.push_back({ std::string(str),std::vector< Vertex3D>() });
			//colours = loadMaterials(newPath);
		}
		else
			if (strstr(inputBuff, "vt"))
			{
				//UV Dat

				UV tmp;
				sscanf_s(inputBuff, "vt %f %f", &tmp.u, &tmp.v);
				uvs.push_back(tmp);
			}
			else if (strstr(inputBuff, "vn"))
			{
				//Normal data
				Coord3D tmp;
				sscanf_s(inputBuff, "vt %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				norms.push_back(tmp);
			}
			else if (strchr(inputBuff, 'o'))
				continue;
			else if (strchr(inputBuff, 's'))
				continue;
			else if (strchr(inputBuff, 'f'))
			{
				//Face Dat

				Vertex3D tmp;

				if ([&]()->GLuint
				{
					char tmp = ' ';
						int counter = 0, count = 0;
						while (tmp = inputBuff[counter++])
							count += (tmp == '/');

						return count;
				}() == 6)
				{
					sscanf_s(inputBuff, "f %f/%f/%f %f/%f/%f %f/%f/%f",
						&tmp.coord[0], &tmp.uv[0], &tmp.norm[0],
						&tmp.coord[1], &tmp.uv[1], &tmp.norm[1],
						&tmp.coord[2], &tmp.uv[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f//%f %f//%f %f//%f",
							&tmp.coord[0], &tmp.norm[0],
							&tmp.coord[1], &tmp.norm[1],
							&tmp.coord[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f// %f// %f//",
							&tmp.coord[0],
							&tmp.coord[1],
							&tmp.coord[2]);
				}
				else
				{
					sscanf_s(inputBuff, "f %f/%f/%f %f/%f/%f %f/%f/%f",
						&tmp.coord[0], &tmp.uv[0], &tmp.norm[0],
						&tmp.coord[1], &tmp.uv[1], &tmp.norm[1],
						&tmp.coord[2], &tmp.uv[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f//%f %f//%f %f//%f",
							&tmp.coord[0], &tmp.norm[0],
							&tmp.coord[1], &tmp.norm[1],
							&tmp.coord[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f// %f// %f//",
							&tmp.coord[0],
							&tmp.coord[1],
							&tmp.coord[2]);

					faces.back().second.push_back(tmp);

					tmp = Vertex3D();
					sscanf_s(inputBuff, "f %f/%f/%f %*f/%*f/%*f %f/%f/%f %f/%f/%f",
						&tmp.coord[0], &tmp.uv[0], &tmp.norm[0],
						&tmp.coord[1], &tmp.uv[1], &tmp.norm[1],
						&tmp.coord[2], &tmp.uv[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f//%f %*f//%*f %f//%f %f//%f",
							&tmp.coord[0], &tmp.norm[0],
							&tmp.coord[1], &tmp.norm[1],
							&tmp.coord[2], &tmp.norm[2]);

					if (!tmp.coord[1])
						sscanf_s(inputBuff, "f %f// %*f// %f// %f//",
							&tmp.coord[0],
							&tmp.coord[1],
							&tmp.coord[2]);

				}
				faces.back().second.push_back(tmp);


			}
			else if (strchr(inputBuff, 'v'))
			{
				//Vertex Dat

				Coord3D tmp;
				sscanf_s(inputBuff, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				verts.push_back(tmp);
			}

	}
	fclose(f);

	//ColourRGBA rgbaTmp;
	//for (unsigned a = 0; a < colours.size(); a++)
	//	rgbaTmp *= colours[a].second.m_colour;

	//unpacked data
	for (unsigned int a = 0; a < faces.size(); a++)
	{
		m_unpackedData.push_back({ faces[a].first,std::vector<Vertex3D>() });

		for (unsigned int c = 0; c < faces[a].second.size(); c++)
			for (unsigned int b = 0; b < 3; b++)
			{
				Vertex3D tmp;

				//set Verticies
				tmp.coord.set(
					verts[faces[a].second[c].coord[b] - 1].x,
					verts[faces[a].second[c].coord[b] - 1].y,
					verts[faces[a].second[c].coord[b] - 1].z);

				//set UV's
				if (faces[a].second[c].uv[b])
					tmp.uv.set(uvs[faces[a].second[c].uv[b] - 1].u, uvs[faces[a].second[c].uv[b] - 1].v);

				//set Normals
				if (faces[a].second[c].norm[b])
					tmp.norm.set(
						norms[faces[a].second[c].norm[b] - 1].x,
						norms[faces[a].second[c].norm[b] - 1].y,
						norms[faces[a].second[c].norm[b] - 1].z);


				m_unpackedData.back().second.push_back(tmp);
			}
		m_numFaces.push_back(faces[a].second.size());
		m_numVerts.push_back(m_numFaces[a] * 3);
	}


	init();

	verts.clear();
	uvs.clear();
	faces.clear();
	for (auto &a : m_unpackedData)
		a.second.clear();
	m_unpackedData.clear();

	return true;
}

void Mesh::render(GLSLCompiler& shader)
{
	for (int a = 0; a < m_vaoID.size(); a++)
	{
		int c = 0;
		for (int b = 0; b < m_textures.size(); b++) {
			if (m_textures[b].first == m_vaoID[a].first)
			{
				//glActiveTexture(GL_TEXTURE0 + c);
		
		
				//if (m_textures[b].second.type == TextureType::DIFFUSE)
					glUniform1i(shader.getUniformLocation("diffuseTexture"), 0);
				//c++;
		
				glBindTexture(GL_TEXTURE_2D, m_textures[b].second.id);
			}
		}

		glBindVertexArray(m_vaoID[a].second);
		glDrawArrays(GL_TRIANGLES, 0, m_numVerts[a]);
		glBindVertexArray(0);
		c = 0;

		for (int b = 0; b < m_textures.size(); b++) {
			if (m_textures[b].first == m_vaoID[a].first)
			{
			//	glActiveTexture(GL_TEXTURE0 + c++);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
}

void Mesh::unload()
{
	for (int a = 0; a < m_numFaces.size(); a++)
	{
		if (m_vboID[a])
			glDeleteBuffers(1, &m_vboID[a]);
		if (m_vaoID[a].second)
			glDeleteVertexArrays(1, &m_vaoID[a].second);
	}
	m_vboID.clear();
	m_vaoID.clear();
	m_numVerts.clear();
	m_numFaces.clear();
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
	for (int a = 0; a < m_numFaces.size(); a++)
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

		////ColourRGBA atributes
		//glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, m_colour));

		//UV         atributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		//normal     atributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
