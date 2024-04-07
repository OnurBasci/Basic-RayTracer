#include"Plane.h"
#include"Vector3.h"

Plane::Plane(Vector3 center, Vector3 planeNormal, Vector3 color, MaterialParameters mParams) : Object(center, color, mParams)
{
	this->planeNormal = planeNormal;
}


bool Plane::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
	float t = ((ray.center.x + center.x) * planeNormal.x + (ray.center.y + center.y) * planeNormal.y + (ray.center.z + center.z) * planeNormal.z)
		/(planeNormal.x*ray.direction.x + planeNormal.y*ray.direction.y + planeNormal.z*ray.direction.z);

	intersection_point = ray.center + ray.direction * t; //R(t) = O + tD

	normal = planeNormal.normalized();

	return false;
}

bool Plane::volumeIntersect(const Ray& ray, float& t0, float& t1)
{
	//Not a 3d object therefore no volume
	return false;
}