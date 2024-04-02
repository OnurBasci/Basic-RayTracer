#pragma once
#include<list>
#include<vector>
#include"Vector3.h"
#include"Object.h"
#include"Rectangle.h"
//#include"ShadowCell.h"

using namespace std;

//forward declaration
class ShadowCell;

//this is the class that contains the shadow map structure wich is a 2d array of cells each cell containing a visibility function
class DeepShadowMap
{
public:
    list<Object*> objects; //the objects on the scene, necessary for intersection calculation
    float focal_length;
    Vector3 position;
    Vector3 target_point;
    Vector3 up_vector;
    Vector3 forward;
    Vector3 right;
    Vector3 up;
    Vector3 celll00_loc;
    float viewport_width, viewport_height;
    float mapResolution;
    Vector3 pixel_delta_u, pixel_delta_v;
    vector<ShadowCell> shadowCells;
    //Rectangle mapRectangle; //This is a rectangle wich helps us to find the corresponding volume function index via an intersection calculation
    

    DeepShadowMap() = default;

    DeepShadowMap(list<Object*> objects, double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, \
        float v_width, float v_height, float mapResolution);
};