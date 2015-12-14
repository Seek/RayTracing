#ifndef LOCAL_SURFACE_HPP
#define LOCAL_SURFACE_HPP

#pragma once

#include "vec3.hpp"

class LocalSurface {
public:
	LocalSurface() {}
	LocalSurface(vec3 _p, vec3 _n)
		: p(_p), n(_n) {}
	vec3 p;
	vec3 n;
};
#endif // !LOCAL_SURFACE_HPP
