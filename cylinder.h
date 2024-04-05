#pragma once
#include"Vector3.h"
#include"Object.h"

class Cylinder : public Object
{
public:
	float radius;

	Cylinder(Vector3 center, float radius, Vector3 color, MaterialParameters mParams);

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) override;
};
