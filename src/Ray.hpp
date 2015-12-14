#ifndef RAY_HPP
#define RAY_HPP

#pragma once
#include <limits>
#include "vec3.hpp"

class Ray {
public:
	//Ctors
	Ray() {};
	Ray(vec3 _o, vec3 _dir, float _mint = 0.0f,
		float _maxt = std::numeric_limits<float>::infinity(),
		int _depth = 0)
		: o(_o), dir(_dir), mint(_mint), maxt(_maxt), depth(_depth) {}
	
	Ray(vec3 _o, vec3 _dir, const Ray& parent, float _mint = 0.0f,
		float _maxt = std::numeric_limits<float>::infinity())
		: o(_o), dir(_dir), mint(_mint), maxt(_maxt), depth(parent.depth + 1) {}
	
	//Data members
	vec3 o;
	vec3 dir;
	float mint = 0.0f;
	float maxt = std::numeric_limits<float>::infinity();
	int depth = 0; //For limiting bounces

	vec3 operator()(float t)
	{
		return o + t * dir;
	}
};

#endif // !RAY_HPP
