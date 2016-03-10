#ifndef SCENE_H
#define SCENE_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "Color.h"
#include "Light.h"
#include "Material.h"
#include "Vec3.h"
#include "Model.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"

using namespace std;

class Scene
{
	public:	
		int ReflectionCount;
		Color Background;
		Color Ambient;
		vector<Light> Lights;
		vector<Material> Materials;
		vector<string> Textures;
		vector<Model*> Models;
		vector<Translation> Translations;
		vector<Scaling> Scalings;
		vector<Rotation> Rotations;

		Scene(){}
		~Scene(){}
		void printScene();
};

#endif
