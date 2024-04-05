#pragma once
#include"Vector3.h"
#include"Object.h"

class Cylinder : public Object
{
public:
	Vector3 position;
	float radius;

	Cylinder(Vector3 position, float radius);

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) override;
};
