#pragma once
#include<vector>
#include<list>
#include "Vector3.h"
#include"Object.h"
#include"Camera.h"
#include"Image.h"

using namespace std;

class Scene {
public:
	list<Object> objects;
	Camera camera;

	Scene(Camera camera, list<Object> objects);
	Scene(list<Object> objects);

	void render(Image& image);
};