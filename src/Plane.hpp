#ifndef PLANE_HPP
#define PLANE_HPP

#ifdef _MSC_VER
#pragma once
#endif 
#include <limits>
#include <memory>
#include "Surface.hpp"

class Plane : public Surface 
{
public:
	//Create an plane of infinite extent at p with normal n. Should be in world space.
	Plane(const vec3& p, const vec3& n);
	// Create a plane of finite extent with normal n. Should be in world space.
	Plane(const vec3& p, const vec3& n, const vec3& min, const vec3& max);
	// Inherited via Surface
	virtual bool intersect(const Ray & r, float * tHit, std::shared_ptr<LocalSurface>& ls) const override;
private:
	vec3 minp{ -std::numeric_limits<float>::infinity(), 
		-std::numeric_limits<float>::infinity(), 
		-std::numeric_limits<float>::infinity() };
	vec3 maxp{ std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity() };
	vec3 normal;
	vec3 point;
};
#endif // !PLANE_HPP
