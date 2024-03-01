#pragma once
#include<vector>
#include "Vector3.h"
#include "Ray.h"

struct MaterialParameters {
	float kd; // Coefficient de diffusion
	float ks; // Coefficient de spécularité

};


class Object {
public:
	Vector3 center;
	float radius;
	Vector3 color;

	Object(Vector3 center, float radius, Vector3 color);

	bool intersect(const Ray& ray);
};