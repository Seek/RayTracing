#ifndef IMAGE_HPP
#define IMAGE_HPP

#ifdef _MSC_VER
#pragma once
#endif 

#include <cstdint>
#include <algorithm>
#include <vector>
#include <mutex>
#include "zlib.h"
#include "png.h"
#include "vec3.hpp"
#include "Sample.hpp"

struct Pixel {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float weightSum = 0.0f;
};

//Implicity using box filter
//Discrete coordinates are floor(contin)
//Discrete to contin are d + 0.5
class Image
{
public:
	Image(int _width, int _height);
	void addSample(const Sample& sample, const vec3& color);
	int saveImage(const char* path);
	float getAspectRatio() const { return aspectRatio; }
protected:
	float aspectRatio;
	int width;
	int height;
	std::vector<Pixel> pixels;
private:
	std::mutex mutex;

};
#endif // !IMAGE_HPP
