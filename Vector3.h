#ifndef VECTOR3_H
#define VECTOR3_H

#include<iostream>

class Vector3 {
public:
    float x;
    float y;
    float z;

public:
    // Constructors
    Vector3();
    Vector3(float x, float y, float z);

    // Destructor
    ~Vector3();

    // Getter methods
    double getX() const;
    double getY() const;
    double getZ() const;

    // Setter methods
    void setX(float x);
    void setY(float y);
    void setZ(float z);

    //Vector operators
    Vector3 add(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator-(const float& scalar) const;
    Vector3 operator+(const Vector3& v) const;
    float operator*(const Vector3& v) const;
    Vector3 operator*(const float& c) const;
    Vector3 operator/(float scalar) const;
    float length() const;
    Vector3 normalized() const;
    Vector3 crossProduct(const Vector3& v) const;
    double magnitude() const;

    friend std::ostream& operator<<(std::ostream &out, const Vector3 &vect);

};

#endif // VECTOR3_H
