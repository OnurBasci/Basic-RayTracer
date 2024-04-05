#pragma once
#include<string>
#include<vector>
#include "Vector3.h"

const int TAILLE_MAX = 200;

using namespace std;

class Image {
public:
	int width, height;
	//Vector3 pixels[TAILLE_MAX][TAILLE_MAX];

	vector<vector<Vector3>> pixels;

	Vector3 bg_color = Vector3(86, 29, 219);

	Image(int width, int height);

	bool write(const std::string& filename) const;
	void writeHelloToFile(const std::string& filename);
};