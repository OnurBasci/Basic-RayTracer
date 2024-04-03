#include"ShadowCell.h"
#include"Vector3.h"
#include"DeepShadowMap.h"
#include<vector>
#include <algorithm>

ShadowCell::ShadowCell(int index_i, int index_j, int sampleNumber, DeepShadowMap* belongingShadowMap)
{
	this->i = index_i;
	this->j = index_j;
	this->sampleNumber = sampleNumber;
	this->hitDepths = vector<vector<float>>(sampleNumber);
	//this->hitDepths = new float*[sampleNumber];
	this->surfaceTransmittance = vector<vector<float>>(sampleNumber);
	this->belongingShadowMap = belongingShadowMap;
}

void ShadowCell::CalculateVisibilityFunction()
{
	//This function calculates the volume function by averaging transmittance functions

	//calculate surface transmittance functions
	CalculateSurfaceTransmittanceFunctions();

	//merge and sort the depth of all transmittance functions
	for (int i = 0; i < sampleNumber; i++)
	{
		hitDepthsForvisibility.insert(hitDepthsForvisibility.end(), hitDepths[i].begin(), hitDepths[i].end());
	}
	sort(hitDepthsForvisibility.begin(), hitDepthsForvisibility.end());

	//calculate transmittance for each of the depth by averaging surface transmittance value (to normalize)
	float transmittance;
	for (float depth : hitDepthsForvisibility)
	{
		transmittance = 0;
		for (int functionIdex = 0; functionIdex < sampleNumber; functionIdex++)
		{
			transmittance += getSurfaceTransmittanceValue(functionIdex, depth);
		}
		visibilityFunction.push_back(transmittance / sampleNumber);
	}


	/*
	cout << "visibility function: [ ";
	for (int i = 0; i < visibilityFunction.size(); i++)
	{
		cout << " d: " << hitDepthsForvisibility[i] << " t : " << visibilityFunction[i];
	}
	cout << "]\n";
	*/ 
}

void ShadowCell::CalculateSurfaceTransmittanceFunctions()
{
	//This function sample random positions on the cell and calculate the transmittance function for all of the points

	for (int sampleIndex = 0; sampleIndex < sampleNumber; sampleIndex++)
	{
		//get a random position in local coordinates
		Vector3 LocalSamplePos(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, 0);

		//get world coordinates
		Vector3 WorldSamplePos = belongingShadowMap->celll00_loc + belongingShadowMap->pixel_delta_u * (j + LocalSamplePos.x) \
			+ belongingShadowMap->pixel_delta_v * (i + LocalSamplePos.y);

		//Create a ray between the light origin and the sample point
		Vector3 RayDirection = (WorldSamplePos - belongingShadowMap->position).normalized();

		Ray sampleRay(belongingShadowMap->position, RayDirection);

		CalculateSurfaceTransmittanceFunctionFromARay(sampleIndex, sampleRay, belongingShadowMap->objects);
	}
}

struct depthOpacity {
	float depth;
	float opacity;

	depthOpacity(float depth, float opac): depth(depth), opacity(opac) {}
};

void ShadowCell::CalculateSurfaceTransmittanceFunctionFromARay(int sampleIndex, Ray ray, list<Object*> objects)
{
	Vector3 intersectionPoint, normal;

	//deine a float2 vector to stock depth and corrsponding objects obacity to later sort them by the depth and calculate transmittance
	vector<depthOpacity> depthOpacVector;

	//Get the transmittance and opacity values
	for (Object* obj : objects)
	{
		if (obj->intersect(ray, intersectionPoint, normal))
		{
			float depth = (intersectionPoint - ray.center).length();
			//update the transmittance
			depthOpacVector.push_back(depthOpacity(depth, obj->m_params.opacity));
		}
	}
	//sort the depth opacity vector by the depth
	sort(depthOpacVector.begin(), depthOpacVector.end(), [](const depthOpacity& a, const depthOpacity& b)
		{ return a.depth < b.depth;});

	//calculate transmittance function. Every hit creates 2 verticies, discontinuous function
	//the transmittance start by 1
	surfaceTransmittance[sampleIndex].push_back(1);
	hitDepths[sampleIndex].push_back(0);

	float epsilon = 1e-6;
	for (depthOpacity elem : depthOpacVector)
	{
		float lastElem = surfaceTransmittance[sampleIndex].back();
		//add the old transmittance, discontinuous function
		surfaceTransmittance[sampleIndex].push_back(lastElem);
		hitDepths[sampleIndex].push_back(elem.depth);
		//add the new value
		float nextTransmittance = lastElem * (1-elem.opacity);
		surfaceTransmittance[sampleIndex].push_back(nextTransmittance);
		hitDepths[sampleIndex].push_back(elem.depth+epsilon);
	}

	/*
	cout << "trans for " << sampleIndex << "[ ";
	for (int i = 0 ; i < surfaceTransmittance[sampleIndex].size(); i ++)
	{
		cout << " d: " << hitDepths[sampleIndex][i] << " t : " << surfaceTransmittance[sampleIndex][i];
	}
	cout << "\n \n";
	*/
}

float ShadowCell::getSurfaceTransmittanceValue(int functionIndex, float depth)
{
	//this function calculates the surface transmittance value of a surface transmittance function for a given depth
	//the value is equal to the surface transmittance value of the last vertex

	int depthIndex = 0;
	float currentDepth;
	for (int i = 0; i < hitDepths[functionIndex].size(); i++)
	{
		currentDepth = hitDepths[functionIndex][i];

		if (depth < currentDepth) break;

		depthIndex++;
	} 
	//depth is higher than the final depth than return the last transmittance
	if (depthIndex >= hitDepths[functionIndex].size() - 1) return surfaceTransmittance[functionIndex].back();
	//if depth is lower than the first depth return 1 
	if (depthIndex <= 0) return 1;

	return surfaceTransmittance[functionIndex][depthIndex-1];

	/*
	//linear interpolatio
	float lastTransmittance = surfaceTransmittance[functionIndex][depthIndex-1];
	float nextTransmittance = surfaceTransmittance[functionIndex][depthIndex];
	float distNormalized = (depth - hitDepths[functionIndex][depthIndex-1])/(hitDepths[functionIndex][depthIndex] - hitDepths[functionIndex][depthIndex-1]);

	return lastTransmittance * (1 - distNormalized) + nextTransmittance * distNormalized;
	*/
}

float ShadowCell::getVisibility(float depth)
{
	//this function returns the visibility value from a given depth
	//the value is calculated by a linear interpolation
	int depthIndex = 0;
	float currentDepth;
	for (int i = 0; i < hitDepthsForvisibility.size(); i++)
	{
		currentDepth = hitDepthsForvisibility[i];

		if (depth < currentDepth) break;

		depthIndex++;
	}
	//depth is higher than the final depth than return the last transmittance
	if (depthIndex >= hitDepthsForvisibility.size() - 1) return visibilityFunction.back();
	//if depth is lower than the first depth return 1 
	if (depthIndex <= 0) return 1;

	//Calculate a linear interpolation between the last and the next vertices
	float lastVisibility = visibilityFunction[depthIndex-1];
	float nextVisibility = visibilityFunction[depthIndex];;
	float distNormalized = (depth - hitDepthsForvisibility[depthIndex-1]) / (hitDepthsForvisibility[depthIndex] - hitDepthsForvisibility[depthIndex-1]);

	return lastVisibility * (1 - distNormalized) + nextVisibility * distNormalized;
}


