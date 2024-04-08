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

std::vector<std::vector<double>> MathHelper::generateUniformKernel(int size)
{
    return std::vector<std::vector<double>>(size, std::vector<double>(size, 1.0/size));
}

float MathHelper::constantDesnity(Vector3 point)
{
    //constant density returns 1 everywhere
    return 1;
}

//Perlin noise 

int p[512] = {
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180
};

double MathHelper::fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
double MathHelper::lerp(double t, double a, double b) { return a + t * (b - a); }
double MathHelper::grad(int hash, double x, double y, double z) { 
    int h = hash & 15;
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
};

double MathHelper::perlin_noise(Vector3 pos)
{
    int X = (int)floor(pos.x) & 255,
        Y = (int)floor(pos.y) & 255,
        Z = (int)floor(pos.z) & 255;
    pos.x -= floor(pos.x);
    pos.y -= floor(pos.y);
    pos.z -= floor(pos.z);
    double u = fade(pos.x),
        v = fade(pos.y),
        w = fade(pos.z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,
        B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    
    double noise = lerp(w, lerp(v, lerp(u, grad(p[AA], pos.x, pos.y, pos.z),
        grad(p[BA], pos.x - 1, pos.y, pos.z)),
        lerp(u, grad(p[AB], pos.x, pos.y - 1, pos.z),
            grad(p[BB], pos.x - 1, pos.y - 1, pos.z))),
        lerp(v, lerp(u, grad(p[AA + 1], pos.x, pos.y, pos.z - 1),
            grad(p[BA + 1], pos.x - 1, pos.y, pos.z - 1)),
            lerp(u, grad(p[AB + 1], pos.x, pos.y - 1, pos.z - 1),
                grad(p[BB + 1], pos.x - 1, pos.y - 1, pos.z - 1))));

    //normalize it between 0 and 1
    return (noise + 1) / 2;
}