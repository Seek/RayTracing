#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

struct Sample 
{
	Sample() {}
	Sample(float _imageX, float _imageY)
		: imageX(_imageX), imageY(_imageY)
	{

	}
	float imageX;
	float imageY;
};
#endif // !SAMPLE_HPP
