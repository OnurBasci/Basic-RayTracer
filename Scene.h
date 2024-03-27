#pragma once
#include<vector>
#include<list>
#include "Vector3.h"
#include"Object.h"
#include"Camera.h"
#include"Image.h"
#include"PointLight.h"

using namespace std;

class Scene {
public:
	list<Object*> objects;
	list<Light*> pointLights;
	Camera camera;

	Scene(Camera camera, list<Object*> objects, list<Light*> ligths);
	Scene(list<Object*> objects, list<Light*> lights);

	void render(Image& image);
};