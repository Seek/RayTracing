#include "Renderable.hpp"

bool Renderable::intersect(const Ray & ray, float * tHit, std::shared_ptr<Intersection>& intersection)
{
	std::shared_ptr<LocalSurface> ls(new LocalSurface());
	bool b = surface->intersect(ray, tHit, ls);
	intersection->ls = ls;
	intersection->material = material;
	return b;
}
