#ifndef SURFACE_HPP
#define SURFACE_HPP

#pragma once

#include "Ray.hpp"
#include "Transform.hpp"
#include "LocalSurface.hpp"

class Surface {
public:
	Surface() {}
	Surface(const Transform& _obj2world, const Transform& _world2obj)
		: obj2world(_obj2world), world2obj(_world2obj) {}

	virtual bool intersect(const Ray& r, float* tHit, LocalSurface* ls) const = 0;

	Transform obj2world, world2obj;
};
#endif // !SURFACE_HPP
