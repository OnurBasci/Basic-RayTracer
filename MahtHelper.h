#pragma once
#include<cmath>
#include<iostream>
#include<vector>
#include"Vector3.h"
#define PI 3.141592653589793

class MathHelper
{
public:
    static double gaussian(double x, double mean, double sigma);

    static std::vector<std::vector<double>> generateGaussianKernel(int size, double sigma);

    //density functions
    static float constantDesnity(Vector3 p);
};