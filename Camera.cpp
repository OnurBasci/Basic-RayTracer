#include"Camera.h"
#include <cmath>
#include<iostream>
#include"Vector3.h"
#include"Ray.h"

Camera::Camera(double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, float v_width, float v_height, float i_width, float i_height): 
    focal_length(_focal_length), position(_position), target_point(_target_point), up_vector(_up_vector),
    viewport_height(v_height), viewport_width(v_width),  image_width(i_width), image_height(i_height) {

    forward = (target_point - position).normalized();
    right = forward.crossProduct(up_vector).normalized();
    up = right.crossProduct(forward).normalized();

    Vector3 viewport_u = right * viewport_width;
    Vector3 viewport_v = up * -viewport_height; //viewport height is negatif since we go from top to down

    pixel_delta_u = viewport_u/image_width;
    pixel_delta_v = viewport_v/image_height;

    Vector3 viewport_upper_left = _position + forward * focal_length 
        - viewport_u / 2 - viewport_v/2;

    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

Ray Camera::pixelToRay(int pixelX, int pixelY) const {
    /*double aspect_ratio = 1.0; // Assuming square pixels for simplicity
    double sensor_x = (2 * (pixelX + 0.5) / aspect_ratio - 1);
    double sensor_y = (1 - 2 * (pixelY + 0.5) / aspect_ratio);

    Vector3 direction_camera = (forward * focal_length) + right * sensor_x + up * sensor_y;*/

    Vector3 pixel_center = pixel00_loc + (pixel_delta_u*pixelX) + (pixel_delta_v*pixelY);
    Vector3 rayStartPosition = pixel_center - forward * focal_length;
    //Vector3 rayStartPosition(pixel_center.x, pixel_center.y, 0); //send horizontal rays
    //Vector3 ray_direction = pixel_center - rayStartPosition;

    return Ray(rayStartPosition, forward.normalized());
}
