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
	this->hitDepthsVolume = vector<vector<float>>(sampleNumber);
	this->hitDepthsTransmittance = vector<vector<float>>(sampleNumber);
	this->volumeTransmittance = vector<vector<float>>(sampleNumber);
	this->transmittance = vector<vector<float>>(sampleNumber);
	this->surfaceTransmittance = vector<vector<float>>(sampleNumber);
	this->volumeTransmittance = vector<vector<float>>(sampleNumber);
	this->belongingShadowMap = belongingShadowMap;
}

void ShadowCell::CalculateVisibilityFunction()
{
	//This function calculates the visibility function by averaging (same weight) transmittance functions 

	//calculate surface transmittance functions
	CalculateTransmittanceFunctions();

	//merge and sort the depth of all transmittance functions
	for (int i = 0; i < sampleNumber; i++)
	{
		hitDepthsForvisibility.insert(hitDepthsForvisibility.end(), hitDepthsTransmittance[i].begin(), hitDepthsTransmittance[i].end());
	}
	sort(hitDepthsForvisibility.begin(), hitDepthsForvisibility.end());

	//calculate transmittance for each of the depth by averaging surface transmittance value (to normalize)
	float transmittance; 
	for (float depth : hitDepthsForvisibility)
	{ 
		transmittance = 0;
		for (int functionIdex = 0; functionIdex < sampleNumber; functionIdex++)
		{
			//transmittance += getSurfaceTransmittanceValue(functionIdex, depth);
			transmittance += getTransmittanceValue(functionIdex, depth);
		}
		visibilityFunction.push_back(transmittance / sampleNumber);
	}
	
	/*
	cout << "visibility function of index(" << i << ", " << j << "): [";
	for (int i = 0; i < visibilityFunction.size(); i++)
	{
		cout << " d: " << hitDepthsForvisibility[i] << " t : " << visibilityFunction[i];
	}
	cout << "]\n";
	*/
	
}

