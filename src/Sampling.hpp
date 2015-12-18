#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>
#include <random>

float hammer_phi(int k, int p);
void hammersley_2d(int n, std::vector<float>& points);
void halton_2d(int n, std::vector<float>& points);
void stratified1D(int nSamples, bool jitter, std::vector<float>& points, 
					std::uniform_real_distribution<float> dist);

#endif //SAMPLING_HPP