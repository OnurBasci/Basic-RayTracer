#pragma once
#include"Vector3.h"


class PointLight{

public:
	Vector3 position;
	Vector3 color;
	float power;

	PointLight(Vector3 pos, Vector3 color, float power);
	
	void at(Vector3& point, Vector3& direction, Vector3& intensity);
};