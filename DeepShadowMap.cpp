#include"DeepShadowMap.h"
#include"ShadowCell.h"

DeepShadowMap::DeepShadowMap(list<Object*> objects,  double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, \
    float v_width, float v_height, float mapRes): focal_length(_focal_length), position(_position), target_point(_target_point), up_vector(_up_vector),
    viewport_height(v_height), viewport_width(v_width), mapResolution(mapRes)
{
    this->objects = objects;

    up = _up_vector.normalized();
    forward = (target_point - position).normalized();
    right = forward.crossProduct(up_vector).normalized() * -1;

    Vector3 viewport_u = right * viewport_width;
    Vector3 viewport_v = up * -viewport_height; //viewport height is negatif since we go from top to down

    pixel_delta_u = viewport_u / mapResolution;
    pixel_delta_v = viewport_v / mapResolution;

    Vector3 viewport_upper_left = _position + forward * focal_length
        - viewport_u / 2 - viewport_v / 2;

    celll00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    //intialize shadow cells
    for (int i = 0; i < mapResolution; i ++)
    {
        for (int j = 0; j < mapResolution; j ++)
        {
            ShadowCell newCell(i, j, 4, this);
            shadowCells.push_back(newCell);
        }
    }

    //Calculate Volume functions for each of the shadowCells
    for (int i = 0; i < mapResolution * mapResolution; i++)
    {
        shadowCells[i].CalculateVisibilityFunction(); 
    }

    //Calculate a virtual rectangle object to help us find the intersection on the shadow map
    Vector3 p0 = viewport_upper_left;
    Vector3 p1 = viewport_upper_left + viewport_u;
    Vector3 p2 = viewport_upper_left + viewport_v;

    mapRectangle = Rectangle(p0, p1, p2, Vector3(0,0,0), MaterialParameters());
}


float DeepShadowMap::getVisibilityFromWorldPos(Vector3 WorldPos)
{
    //get the intersection position on the rectangle
    Ray rayToMap(WorldPos, position - WorldPos);
    Vector3 intersectionPoint, normal;
    mapRectangle.intersect(rayToMap, intersectionPoint, normal);

    cout << "Intersection Point : " << intersectionPoint << "\n";

    //get the local coordinates with a projection
    Vector3 edge1 = mapRectangle.point2 - mapRectangle.point1;
    Vector3 edge2 = mapRectangle.point3 - mapRectangle.point1;
    Vector3 interPointDir = intersectionPoint - mapRectangle.point1;

    float localCoordinateX = (interPointDir * edge1) / (edge1 * edge1);
    float localCoordinateY = (interPointDir * edge2) / (edge2 * edge2);

    cout << "local coordinates (" << localCoordinateX << ", " << localCoordinateY << ")\n";

    //get cell Index
    int cellIndexI = localCoordinateX * mapResolution;
    int cellIndexJ = localCoordinateY * mapResolution;
    cout << "cell coordinates (" << cellIndexI << ", " << cellIndexJ << ")\n";
    
    return 0;
}