#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#ifdef _MSC_VER
#pragma once
#endif


#include <memory>
#include "Material.hpp"
#include "LocalSurface.hpp"

class Intersection
{
public:
	Intersection() {};
	Intersection(std::shared_ptr<LocalSurface> _ls, std::shared_ptr<Material> _material)
		: ls(_ls), material(_material) {}
	std::shared_ptr<LocalSurface> ls;
	std::shared_ptr<Material> material;
};
#endif // !INTERSECTION_HPP
