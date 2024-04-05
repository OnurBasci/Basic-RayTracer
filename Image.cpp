#include"Image.h"
#include <cmath>
#include<iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Vector3.h"


//Constructor
Image::Image(int w, int h)
{
    width = w;
    height = h;

    this->pixels = vector<vector<Vector3>>(height);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixels[i].push_back(bg_color);
        }
    }
};


bool Image::write(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Couldn't open file " << filename << " for writing." << std::endl;
        return false;
    }
   
    // Write PPM header
    file << "P3\n" << width << " " << height << "\n255\n";


    Vector3 pixel;
    // Write pixel data
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j+=1)
        {
            pixel = pixels[i][j];
            file << pixel.x << " " << pixel.y << " " << pixel.z << "  ";
        }
    }
    /*for (const auto& pixel : pixels) {

        file << pixel.x << " " << pixel.y << " " << pixel.z;

        unsigned int r = static_cast<unsigned int>(255 * pixel.getX());
        unsigned int g = static_cast<unsigned int>(255 * pixel.getY());
        unsigned int b = static_cast<unsigned int>(255 * pixel.getZ());

        file.put(r);
        file.put(g);
        file.put(b);
    }*/

    file.close();
    return true;
}

void Image::writeHelloToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "Hello" << std::endl;
        file.close();
        std::cout << "Successfully wrote 'Hello' to " << filename << std::endl;
    }
    else {
        std::cerr << "Error: Couldn't open file " << filename << " for writing." << std::endl;
    }
}