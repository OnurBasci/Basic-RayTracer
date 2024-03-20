#pragma once
#include<vector>
#include "Vector3.h"

class Ray {

public:
	Vector3 center;
	Vector3 direction;

	Ray(Vector3 center, Vector3 direction);
};