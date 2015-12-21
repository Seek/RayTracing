#ifndef SPHERE_HPP
#define SPHERE_HPP

#pragma once

#include <memory>
#include "rt_util.hpp"
#include "Surface.hpp"

class Sphere : public Surface
{
public:
	Sphere(const Transform& obj2world, const Transform& world2obj, float _radius)
		: Surface(obj2world, world2obj), radius(_radius) {}
	
	// Inherited via Surface
	virtual bool intersect(const Ray & r, float * tHit, std::shared_ptr<LocalSurface>& ls) const override;
private:
	float radius = 1.0f;
};
#endif // !SPHERE_HPP
