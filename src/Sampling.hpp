#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>

static float hammer_phi(int k, int p)
{
	int kk = k;
	float pp = float(p);
	float phi = 0;
	while (kk > 0)
	{
		int a = kk % p;
		phi += float(a) / pp;
		kk = int(kk / p);
		pp = pp*p;
	}
	return phi;
}

static void hammersley_2d(int n, std::vector<float>& points)
{
	int k = 1;
	points.push_back(float(k) / float(n));
	points.push_back(hammer_phi(k, 2));
	for (++k; k < n + 1; ++k)
	{
		points.push_back(float(k) / n);
		points.push_back(hammer_phi(k, 2));
	}
}

static void halton_2d(int n, std::vector<float>& points)
{
	for (int k = 1; k < n + 1; ++k)
	{
		points.push_back(hammer_phi(k, 2));
		points.push_back(hammer_phi(k, 3));
	}
}
#endif //SAMPLING_HPP