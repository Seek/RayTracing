#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>
#include <algorithm>
#include <cmath>
#include "RNG.hpp"
#include "vec3.hpp"
float hammer_phi(int k, int p);
void hammersley_2d(int n, std::vector<float>& points);
void halton_2d(int n, std::vector<float>& points);
void stratified1D(int nSamples, bool jitter, std::vector<float>& points, const RNG& rng);
// Creates a 2D stratified sampling pattern layed out in one dimension in \p points
void stratified2D(int nX, int nY, bool jitter, std::vector<float>& points, const RNG& rng);

//Following functison borrowed from pbrt v2 
void concentricSampleDisk(float u1, float u2, float* dx, float* dy);

inline vec3 cosineSampleHemisphere(float u1, float u2) {
	vec3 ret;
	concentricSampleDisk(u1, u2, &ret.x, &ret.y);
	ret.z = std::sqrtf(std::max(0.f, 1.f - ret.x*ret.x - ret.y*ret.y));
	return ret;
}

inline float cosineHemispherePdf(float costheta, float phi) {
	return costheta * M_1_PI;
}
//inline float CosTheta(const vec3 &w) { return w.z; }
//inline float AbsCosTheta(const vec3 &w) { return fabsf(w.z); }

#endif //SAMPLING_HPP