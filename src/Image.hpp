#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>

struct Pixel {
	float r = 0;
	float g = 0;
	float b = 0;
};

class Image
{
public:
	Image(int width, int height);
protected:
	int width;
	int height;

};
#endif // !IMAGE_HPP
