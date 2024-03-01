#pragma once
#include<string>
#include<vector>
#include "Vector3.h"

const int TAILLE_MAX = 200;

class Image {
public:
	int width, height;
	Vector3 pixels[TAILLE_MAX][TAILLE_MAX];

	Image(int width, int height);

	bool write(const std::string& filename) const;
	void writeHelloToFile(const std::string& filename);
};