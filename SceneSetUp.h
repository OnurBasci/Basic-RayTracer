#pragma once
#include<list>
#include"Object.h"
#include"Light.h"

using namespace std;

//This class is an helper class to create diffrent scenes you can just call the function you want to render the coresponding scene
class SceneSetUp
{
public:
	float numPixelX, numPixelY;
	float deepShadowMapRes, deepShadowMapSample;
	bool useDeepShadowMap;
	list<Object*> objects;
	list<Light*> lights;

	SceneSetUp();
	SceneSetUp(float numPx, float numPy, float dsmRes, float dsmSampleNum, bool useDsm);
	~SceneSetUp();

	void render3BoxSceneSetUp();
	void renderCylinderScene();
	void volumetricObjectTestScene();
};
