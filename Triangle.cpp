#include"Triangle.h"
#include"Object.h"
#include"Vector3.h"

Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 color, MaterialParameters mParams) : Object(Vector3(0,0,0), color, mParams)
{
	this->point1 = p1;
	this->point2 = p2;
	this->point3 = p3;
	//define the normal as a normalized cross product of the vector obtained from the triangle points
	Vector3 A = point2 - point1;
	Vector3 B = point3 - point1;
	this->tNormal = A.crossProduct(B).normalized();

    //The center of the triangle is the barycenter
    this->center = (point1 + point2 + point3) / 3;
}

Triangle::Triangle()
{

}


bool Triangle::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
    // Compute the plane's normal
    Vector3 v1v2 = point2 - point1;
    Vector3 v1v3 = point3 - point1;
    // No need to normalize
    Vector3 N = v1v2.crossProduct(v1v3); // N
    float area2 = N.length();

    // Step 1: Finding P

    // Check if the ray and plane are parallel
    float NdotRayDirection = N*ray.direction;
    if (fabs(NdotRayDirection) < 0.0000001) // Almost 0
        return false; // They are parallel, so they don't intersect!

    // Compute d parameter using equation 2
    float d = N*point1*-1;

    // Compute t (equation 3)
    float t = -(N*ray.center + d) / NdotRayDirection;

    // Check if the triangle is behind the ray
    //if (t < 0) return false; // The triangle is behind

    // Compute the intersection point using equation 1
    intersection_point = ray.center + ray.direction*t;

    // Step 2: Inside-Outside Test
    Vector3 C; // Vector perpendicular to triangle's plane

    // Edge 0
    Vector3 edge0 = point2 - point1;
    Vector3 vp0 = intersection_point - point1;
    C = edge0.crossProduct(vp0);
    if (N*C < 0) return false; // P is on the right side

    // Edge 1
    Vector3 edge1 = point3 - point2;
    Vector3 vp1 = intersection_point - point2;
    C = edge1.crossProduct(vp1);
    if (N*C < 0) return false; // P is on the right side

    // Edge 2
    Vector3 edge2 = point1 - point3;
    Vector3 vp2 = intersection_point - point3;
    C = edge2.crossProduct(vp2);
    if (N*C < 0) return false; // P is on the right side

    normal = tNormal;

    return true; // This ray hits the triangle
}

bool Triangle::volumeIntersect(const Ray& ray, float& t0, float& t1)
{
    //this is not a 3d object therefore no volume
    return false;
}