#pragma once
#include<vector>
#include "Vector3.h"
#include"Object.h"
#include"Triangle.h"

//A rectangle is composed of 2 triangles
class Rectangle : public Object {
public:
	Vector3 point1;
	Vector3 point2;
	Vector3 point3;
	Vector3 point4;
	Vector3 tNormal;

	Triangle triangle1;
	Triangle triangle2;

	Rectangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 color, MaterialParameters mParams);

	Rectangle() = default;

	bool intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal) override;
};