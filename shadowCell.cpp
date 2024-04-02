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

	//calculate transmittance function
	//the transmittance start by 1
	surfaceTransmittance[sampleIndex].push_back(1);
	cout << "transmitance function for " << sampleIndex << " [";
	for (depthOpacity elem : depthOpacVector)
	{
		//cout << "opacity " << (1 - elem.opacity) << " ";
		float nextTransmittance = surfaceTransmittance[sampleIndex].back() * (1-elem.opacity);
		surfaceTransmittance[sampleIndex].push_back(nextTransmittance);
		hitDepths[sampleIndex].push_back(elem.depth);
		cout << hitDepths[sampleIndex].back() << " ";
	}
	cout << "]\n";
}

void ShadowCell::CalculateVolumeFunction()
{
	//This function calculates the volume function by averaging transmittance functions

	//Naif implementation merge vectors and sort them
	for (int i = 0; i < sampleNumber-1; i++)
	{
		hitDepthsForvisibility.insert(hitDepthsForvisibility.end(), hitDepths[i+1].begin(), hitDepths[i+1].end());
		visibilityFunction.insert(visibilityFunction.end(), surfaceTransmittance[i + 1].begin(), surfaceTransmittance[i+1].end());
	}

	sort(hitDepthsForvisibility.begin(), hitDepthsForvisibility.end());
	sort(visibilityFunction.begin(), visibilityFunction.end());

	cout << "hit depth for visibility: ";
	for (int i = 0; i < hitDepthsForvisibility.size(); i++)
	{
		cout << hitDepthsForvisibility[i] << " ";
	}
}