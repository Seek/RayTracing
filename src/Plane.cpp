#include "Plane.hpp"

Plane::Plane(const vec3& p, const vec3 & n)
	: point(p), normal(n)
{
}

Plane::Plane(const vec3& p, const vec3 & n, const vec3 & min, const vec3 & max)
	: point(p),normal(n), minp(min), maxp(max)
{
}


bool Plane::intersect(const Ray & r, float * tHit, std::shared_ptr<LocalSurface>& ls) const
{
	float ldn = dot(r.dir, normal);
	if (ldn < 0)
	{
		*tHit = dot((point - r.o), normal) / ldn;
		ls->p = r(*tHit);
		ls->n = normal;
		return true;
	}
	return false;
}
