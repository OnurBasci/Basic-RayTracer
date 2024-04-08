#pragma once
#include<vector>
#include <functional>
#include "Vector3.h"
#include"MahtHelper.h"
#include "Ray.h"

struct MaterialParameters {
	float kd; // diffusion coefficient
	float ks; // specularity coefficient
	float opacity; //for the surface transmittance function
	float sigma_a; //for volumetric objects
	float extinction_coefficient; //for volume transmittance function
	std::function<float(Vector3 pos)> density; //density function that gives the density from a 3d point in space

	MaterialParameters() : kd(1), ks(1), opacity(1), sigma_a(1), extinction_coefficient(1) {}
	MaterialParameters(float kd, float ks) : kd(kd), ks(ks), opacity(1), sigma_a(1), extinction_coefficient(1) {};
	MaterialParameters(float kd, float ks, float opacity) : kd(kd), ks(ks), opacity(opacity), sigma_a(1), extinction_coefficient(1) {};
	MaterialParameters(float kd, float ks, float opacity, float sigma_a) : kd(kd), ks(ks), opacity(opacity), sigma_a(sigma_a), extinction_coefficient(1) {};
	MaterialParameters(float kd, float ks, float opacity, float sigma_a, float extinction_coef, std::function<float(Vector3 pos)> density) :
		 kd(kd), ks(ks), opacity(opacity), sigma_a(sigma_a), extinction_coefficient(extinction_coef), density(density) {};
};

//This is an abstract class that contains commun methods for each object
class Object {
public:
	Vector3 center;
	MaterialParameters m_params;
	Vector3 color; //used also as scatter value for volumetric object
	int id = -1;
	bool is_volumetric_object;

	Object(Vector3 center, Vector3 color, MaterialParameters m_params);
	Object(Vector3 center, Vector3 color, MaterialParameters m_params, bool is_volumetric_object);
	Object();

	//an abstract class that depends on the geometry of the object
	virtual bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) = 0;
	virtual bool volumeIntersect(const Ray& ray, float& t0, float& t1) = 0; //calculates the 2 intersection point enterance and exit 
};