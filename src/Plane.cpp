#include "Plane.hpp"

inline void CoordinateSystem(const vec3 &v1, vec3 *v2, vec3 *v3) {
	if (fabsf(v1.x) > fabsf(v1.y)) {
		float invLen = 1.f / sqrtf(v1.x*v1.x + v1.z*v1.z);
		*v2 = vec3(-v1.z * invLen, 0.f, v1.x * invLen);
	}
	else {
		float invLen = 1.f / sqrtf(v1.y*v1.y + v1.z*v1.z);
		*v2 = vec3(0.f, v1.z * invLen, -v1.y * invLen);
	}
	*v3 = cross(v1, *v2);
}

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
		vec3 dpdu, dpdv;
		CoordinateSystem(normal, &dpdu, &dpdv);
		//vec3 dpdu = normalize(vec3(1, 0, normal.y / normal.z));
		//vec3 dpdv = normalize(vec3(0, 1, normal.x / normal.z));
		ls->dpdu = dpdu;
		ls->dpdv = dpdv;
		return true;
	}
	return false;
}
