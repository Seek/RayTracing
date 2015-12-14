#include "Sphere.hpp"

bool Sphere::intersect(const Ray & r, float * tHit, LocalSurface * ls) const
{
	Ray ray(world2obj.transformPoint(r.o), world2obj.transformVector(r.dir));
	float a = ray.dir.length_squared();
	float b = 2.0f * dot(ray.dir, ray.o);
	float c = ray.o.length_squared() - radius * radius;
	float t0, t1;
	bool hit = quadratic(a, b, c, &t0, &t1);
	if (hit == false) return false;
	vec3 p;
	if (t0 < r.mint && t1 < r.mint) return false;
	if (t1 > r.maxt) return false;
	if (t0 < r.mint)
	{
		p = ray(t1); *tHit = t1;
	}
	else
	{
		p = ray(t0); *tHit = t0;
	}
	ls->p = obj2world.transformPoint(p);
	ls->n = normalize(obj2world.transformVector(p));
	return true;
}
