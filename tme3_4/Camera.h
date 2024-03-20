#pragma once
#include "Vector3.h"
#include "Ray.h"

class Camera {
private:
    float focal_length;
    Vector3 position;
    Vector3 target_point;
    Vector3 up_vector;
    Vector3 forward;
    Vector3 right;
    Vector3 up;
    Vector3 pixel00_loc;
    float viewport_width, viewport_height;
    float image_width, image_height;
    Vector3 pixel_delta_u, pixel_delta_v;

public:
    Camera() = default;

    Camera(double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, \
        float v_width, float v_height, float image_width, float image_height);

    Ray pixelToRay(int pixelX, int pixelY) const;
};