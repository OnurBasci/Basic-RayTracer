#pragma once
#include<vector>
#include "Vector3.h"
#include "Ray.h"

struct MaterialParameters {
	float kd; // diffusion coefficient
	float ks; // specularity coefficient
	float opacity;
	float sigma_a; //for volumetric objects

	MaterialParameters() : kd(1), ks(1), opacity(1), sigma_a(1) {}
	MaterialParameters(float kd, float ks) : kd(kd), ks(ks), opacity(1), sigma_a(1) {};
	MaterialParameters(float kd, float ks, float opacity) : kd(kd), ks(ks), opacity(opacity), sigma_a(1) {};
	MaterialParameters(float kd, float ks, float opacity, float sigma_a) : kd(kd), ks(ks), opacity(opacity), sigma_a(sigma_a) {};
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