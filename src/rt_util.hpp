#ifndef RT_UTIL_HPP
#define RT_UTIL_HPP

#pragma once

#include <cmath>
#include <algorithm>

inline float degreeToRadians(float theta)
{
	return ( M_PI * theta) / 180.0f;
}
inline float radiansToDegrees(float theta)
{
	return (theta*180.0f) / M_PI;
}
inline float lerp(float t, float v1, float v2)
{
	return (1.0f - t) * v1 + t * v2;
}

inline float clamp(float min, float max, float val)
{
	if (val < min) return min;
	else if(val > max) return max;
	else return val;

}

static bool quadratic(float a, float b, float c, float* r0, float* r1)
{
	float discrim = b * b - 4.0f * a *c;
	if (discrim < 0.0f) return false;
	float sqdiscrim = sqrt(discrim);
	float q;
	if (b < 0.0f) q = -0.5f * (b - sqdiscrim);
	else q = -0.5f * (b + sqdiscrim);
	*r0 = q / a; *r1 = c / q;
	if (*r0 > *r1) std::swap(*r0, *r1);
	return true;
}
#endif // !RT_UTIL_HPP
