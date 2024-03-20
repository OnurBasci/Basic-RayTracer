#include"Object.h"
#include <cmath>
#include<iostream>
#include"Vector3.h"

Object::Object(Vector3 center, float radius, Vector3 color, MaterialParameters m_params)
{
	this->center = center;
	this->radius = radius;
	this->color = color;
	this->m_params = m_params;
}


bool Object::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
	//let's calculate the coefficients
	float a = ray.direction * ray.direction;
	float b = 2*(ray.direction * (ray.center - center));
	float c = (ray.center - center) * (ray.center - center) - radius * radius;
	
	float delta = b * b - 4 * a * c;

	if (delta >= 0)
	{
		float t = (-b - sqrt(delta)) / (2 * a);

		intersection_point = ray.center + ray.direction*t; //R(t) = O + tD
		
		normal = (intersection_point - center).normalized(); //spheres normal

		return true;
	}

	return false;
}