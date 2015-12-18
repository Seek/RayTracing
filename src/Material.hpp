#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#ifdef _MSC_VER
#pragma once
#endif 

#include "vec3.hpp"

class Material
{
public: 
	Material() {}
	Material(const vec3& _color)
		: color(_color) {}
	vec3 getColor() const { return color; }
	vec3 setColor(const vec3& _color) { color = _color; }
private:
	vec3 color{ 1.0f, 1.0f, 1.0f };

};
#endif // !MATERIAL_HPP
