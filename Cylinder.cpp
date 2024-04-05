#include<cmath>
#include"Cylinder.h"

using namespace std;

Cylinder::Cylinder(Vector3 center, float radius, Vector3 color, MaterialParameters mParams) : Object(center, color, mParams)
{
	this->radius = radius;
}


bool Cylinder::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
	//the coefficient of the intersection equation between a ray and a cylinder
	float a = pow(ray.direction.x, 2) + (pow(ray.direction.z, 2));
	float b = 2 * (ray.center.x * ray.direction.x + ray.center.z * ray.direction.z - ray.direction.x*center.x - ray.direction.z*center.z);
	float c = pow(ray.center.x, 2) + pow(ray.center.z, 2) - pow(radius, 2) - 2*ray.center.x*center.x - 2*ray.center.z*center.z + pow(center.x,2) + pow(center.z,2);

	float delta = b * b - 4 * a * c;

	if (delta >= 0)
	{
		float t = (-b - sqrt(delta)) / (2 * a);

		intersection_point = ray.center + ray.direction * t; //R(t) = O + tD

		normal = (intersection_point - center);
		normal.y = 0;
		normal = normal.normalized();
	

		return true;
	}
	
	return false;
}