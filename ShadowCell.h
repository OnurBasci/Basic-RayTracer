#pragma once
#include<vector>
#include<list>
#include"Vector3.h"
#include"Ray.h"
#include"Object.h"

using namespace std;

//forward declaration
class DeepShadowMap;

//This class represent a cell of the shadow map. It contains the transmittance functions and visibility function.
class ShadowCell
{
public:
	int i, j; //the index in the shadow map
	int sampleNumber; //the number of ray to send
	int functionSize; //the number of element that surface transmittance function can have
	vector<vector<float>> hitDepths; //Data structure containing the depth value for each of the sample
	vector<vector<float>> surfaceTransmittance;
	vector<float> visibilityFunction; //the function that contains the visibilty information
	vector<float> hitDepthsForvisibility; //Data structure containing the depth value for each of the sample of each transmittance functions
	DeepShadowMap* belongingShadowMap;

	ShadowCell(int index_i, int index_j, int sampleNumber, DeepShadowMap* belongingShadowMap);

	void CalculateSurfaceTransmittanceFunctions();
	void CalculateSurfaceTransmittanceFunctionFromARay(int sampleIndex, Ray ray, list<Object*> objects);
	
	float getSurfaceTransmittanceValue(int functionIndex, float depth);

	float getVisibility(float depth); 

	void CalculateVisibilityFunction();
};