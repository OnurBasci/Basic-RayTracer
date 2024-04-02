#include"DeepShadowMap.h"
#include"ShadowCell.h"

DeepShadowMap::DeepShadowMap(double _focal_length, const Vector3& _position, const Vector3& _target_point, const Vector3& _up_vector, \
    float v_width, float v_height, float mapRes): focal_length(_focal_length), position(_position), target_point(_target_point), up_vector(_up_vector),
    viewport_height(v_height), viewport_width(v_width), mapResolution(mapRes)
{
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
}