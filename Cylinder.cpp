#include<cmath>
#include"Cylinder.h"

using namespace std;

Cylinder::Cylinder(Vector3 pos, float radius) : radius(radius), position(pos) {}


bool Cylinder::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
	//the coefficient of the intersection equation between a ray and a cylinder
	float a = (pow(ray.direction.x, 2) + (pow(ray.direction.y, 2)));
	float b = 2 * (ray.center.x * ray.direction.x + ray.center.y + ray.direction.y);
	float c = pow(ray.center.x, 2) + pow(ray.center.y, 2) - pow(radius, 2);

	float delta = b * b - 4 * a * c;

	if (delta >= 0)
	{
		float t = (-b - sqrt(delta)) / (2 * a);

		intersection_point = ray.center + ray.direction * t; //R(t) = O + tD

		normal = (intersection_point - center).normalized(); //spheres normal

		return true;
	}
	
	return false;
}