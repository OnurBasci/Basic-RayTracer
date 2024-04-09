#include"DeepShadowMap.h"
#include"ShadowCell.h"
#include"MahtHelper.h"
#include <chrono>


DeepShadowMap::DeepShadowMap(list<Object*> objects,  double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, \
    float v_width, float v_height, float mapRes, float sampleNum): focal_length(_focal_length), position(_position), target_point(_target_point), up_vector(_up_vector),
    viewport_height(v_height), viewport_width(v_width), mapResolution(mapRes), samplePerCell(sampleNum)
{
    cout << "Deep shadow map Initilaization:";
    auto start = std::chrono::steady_clock::now(); 

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
            //cout << "(" << i << ", " << j << ") " << "\n";
            ShadowCell newCell(i, j, samplePerCell, this);
            shadowCells.push_back(newCell);
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    cout << "Visibility function calculation :";
    start = std::chrono::steady_clock::now();

    //Calculate Volume functions for each of the shadowCells
    for (int i = 0; i < mapResolution * mapResolution; i++)
    {
        //cout << " current pixel:" << i;
        shadowCells[i].CalculateVisibilityFunction(); 
    }

    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    //Calculate a virtual rectangle object to help us find the intersection on the shadow map
    Vector3 p0 = viewport_upper_left;
    Vector3 p1 = viewport_upper_left + viewport_u;
    Vector3 p2 = viewport_upper_left + viewport_v;

    mapRectangle = Rectangle(p0, p1, p2, Vector3(0,0,0), MaterialParameters());
}


float DeepShadowMap::getVisibilityFromWorldPos(Vector3 WorldPos)
{
    //this function returns the visibility value of a point in the worldspace

    //get the intersection position on the rectangle
    Ray rayToMap(WorldPos, position - WorldPos);
    Vector3 intersectionPoint, normal;
    mapRectangle.intersect(rayToMap, intersectionPoint, normal);

    //cout << "Intersection Point : " << intersectionPoint << "\n";

    //get the local coordinates with a projection
    Vector3 edge1 = mapRectangle.point2 - mapRectangle.point1;
    Vector3 edge2 = mapRectangle.point3 - mapRectangle.point1;
    Vector3 interPointDir = intersectionPoint - mapRectangle.point1;

    float localCoordinateX = (interPointDir * edge1) / (edge1 * edge1);
    float localCoordinateY = (interPointDir * edge2) / (edge2 * edge2);

    //cout << "local coordinates (" << localCoordinateX << ", " << localCoordinateY << ")\n";

    //get cell Index
    int cellIndexI = localCoordinateX * mapResolution;
    int cellIndexJ = localCoordinateY * mapResolution;

    //check if the cell coordinates are in the map resolution
    //if the index is not in the range than the object is not defined in the shadow map so we return 1 for toatly visible
    if (cellIndexI > mapResolution || cellIndexI < 0 || cellIndexJ > mapResolution || cellIndexJ < 0) return 1;

    float depth = (WorldPos - position).length();

    
    if (cellIndexI == 5 && cellIndexJ == 5)
    {
        cout << "cell coordinates (" << cellIndexI << ", " << cellIndexJ << ")" << " depth: " << depth << "\n";
        cout << "visbility : " << shadowCells[cellIndexI * mapResolution + cellIndexJ].getVisibility(depth) << "\n";
    }
    
    return shadowCells[cellIndexI * mapResolution + cellIndexJ].getVisibility(depth);
}


float DeepShadowMap::getAveragesVisibilityFromWorldPos(Vector3 WorldPos)
{
    //This function calculates an average visbility value from the surrounding of the shadowcell corresponding of the world position
    //get the intersection position on the rectangle
    Ray rayToMap(WorldPos, position - WorldPos);
    Vector3 intersectionPoint, normal;
    mapRectangle.intersect(rayToMap, intersectionPoint, normal);

    //cout << "Intersection Point : " << intersectionPoint << "\n";

    //get the local coordinates with a projection
    Vector3 edge1 = mapRectangle.point2 - mapRectangle.point1;
    Vector3 edge2 = mapRectangle.point3 - mapRectangle.point1;
    Vector3 interPointDir = intersectionPoint - mapRectangle.point1;

    float localCoordinateX = (interPointDir * edge1) / (edge1 * edge1);
    float localCoordinateY = (interPointDir * edge2) / (edge2 * edge2);

    //cout << "local coordinates (" << localCoordinateX << ", " << localCoordinateY << ")\n";

    //get cell Index
    int cellIndexI = localCoordinateX * mapResolution;
    int cellIndexJ = localCoordinateY * mapResolution;

    //check if the cell coordinates are in the map resolution
    //if the index is not in the range than the object is not defined in the shadow map so we return 1 for toatly visible
    if (cellIndexI > mapResolution || cellIndexI < 0 || cellIndexJ > mapResolution || cellIndexJ < 0) return 1;
    float depth = (WorldPos - position).length();

    //Avarage visibility calculation
    float averageVisibility = 0;
    float weight;
    float normalizationValue = 0;

    //create a kernel
    int kernelSize = 1;
    std::vector<std::vector<double>> kernel = MathHelper::generateGaussianKernel(kernelSize, 5);
    //std::vector<std::vector<double>> kernel = MathHelper::generateUniformKernel(kernelSize);

    //cout << "cell calculation(" << cellIndexI << "," << cellIndexJ << "): \n";
    for (int i = -kernelSize/2; i <= kernelSize/2; i++)
    {
        if (cellIndexI + i < 0 || cellIndexI + i >= mapResolution) continue;
        for (int j = -kernelSize/2; j <= kernelSize/2; j ++)
        {
            if (cellIndexJ + j < 0 || cellIndexJ + j >= mapResolution) continue;
            weight = kernel[i + kernelSize/2][j + kernelSize/2];
            normalizationValue += weight;

            float vis = shadowCells[(cellIndexI + i) * mapResolution + (cellIndexJ + j)].getVisibility(depth);
            averageVisibility += vis * weight;

        }
    }

    return averageVisibility/normalizationValue;
}