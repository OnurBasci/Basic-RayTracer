#define _USE_MATH_DEFINES
#include<cmath>
#include"Vector3.h"
#include"Light.h"


Light::Light(Vector3 position, Vector3 color, float power)
{
	this->position = position;
	this->color = color;
	this->power = power;
}