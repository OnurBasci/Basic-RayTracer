#pragma once
#include"Vector3.h"
#include"Light.h"

class PointLight : public Light{

public:
	PointLight(Vector3 pos, Vector3 color, float power);
	
	void at(Vector3& point, Vector3& direction, Vector3& intensity) override;
};