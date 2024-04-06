#pragma once
#include<cmath>
#include<iostream>
#include<vector>
#define PI 3.141592653589793

class MathHelper
{
public:
    static double gaussian(double x, double mean, double sigma);

    static std::vector<std::vector<double>> generateGaussianKernel(int size, double sigma);

};