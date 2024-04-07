#include"Rectangle.h"
#include"Object.h"
#include"Vector3.h"

Rectangle::Rectangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 color, MaterialParameters mParams) : Object(Vector3(0, 0, 0), color, mParams)
{
    this->point1 = p1;
    this->point2 = p2;
    this->point3 = p3;
    Vector3 A = p2 - p1;
    Vector3 B = p3 - p1;
    this->point4 = p1 + A + B;
    //define the normal as a normalized cross product of the vector obtained from the triangle points
    //p1 and p4 is not commun
    this->triangle1 = Triangle(p1, p2, p3, color, mParams);
    this->triangle2 = Triangle(point4, p3, p2, color, mParams);
    this->tNormal = triangle1.tNormal;

    //The center of the triangle is the barycenter
    this->center = (point1 + point2 + point3 + point4) / 4;
}

bool Rectangle::intersect(const Ray& ray, Vector3& intersection_point, Vector3& normal)
{
    if (triangle1.intersect(ray, intersection_point, normal))
    {
        return true;
    }
    else if (triangle2.intersect(ray, intersection_point, normal))
    {
        return true;
    }
    return false;
}

bool Rectangle::volumeIntersect(const Ray& ray, float& t0, float& t1)
{
    //TO DO
    return false;
}