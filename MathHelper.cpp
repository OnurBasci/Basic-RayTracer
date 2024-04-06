#include<cmath>
#include"MahtHelper.h"


double MathHelper::gaussian(double x, double mean, double sigma) {
    return exp(-((x - mean) * (x - mean)) / (2 * sigma * sigma)) / (sqrt(2 * PI) * sigma);
}

std::vector<std::vector<double>> MathHelper::generateGaussianKernel(int size, double sigma) {
    if (size % 2 == 0) {
        std::cerr << "Kernel size must be odd." << std::endl;
        exit(1);
    }

    std::vector<std::vector<double>> kernel(size, std::vector<double>(size, 0.0));

    int center = size / 2;
    double total = 0.0;

    // Generate kernel values
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] = gaussian(i - center, 0, sigma) * gaussian(j - center, 0, sigma);
            total += kernel[i][j];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] /= total;
        }
    }

    return kernel;
}