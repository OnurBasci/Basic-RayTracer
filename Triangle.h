#pragma once
#include<vector>
#include "Vector3.h"
#include"Object.h"


class Triangle : public Object {
public:
	Vector3 point1;
	Vector3 point2;
	Vector3 point3;
	Vector3 tNormal;
	
	Triangle();
	Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 color, MaterialParameters mParams);

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) override;
	bool volumeIntersect(const Ray& ray, float& t0, float& t1) override;
};