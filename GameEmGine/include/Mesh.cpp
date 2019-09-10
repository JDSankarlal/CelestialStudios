#include "Mesh.h"
#include <functional>
#include <filesystem>

namespace fs = std::experimental::filesystem;

Mesh::Mesh()
{
	if(!fs::exists("Models/BIN"))
		system("mkdir \"Models/BIN\"");
}

Mesh::Mesh(Mesh& mesh):
	m_vaoID(mesh.m_vaoID),
	m_vboID(mesh.m_vboID),
	m_numVerts(mesh.m_numVerts),
	m_textures(mesh.m_textures),
	m_replaceTex(mesh.m_replaceTex),
	top(mesh.top),
	bottom(mesh.bottom),
	left(mesh.left),
	right(mesh.right),
	front(mesh.front),
	back(mesh.back)
{
	if(!fs::exists("Models/BIN"))
		system("mkdir \"Models/BIN\"");
}

Mesh::~Mesh()
{
#if _DEBUG
	printf("Deleted Mesh\n");
#endif // _DEBUG

	if(!ani)
		unload();

	m_textures.clear();
	m_replaceTex.clear();
}

std::string substr(const char* str, const char* find)
{
	char tmp[CHAR_BUFF_SIZE];
	unsigned count = (unsigned)strlen(str);
	for(; count >= 0; --count)
		if(strstr(str + count, find))
			break;

	memset(tmp, 0, CHAR_BUFF_SIZE);
	memmove_s(tmp, CHAR_BUFF_SIZE, str, count + 1);
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
			unsigned count = unsigned(strrchr(path, '/') - path) + 1;
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
				m_textures.push_back({std::string(str2), std::vector<Texture2D>()});
				m_replaceTex.push_back(std::vector<GLuint>());
				if(strstr(str, "None"))
					return;
			}

			else if(strstr(str, "illum"))
			{
				continue;
			}
			else if(strstr(str, "map_Kd"))
			{
				char str2[CHAR_BUFF_SIZE];
				//sscanf_s(str, "map_Kd %s", &str2, (unsigned)_countof(str2));
				memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

				//path.resizeDepth(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::DIFFUSE;
				m_replaceTex.back().push_back(0);

			}
			else if(strstr(str, "map_Ks"))
			{
				char str2[CHAR_BUFF_SIZE];
				memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

				//path.resizeDepth(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::SPECULAR;
				m_replaceTex.back().push_back(0);

			}
			else if(strstr(str, "Ns"))
			{
				continue;
			}
			else if(strstr(str, "Ka"))
			{
				float a[3];
				sscanf_s(str, "Ka %f %f %f", &a[0], &a[1], &a[2]);
				for(auto& b : m_textures.back().second)
					b.colour.a = (GLubyte)(255 * a[0] * a[1] * a[2]);
			}
			else if(strstr(str, "Kd"))
			{
				float r, g, b;
				sscanf_s(str, "Kd %f %f %f", &r, &g, &b);
				for(auto& a : m_textures.back().second)
					if(a.type == TEXTURE_TYPE::DIFFUSE)
						a.colour.set(r, g, b);
			}
			else if(strstr(str, "Ks"))
			{
				continue;
			}
			else if(strstr(str, "Ke"))
			{
				continue;
			}
			else if(strstr(str, "Ni"))
			{
				continue;
			}
			else if(strchr(str, 'd'))
			{
				continue;
			}
		}

	if(f)
		fclose(f);
}

