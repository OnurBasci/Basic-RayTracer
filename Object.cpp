#include"Object.h"
#include <cmath>
#include<iostream>
#include"Vector3.h"

Object::Object(Vector3 center, float radius, Vector3 color)
{
	this->center = center;
	this->radius = radius;
	this->color = color;
}


bool Object::intersect(const Ray& ray)
{
	//let's calculate the coefficients
	float a = ray.direction * ray.direction;
	float b = 2*(ray.direction * (ray.center - center));
	float c = (ray.center - center) * (ray.center - center) - radius * radius;
	
	float delta = b * b - 4 * a * c;

	if (delta >= 0)
		return true;

	return false;
}