void ShadowCell::CalculateTransmittanceFunctions()
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
		CalculateVolumeTransmittanceFunctionFromARay(sampleIndex, sampleRay, belongingShadowMap->objects);

		//calculate transmittance function by mutliplying volume function with the surface function t = ts * tv
		int surfaceIndex = 0, volumeIndex = 0;
		while (surfaceIndex < surfaceTransmittance[sampleIndex].size() && volumeIndex < volumeTransmittance[sampleIndex].size())
		{
			float surface_depth = hitDepths[sampleIndex][surfaceIndex];
			float volume_depth = hitDepthsVolume[sampleIndex][volumeIndex];
			if (surface_depth < volume_depth)
			{
				transmittance[sampleIndex].push_back(getSurfaceTransmittanceValue(sampleIndex, surfaceIndex) * getVolumeTransmittanceValue(sampleIndex, surface_depth));
				hitDepthsTransmittance[sampleIndex].push_back(surface_depth);
				surfaceIndex++;
			}
			else 
			{
				transmittance[sampleIndex].push_back(getSurfaceTransmittanceValue(sampleIndex, volume_depth) * getVolumeTransmittanceValue(sampleIndex, volumeIndex));
				hitDepthsTransmittance[sampleIndex].push_back(volume_depth);
				volumeIndex++;
			}
		}
		//fill the rest with the longer transmitance vector
		if (surfaceIndex >= surfaceTransmittance[sampleIndex].size())
		{
			for (int i = 0; i < volumeTransmittance[sampleIndex].size() - volumeIndex; i++)
			{
				transmittance[sampleIndex].push_back(getSurfaceTransmittanceValue(sampleIndex, surfaceIndex) * getVolumeTransmittanceValue(sampleIndex, volumeIndex + i));
				hitDepthsTransmittance[sampleIndex].push_back(hitDepthsVolume[sampleIndex][volumeIndex + i]);
			}
		}
		else if(volumeIndex >= volumeTransmittance[sampleIndex].size())
		{
			for (int i = 0; i < surfaceTransmittance[sampleIndex].size() - surfaceIndex; i++)
			{
				transmittance[sampleIndex].push_back(getSurfaceTransmittanceValue(sampleIndex, surfaceIndex + i) * getVolumeTransmittanceValue(sampleIndex, volumeIndex));
				hitDepthsTransmittance[sampleIndex].push_back(hitDepths[sampleIndex][surfaceIndex + i]);
			}
		}
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

	//define a float2 vector to stock depth and corrsponding objects obacity to later sort them by the depth and calculate transmittance
	vector<depthOpacity> depthOpacVector;

	//Get the transmittance and opacity values
	for (Object* obj : objects)
	{
		if (!obj->is_volumetric_object && obj->intersect(ray, intersectionPoint, normal))
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

struct depthExtinction {
	float depth;
	float extinction;

	depthExtinction(float depth, float extinction) : depth(depth), extinction(extinction) {}
};

void ShadowCell::CalculateVolumeTransmittanceFunctionFromARay(int sampleIndex, Ray ray, list<Object*> objects)
{
	//define a float2 vector to stock depth and corrsponding objects extinction to later sort them by the depth and calculate transmittance
	vector<depthExtinction> depthExtinctionVector;

	//Get the transmittance and opacity values
	float t0, t1;
	for (Object* obj : objects)
	{
		if (obj->is_volumetric_object && obj->volumeIntersect(ray, t0, t1))
		{
			//get the correct step size 
			int num_sample = std::ceil((t1 - t0) / step_size);
			step_size = (t1 - t0)/num_sample;
			
			//Ray marching to find the extinction values
			for (int i = 0; i < num_sample; i++)
			{
				float t = t0 + step_size * (i + 0.5);
				Vector3 sample = ray.center + ray.direction * t;

				float depth = (sample - ray.center).length();
				float extinction = obj->m_params.extinction_coefficient * obj->m_params.density(sample);

				depthExtinctionVector.push_back(depthExtinction(depth, extinction));
			}
		}
	}

	//sort the depth extinction vector by the depth
	sort(depthExtinctionVector.begin(), depthExtinctionVector.end(), [](const depthExtinction& a, const depthExtinction& b)
		{ return a.depth < b.depth; });

	//calculate transmittance function. Every hit creates 2 verticies, discontinuous function
	//the transmittance start by 1
	volumeTransmittance[sampleIndex].push_back(1);
	hitDepthsVolume[sampleIndex].push_back(0);

	if (depthExtinctionVector.empty()) return;

	//calculate volume transmitance function
	float transmittance = 1; 
	for (int i = 0; i < depthExtinctionVector.size()-1; i++)
	{
		float ti = exp(-(depthExtinctionVector[i + 1].depth - depthExtinctionVector[i].depth) * (depthExtinctionVector[i + 1].extinction + depthExtinctionVector[i].extinction)/2);
		transmittance *= ti;
		volumeTransmittance[sampleIndex].push_back(transmittance);
		hitDepthsVolume[sampleIndex].push_back(depthExtinctionVector[i].depth);
	}
}

float ShadowCell::getTransmittanceValue(int functionIndex, float depth)
{
	//this function calculates the transmittance value of a transmittance function for a given depth
	//the value is calculated by a linear interpolation between the last and current vertex

	int depthIndex = 0;
	float currentDepth;
	for (int i = 0; i < hitDepthsTransmittance[functionIndex].size(); i++)
	{
		currentDepth = hitDepthsTransmittance[functionIndex][i];

		if (depth < currentDepth) break;

		depthIndex++;
	}
	//depth is higher than the final depth than return the last transmittance
	if (depthIndex >= hitDepthsTransmittance[functionIndex].size() - 1) return transmittance[functionIndex].back();
	//if depth is lower than the first depth return 1 
	if (depthIndex <= 0) return 1;

	//linear interpolation
	float lastTransmittance = transmittance[functionIndex][depthIndex - 1];
	float nextTransmittance = transmittance[functionIndex][depthIndex];
	float distNormalized = (depth - hitDepthsTransmittance[functionIndex][depthIndex - 1]) / (hitDepthsTransmittance[functionIndex][depthIndex] - hitDepthsTransmittance[functionIndex][depthIndex - 1]);

	return lastTransmittance * (1 - distNormalized) + nextTransmittance * distNormalized;
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
}



float ShadowCell::getSurfaceTransmittanceValue(int functionIndex, int depthIndex)
{
	//depth is higher than the final depth than return the last transmittance
	if (depthIndex >= hitDepths[functionIndex].size() - 1) return surfaceTransmittance[functionIndex].back();
	//if depth is lower than the first depth return 1 
	if (depthIndex <= 0) return 1;

	return surfaceTransmittance[functionIndex][depthIndex - 1];
}

float ShadowCell::getVolumeTransmittanceValue(int functionIndex, float depth)
{
	//this function calculates the volume transmittance value of a volume transmittance function for a given depth
	//the value is calculated by a linear interpolation between the last and current vertex

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

	float lastVolumeTransmittance = volumeTransmittance[functionIndex][depthIndex - 1];
	float nextVolumeTransmittance = volumeTransmittance[functionIndex][depthIndex];
	float distNormalized = (depth - hitDepthsVolume[functionIndex][depthIndex - 1]) / (hitDepthsVolume[functionIndex][depthIndex] - hitDepthsVolume[functionIndex][depthIndex - 1]);

	return lastVolumeTransmittance * (1 - distNormalized) + nextVolumeTransmittance * distNormalized;
}

float ShadowCell::getVolumeTransmittanceValue(int functionIndex, int depthIndex)
{
	//depth is higher than the final depth than return the last transmittance
	if (depthIndex >= hitDepthsVolume[functionIndex].size() - 1) return volumeTransmittance[functionIndex].back();
	//if depth is lower than the first depth return 1 
	if (depthIndex <= 0) return 1;

	return volumeTransmittance[functionIndex][depthIndex];
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


