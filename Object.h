#pragma once
#include<vector>
#include "Vector3.h"
#include "Ray.h"

struct MaterialParameters {
	float kd; // diffusion coefficient
	float ks; // specularity coefficient
	float opacity;

	MaterialParameters() : kd(1), ks(1), opacity(1) {}
	MaterialParameters(float kd, float ks) : kd(kd), ks(ks), opacity(1) {};
	MaterialParameters(float kd, float ks, float opacity) : kd(kd), ks(ks), opacity(opacity) {};
};

//This is an abstract class that contains commun methods for each object
class Object {
public:
	Vector3 center;
	MaterialParameters m_params;
	Vector3 color;

	Object(Vector3 center, Vector3 color, MaterialParameters m_params);
	Object();

	//an abstract class that depends on the geometry of the object
	virtual bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) = 0;
};