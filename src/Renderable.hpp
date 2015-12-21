#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include <memory>
#include "Surface.hpp"
#include "Material.hpp"
#include "Intersection.hpp"

typedef std::shared_ptr<Surface> shared_surface_ptr;
typedef std::shared_ptr<Material> shared_material_ptr;

class Renderable {
public:
	Renderable(const shared_surface_ptr& _surface, const shared_material_ptr& _material)
		: surface(_surface), material(_material) {}
	bool intersect(const Ray& ray, float* tHit, std::shared_ptr<Intersection>& intersection);
	shared_surface_ptr getSurface() const { return surface; }
	shared_material_ptr getMaterial() const { return material; }
private:
	shared_surface_ptr surface;
	shared_material_ptr material;
};
#endif // !RENDERABLE_HPP
