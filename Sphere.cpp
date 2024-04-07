#include"Sphere.h"
#include"Vector3.h"


Sphere::Sphere(Vector3 center, float radius, Vector3 color, MaterialParameters mParams) : Object(center, color, mParams)
{
	this->radius = radius;
}


bool Sphere::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
	//let's calculate the coefficients
	float a = ray.direction * ray.direction;
	float b = 2 * (ray.direction * (ray.center - center));
	float c = (ray.center - center) * (ray.center - center) - radius * radius;

	float delta = b * b - 4 * a * c;

	if (delta >= 0)
	{
		float t = (-b - sqrt(delta)) / (2 * a);

		intersection_point = ray.center + ray.direction * t; //R(t) = O + tD

		normal = (intersection_point - center).normalized(); //spheres normal

		//push the intersection a bit in the direction of the normal
		//intersection_point = intersection_point + normal * 0.1;

		return true;
	}

	return false;
}

bool Sphere::volumeIntersect(const Ray& ray, float& t0, float& t1)
{
	//This function calculates the entrance and exit intersection

	//let's calculate the coefficients
	float a = ray.direction * ray.direction;
	float b = 2 * (ray.direction * (ray.center - center));
	float c = (ray.center - center) * (ray.center - center) - radius * radius;

	float delta = b * b - 4 * a * c;

	if (delta >= 0)
	{
		t0 = (-b - sqrt(delta)) / (2 * a);
		t1 = (-b + sqrt(delta)) / (2 * a);

		return true;
	}

	return false;
}