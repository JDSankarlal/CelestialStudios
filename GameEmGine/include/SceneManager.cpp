#include "SceneManager.h"
#include "GameEmGine.h"
#include <fstream>
using std::vector;
using std::ofstream;
using std::ifstream;
using std::ios;

void SceneManager::createScene(cstring file, vector<Model*> models)
{

	ofstream out(file, ios::binary);

	//amount of models

	//looping data
		//amount of meshes

	out.close();

}

void SceneManager::loadScene()
{}
