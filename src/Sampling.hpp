#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>
#include "RNG.hpp"

float hammer_phi(int k, int p);
void hammersley_2d(int n, std::vector<float>& points);
void halton_2d(int n, std::vector<float>& points);
void stratified1D(int nSamples, bool jitter, std::vector<float>& points, const RNG& rng);
// Creates a 2D stratified sampling pattern layed out in one dimension in \p points
void stratified2D(int nX, int nY, bool jitter, std::vector<float>& points, const RNG& rng);
#endif //SAMPLING_HPP