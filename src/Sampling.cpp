#include "Sampling.hpp"

float hammer_phi(int k, int p)
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

void hammersley_2d(int n, std::vector<float>& points)
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

void halton_2d(int n, std::vector<float>& points)
{
	for (int k = 1; k < n + 1; ++k)
	{
		points.push_back(hammer_phi(k, 2));
		points.push_back(hammer_phi(k, 3));
	}
}

void stratified1D(int nSamples, bool jitter, std::vector<float>& points, const std::mt19937& engine)
{
	auto dist = std::uniform_real_distribution<float>();
	float stratum_size = 1.0f / float(nSamples);
	for (int i = 0; i < nSamples; ++i)
	{
		float del = jitter ? dist(engine) : 0.5;
		float s = i + jitter;
		s *= stratum_size;
		points.push_back(s);
	}
}
