#include "Sphere.hpp"

bool Sphere::intersect(const Ray & r, float * tHit, std::shared_ptr<LocalSurface>& ls) const
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
	//should check for div by 0?
	//if (!(p.x < 0.0f) && !(p.x > 0.0f)) p.x = 0.0001f;
	float u = atan2(p.y, p.x); //phi
	float v = acos(clamp(-1.0f, 1.0f, p.z / radius)); //theta
	vec3 dpdu(-radius*sin(v)*sin(u), radius*sin(v)*cos(u), 0);
	vec3 dpdv(radius*cos(v)*cos(u), radius*cos(v)*sin(u), -radius*cos(v));
	ls->dpdu = normalize(obj2world.transformVector(dpdu));
	ls->dpdv = normalize(obj2world.transformVector(dpdv));
	return true;
}
