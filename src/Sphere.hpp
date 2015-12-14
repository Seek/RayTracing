#ifndef SPHERE_HPP
#define SPHERE_HPP

#pragma once

#include "rt_util.hpp"
#include "Surface.hpp"

class Sphere : public Surface
{
public:
	Sphere(const Transform& obj2world, const Transform& world2obj, float _radius)
		: Surface(obj2world, world2obj), radius(_radius) {}

private:
	float radius = 1.0f;

	// Inherited via Surface
	virtual bool intersect(const Ray & r, float * tHit, LocalSurface * ls) const override;
};
#endif // !SPHERE_HPP
