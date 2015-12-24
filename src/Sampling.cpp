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

void stratified1D(int nSamples, bool jitter, std::vector<float>& points, const RNG& rng)
{
	float stratum_size = 1.0f / float(nSamples);
	for (int i = 0; i < nSamples; ++i)
	{
		float del = jitter ? rng.nextFloat() : 0.5f;
		float s = float(i) + del;
		s *= stratum_size;
		points.push_back(s);
	}
}

void stratified2D(int nX, int nY, bool jitter, std::vector<float>& points, const RNG & rng)
{
	float sizeX = 1.0f / float(nX);
	float sizeY = 1.0f / float(nY);
	for (int y = 0; y < nY; ++y)
	{
		for (int x = 0; x < nX; ++x)
		{
			float delX = jitter ? rng.nextFloat() : 0.5f;
			float delY = jitter ? rng.nextFloat() : 0.5f;
			points.push_back((x + delX) * sizeX);
			points.push_back((y + delY) * sizeY);
		}
	}
}
void concentricSampleDisk(float u1, float u2, float* dx, float* dy)
{

	float r, theta;
	// Map uniform random numbers to $[-1,1]^2$
	float sx = 2 * u1 - 1;
	float sy = 2 * u2 - 1;

	// Map square to $(r,\theta)$

	// Handle degeneracy at the origin
	if (sx == 0.0 && sy == 0.0) {
		*dx = 0.0;
		*dy = 0.0;
		return;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			// Handle first region of disk
			r = sx;
			if (sy > 0.0) theta = sy / r;
			else          theta = 8.0f + sy / r;
		}
		else {
			// Handle second region of disk
			r = sy;
			theta = 2.0f - sx / r;
		}
	}
	else {
		if (sx <= sy) {
			// Handle third region of disk
			r = -sx;
			theta = 4.0f - sy / r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx / r;
		}
	}
	theta *= float(M_PI) / 4.f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}