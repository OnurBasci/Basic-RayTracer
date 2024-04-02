#include"ShadowCell.h"
#include"Vector3.h"
#include<vector>
#include <algorithm>
#include"DeepShadowMap.h"

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
		Vector3 WorldSamplePos = belongingShadowMap->celll00_loc + belongingShadowMap->pixel_delta_u * (i + LocalSamplePos.x) \
			+ belongingShadowMap->pixel_delta_v * (j + LocalSamplePos.y);

		//Create a ray between the light origin and the sample point
		Vector3 RayDirection = (WorldSamplePos - belongingShadowMap->position).normalized();
		//Vector3 RayDirection(-1, 0, 0);

		Ray sampleRay(belongingShadowMap->position, RayDirection);

		CalculateSurfaceTransmittanceFunctionFromARay(sampleIndex, sampleRay, belongingShadowMap->objects);
	}
}

struct depthOpacity {
	float depth;
	float opacity;

	depthOpacity(float depth, float opac)
	{
		depth = depth;
		opacity = opac;
	}
};

void ShadowCell::CalculateSurfaceTransmittanceFunctionFromARay(int sampleIndex, Ray ray, list<Object*> objects)
{
	Vector3 intersectionPoint, normal;

	//deine a float2 vector to stock depth and corrsponding objects obacity to later sort them by the depth and calculate transmittance
	vector<depthOpacity> depthOpacVector;
	std::cout << "Index is " << sampleIndex << "\n";
	std::cout << "Origin is " << ray.center << "\n";
	std::cout << "Direction is " << ray.direction << "\n";
	//find all of the intersections and stock the depth and opacity informations
	for (Object* obj : objects)
	{
		std::cout << "object light dir " << (obj->center - ray.center).normalized() << "\n";
		if (obj->intersect(ray, intersectionPoint, normal))
		{
			std::cout << "there is a hit";
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
	for (depthOpacity elem : depthOpacVector)
	{
		float nextTransmittance = surfaceTransmittance[sampleIndex].back() * (1-elem.opacity);
		surfaceTransmittance[sampleIndex].push_back(nextTransmittance);
		hitDepths[sampleIndex].push_back(elem.depth);
	}
}

ShadowCell::~ShadowCell() {
	delete[] visibilityFunction;
}