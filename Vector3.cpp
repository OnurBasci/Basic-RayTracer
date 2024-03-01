#include "Vector3.h"
#include <cmath>
#include<iostream>

// Constructors
Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// Destructor
Vector3::~Vector3() {}

// Getter methods
double Vector3::getX() const {
    return x;
}

double Vector3::getY() const {
    return y;
}

double Vector3::getZ() const {
    return z;
}

// Setter methods
void Vector3::setX(float _x) {
    x = _x;
}

void Vector3::setY(float _y) {
    y = _y;
}

void Vector3::setZ(float _z) {
    z = _z;
}

// Vector operations
Vector3 Vector3::add(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const float& scalar) const {
    return Vector3(x - scalar, y-scalar, z - scalar);
}

float Vector3::operator*(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::operator*(const float& c) const {
    return Vector3(x * c, y * c, z * c);
}

Vector3 Vector3::operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

float Vector3::length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const {
    float len = length();
    return (*this) / len;
}

std::ostream& operator<<(std::ostream& out, const Vector3 &vect)
{
    return std::cout << "(" << vect.x << ", " << vect.y << "," << vect.z << ")";
}

Vector3 Vector3::crossProduct(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

double Vector3::magnitude() const {
    return sqrt(x * x + y * y + z * z);
}
