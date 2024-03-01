#pragma once
#include<vector>
#include "Vector3.h"
#include "Ray.h"

struct MaterialParameters {
	float kd; // diffusion coefficient
	float ks; // specularity coefficient

	MaterialParameters() : kd(1), ks(1) {}
	MaterialParameters(float kd, float ks) : kd(kd), ks(ks) {};
};


class Object {
public:
	Vector3 center;
	float radius;
	Vector3 color;
	MaterialParameters m_params;

	Object(Vector3 center, float radius, Vector3 color, MaterialParameters m_params);

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal);
};