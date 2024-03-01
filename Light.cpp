#define _USE_MATH_DEFINES
#include<cmath>
#include"Vector3.h"
#include"PointLight.h"


PointLight::PointLight(Vector3 pos, Vector3 color, float power)
{
	this->position = pos;
	this->color = color;
	this->power = power;
}

void PointLight::at(Vector3& point, Vector3& direction, Vector3& intensity)
{
	direction = point - direction;
	float distance = direction.length();
	float intens_coeff = power / (4 * M_PI * distance * distance);
	intensity = Vector3(intens_coeff * color.x, intens_coeff * color.y, intens_coeff * color.z);
}