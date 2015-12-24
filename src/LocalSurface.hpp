#ifndef LOCAL_SURFACE_HPP
#define LOCAL_SURFACE_HPP

#pragma once

#include "vec3.hpp"

class LocalSurface {
public:
	LocalSurface() {}
	LocalSurface(const vec3& _p, const vec3& _n,
		const vec3& _dpdu, const vec3& _dpdv)
		: p(_p), n(_n), dpdu(_dpdu), dpdv(_dpdv) {}
	vec3 p;
	vec3 n;
	vec3 dpdu;
	vec3 dpdv;
};
#endif // !LOCAL_SURFACE_HPP
