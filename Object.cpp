#include"Object.h"
#include <cmath>
#include<iostream>
#include"Vector3.h"

Object::Object(Vector3 center, Vector3 color, MaterialParameters m_params)
{
	this->center = center;
	this->color = color;
	this->m_params = m_params;
	this->is_volumetric_object = false;
}

Object::Object()
{
	this->center = Vector3(0, 0, 0);
	this->color = Vector3(0, 0, 0);
	this->m_params = MaterialParameters();
	this->is_volumetric_object = false;
}

Object::Object(Vector3 center, Vector3 color, MaterialParameters m_params, bool is_volumetric_object)
{
	this->center = center;
	this->color = color;
	this->m_params = m_params;
	this->is_volumetric_object = is_volumetric_object;
}
