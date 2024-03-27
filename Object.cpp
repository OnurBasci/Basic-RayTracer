#include"Object.h"
#include <cmath>
#include<iostream>
#include"Vector3.h"

Object::Object(Vector3 center, Vector3 color, MaterialParameters m_params)
{
	this->center = center;
	this->color = color;
	this->m_params = m_params;
}

Object::Object()
{
	this->center = Vector3(0, 0, 0);
	this->color = Vector3(0, 0, 0);
	this->m_params = MaterialParameters();
}