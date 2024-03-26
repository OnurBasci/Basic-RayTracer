#pragma once
#include<vector>
#include "Vector3.h"
#include"Object.h"


class Plane : public Object {
public:
	Vector3 planeNormal;

	Plane(Vector3 center, Vector3 planeNormal, Vector3 color, MaterialParameters mParams);

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) override;
};