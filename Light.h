#pragma once
#include"Vector3.h"

class Light {

public:
	Vector3 position;
	Vector3 color;
	float power;

	Light(Vector3 position, Vector3 color, float power);

	virtual void at(Vector3& point, Vector3& direction, Vector3& intensity) = 0;
};