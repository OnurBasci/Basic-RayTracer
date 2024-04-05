#pragma once

//This class is an helper class to create diffrent scenes you can just call the function you want to render the coresponding scene
class SceneSetUp
{
public:
	float numPixelX, numPixelY;
	float deepShadowMapRes, deepShadowMapSample;
	bool useDeepShadowMap;

	SceneSetUp();
	SceneSetUp(float numPx, float numPy, float dsmRes, float dsmSampleNum, bool useDsm);
	~SceneSetUp() = default;

	void render3BoxSceneSetUp();
};
