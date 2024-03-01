#include"Ray.h"
#include<iostream>
#include"Vector3.h"

Ray::Ray(Vector3 center, Vector3 direction)
{
	this->center = center;
	this->direction = direction;
}