bool Mesh::load(std::string path)
{
	if(!strstr(path.c_str(), ".obj"))return false;
	unload();

	std::vector<Coord3D<>> verts;
	std::vector<UV> uvs;
	std::vector<Coord3D<>> norms;

	std::vector < std::pair<std::string, std::vector<Vertex3D>>> faces;
	FILE* bin;
	cDir((char*)path.c_str());


	//path.insert(path.begin(), '\"');
	//path.insert(path.end(), '\"');

	if(!fs::exists((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin"))
	{
		FILE* f;

		fopen_s(&f, path.c_str(), "r");

		loadMaterials(path.c_str());

		if(!f)
		{
			printf("unknown file\n");
			return false;
		}

		char inputBuff[CHAR_BUFF_SIZE];


		char* MeshCheck = nullptr;
		bool initFace = true;
		while(MeshCheck = fgets(inputBuff, CHAR_BUFF_SIZE, f),
			//this part takes out the '\n' from the string
			(inputBuff == nullptr ? "" : (inputBuff[strlen(inputBuff) - 1] = (inputBuff[strlen(inputBuff) - 1] == '\n' ? ' ' : inputBuff[strlen(inputBuff) - 1]), inputBuff)),
			MeshCheck)
		{
			//checks for comments
			if(strchr(inputBuff, '#'))
				memset(strchr(inputBuff, '#'), '\0', sizeof(char));
			if(strstr(inputBuff, "mtllib"))

				continue;

			if(strstr(inputBuff, "usemtl"))
			{

				char str[CHAR_BUFF_SIZE];
				sscanf_s(inputBuff, "usemtl %s", str, CHAR_BUFF_SIZE);
				faces.push_back({std::string(str),std::vector< Vertex3D>()});
			}
			else if(strstr(inputBuff, "vt"))
			{
				//UV data

				UV tmp;
				sscanf_s(inputBuff, "vt %f %f", &tmp.uv_u, &tmp.uv_v);
				uvs.push_back(tmp);
			}
			else if(strstr(inputBuff, "vn"))
			{
				//Normal data
				Coord3D<> tmp;
				sscanf_s(inputBuff, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				norms.push_back(tmp);
			}
			else if(strchr(inputBuff, 'o'))
				continue;
			else if(strchr(inputBuff, 's'))
				continue;
			else if(strchr(inputBuff, 'f'))//Collect Face Data
			{
				//Face data

				Vertex3D tmp;

				char check = 0;
				unsigned counter = 0, count = 0;
				while(bool(check = inputBuff[counter++]))
					count += (check == '/');

				count /= 2;
				std::string	faceTmp[2][2]
				{{ " %f/%f/%f"," %*f/%*f/%*f" },
					{ " %f//%f"," %*f//%*f" }};

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

			}
			else if(strchr(inputBuff, 'v'))//Collects Vertex Data
			{
				//Vertex Data

				Coord3D<> tmp;
				sscanf_s(inputBuff, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				verts.push_back(tmp);
				if(initFace)
				{
					front = back = left = right = top = bottom = tmp;
					initFace = false;
				}
				else
				{
					front = tmp.z > front.z ? tmp : front;
					back = tmp.z < back.z ? tmp : back;
					left = tmp.x < left.x ? tmp : left;
					right = tmp.x > right.x ? tmp : right;
					top = tmp.y > top.y ? tmp : top;
					bottom = tmp.y < bottom.y ? tmp : bottom;
				}
			}

		}
		fclose(f);


		//UNPACKED DATA//
		{

			//open bin file
			fopen_s(&bin, ((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin").c_str(), "wb");

			unsigned meshSize = (int)faces.size(), dataSize = 0, faceSize = 0;

			fwrite(&meshSize, sizeof(char), sizeof(int), bin);
			//fpos_t fpos, fend;

			for(unsigned int a = 0; a < faces.size(); a++)
			{
				dataSize = 0, faceSize = 0;
				int chars = (int)faces[a].first.size() + 1;
				fwrite(&chars, sizeof(char), sizeof(int), bin);
				fwrite(faces[a].first.c_str(), sizeof(char), sizeof(char) * chars, bin);

				//fgetpos(bin, &fpos);
				faceSize = (unsigned int)faces[a].second.size();
				dataSize = faceSize * 3;
				fwrite(&dataSize, sizeof(char), sizeof(unsigned int), bin);
				fwrite(&faceSize, sizeof(char), sizeof(unsigned int), bin);

				m_unpackedData.push_back({faces[a].first,std::vector<Vertex3D>()});

				for(unsigned int c = 0; c < faces[a].second.size(); c++)
					for(unsigned int b = 0; b < 3; b++)
					{
						Vertex3D tmp;
						
						//set Vertices
						tmp.setCoord(
							verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].x,
							verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].y,
							verts[(unsigned int)(faces[a].second[c].coord[b] - 1)].z);


						//set UV's
						if(faces[a].second[c].uv[0])//check if there's data
						{
							//uvSize++;
							tmp.setUV(uvs[(unsigned int)(faces[a].second[c].uv[b] - 1)].uv_u, uvs[(unsigned int)(faces[a].second[c].uv[b] - 1)].uv_v);
						}

						//set Normals
						if(faces[a].second[c].norm[0])//check if there's data
						{
							//normSize++;
							tmp.setNorm(
								norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].x,
								norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].y,
								norms[(unsigned int)(faces[a].second[c].norm[b] - 1)].z);

						}

						fwrite(&tmp, sizeof(char), sizeof(Vertex3D), bin);

						m_unpackedData.back().second.push_back(tmp);
					}
				//fgetpos(bin, &fend);
				//
				//fsetpos(bin, &fpos);
				//fwrite(&dataSize, sizeof(char), sizeof(unsigned), bin);
				//fwrite(&faceSize, sizeof(char), sizeof(unsigned), bin);
				//
				//fsetpos(bin, &fend);//goes to the end of the file

				m_numFaces.push_back((unsigned)faces[a].second.size());
				m_numVerts.push_back((unsigned)m_numFaces[a] * 3);
			}
		}


	}
	else
	{
		loadMaterials(path.c_str());
		unsigned meshes, dataSize = 0, faceSize = 0;;

		fopen_s(&bin, ((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin").c_str(), "rb");

		fread(&meshes, sizeof(int), 1, bin);
		bool initFace = true;

		std::vector<char> str;
		for(unsigned a = 0; a < meshes; a++)
		{
			int chars = 0;
			fread(&chars, sizeof(int), 1, bin);
			str.resize(chars, 0);
			fread(str.data(), sizeof(char), chars, bin);

			fread(&dataSize, sizeof(unsigned int), 1, bin);
			fread(&faceSize, sizeof(unsigned int), 1, bin);

			m_unpackedData.push_back({str.data(),std::vector<Vertex3D>(dataSize)});
			fread(m_unpackedData.back().second.data(), sizeof(Vertex3D), dataSize, bin);

			for(unsigned int b = 0; b < dataSize; b++)
			{
				Vertex3D tmp = m_unpackedData[a].second[b];

				if(initFace)
				{
					front = back = left = right = top = bottom = tmp.coord;
					initFace = false;
				}
				else
				{
					front = tmp.coord.z > front.z ? tmp.coord : front;
					back = tmp.coord.z < back.z ? tmp.coord : back;
					left = tmp.coord.x < left.x ? tmp.coord : left;
					right = tmp.coord.x > right.x ? tmp.coord : right;
					top = tmp.coord.y > top.y ? tmp.coord : top;
					bottom = tmp.coord.y < bottom.y ? tmp.coord : bottom;
				}
				
			}

			m_numFaces.push_back(faceSize);
			m_numVerts.push_back(faceSize * 3);
		}

	}

	if(bin)
		fclose(bin);

	verts.clear();
	uvs.clear();
	faces.clear();

	return true;
}

bool Mesh::loadMesh(std::string path)
{
	if(!load(path.c_str()))return false;

	init();


	//for(auto& a : m_unpackedData)
	//	a.second.clear();
	m_unpackedData.clear();

	return true;
}

bool Mesh::loadPrimitive(primitiveMesh* mesh)
{
	if(!mesh)
		return false;

	mesh->createMesh();

	m_unpackedData.push_back({"",mesh->getData()});
	m_textures.resize(1, {"", {Texture2D(),Texture2D()}});
	m_replaceTex.resize(1, {{0,0}});

	m_textures[0].second[0].type = TEXTURE_TYPE::DIFFUSE;
	m_textures[0].second[1].type = TEXTURE_TYPE::SPECULAR;

	top = mesh->m_top;
	bottom = mesh->m_bottom;
	left = mesh->m_left;
	right = mesh->m_right;
	front = mesh->m_front;
	back = mesh->m_back;

	m_numVerts.push_back((int)mesh->getData().size());

	init();

	m_unpackedData.clear();

	return true;
}

std::vector< std::pair<std::string, std::vector<Vertex3D>>>& Mesh::loadAni(std::string path)
{
	ani = true;
	static std::vector< std::pair<std::string, std::vector<Vertex3D>>> empty;

	if(!load(path))return empty;

	return m_unpackedData;
}

void Mesh::render(Shader& shader)
{
	shader.enable();
	for(unsigned a = 0; a < m_vaoID.size(); a++)
	{
		bool textured = false;
		int c = 0;

		for(unsigned b = 0; b < m_textures.size(); b++)
		{
			if(m_textures[b].first == m_vaoID[a].first)
			{
				glActiveTexture(GL_TEXTURE0 + c);
				int e = 0;
				for(auto& d : m_textures[b].second)
					if(d.type == TEXTURE_TYPE::DIFFUSE)
						if(d.id|| m_replaceTex[b][e])
						{
							textured = true;
							glUniform1i(shader.getUniformLocation("uTex"), c);
							glBindTexture(GL_TEXTURE_2D, m_replaceTex[b][e] ? m_replaceTex[b][e] : d.id);
							e++;
						}

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

	shader.disable();
}

GLuint Mesh::getNumFaces(int m_index) const
{
	return m_numFaces[m_index];
}

GLuint Mesh::getNumVerticies(int m_index) const
{
	return m_numVerts[m_index];
}

void Mesh::init()
{
	for(unsigned a = 0; a < m_unpackedData.size(); a++)
	{
		m_vaoID.push_back({m_unpackedData[a].first ,0});
		m_vboID.push_back({0 ,0});

		glGenVertexArrays(1, &m_vaoID[a].second);
		glGenBuffers(1, &m_vboID[a].first);
		glGenBuffers(1, &m_vboID[a].second);

		glBindVertexArray(m_vaoID[a].second);


		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].first);
		glBufferData(GL_ARRAY_BUFFER, m_unpackedData[a].second.size() * sizeof(Vertex3D), m_unpackedData[a].second.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		//vertex 1   attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

		//UV         attributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		//normal 1   attributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));


		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].second);
		glBufferData(GL_ARRAY_BUFFER, m_unpackedData[a].second.size() * sizeof(Vertex3D), m_unpackedData[a].second.data(), GL_STATIC_DRAW);

		//vertex 2   attributes
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

		//normal 2   attributes
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::replaceTexture(int mesh, int index, GLuint tex)
{
	m_replaceTex[mesh][index] = tex;
}

void Mesh::editVerts(std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts1, std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts2)
{
	for(unsigned a = 0; a < verts1.size(); a++)
	{

		glBindVertexArray(m_vaoID[a].second);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].first);

		glBufferData(GL_ARRAY_BUFFER, verts1[a].second.size() * sizeof(Vertex3D), verts1[a].second.data(), GL_STATIC_DRAW);

		//vertex 1   attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

		//UV         attributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		//normal 1   attributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));


		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].second);

		glBufferData(GL_ARRAY_BUFFER, verts2[a].second.size() * sizeof(Vertex3D), verts2[a].second.data(), GL_STATIC_DRAW);


		//vertex 2   attributes
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)(offsetof(Vertex3D, coord)));

		//normal 2   attributes
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)(offsetof(Vertex3D, norm)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::unload()
{
	for(unsigned a = 0; a < m_unpackedData.size(); a++)
	{
		if(a < m_vboID.size())
			if(m_vboID[a].first)
			{
				glDeleteBuffers(1, &m_vboID[a].first);
				glDeleteBuffers(1, &m_vboID[a].second);
			}
		if(a < m_vaoID.size())
			if(m_vaoID[a].second)
				glDeleteVertexArrays(1, &m_vaoID[a].second);
	}
	m_vboID.clear();
	m_vaoID.clear();
	m_numVerts.clear();
	m_numFaces.clear();
}