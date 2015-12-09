#include "mat4.hpp"

mat4::mat4(float data[16])
{
	std::memcpy(d, data, sizeof(float) * 16); //Not safe but meh
}

mat4::mat4(float f1, float f2, float f3, float f4, float f5, float f6, 
	float f7, float f8, float f9, float f10, float f11, float f12, 
	float f13, float f14, float f15, float f16)
{
	d[0] = f1; d[1] = f2; d[2] = f3; d[3] = f4; d[4] = f5; d[5] = f6;
	d[6] = f7; d[7] = f8; d[8] = f9; d[9] = f10; d[10] = f11; d[11] = f12;
	d[12] = f13; d[13] = f14; d[14] = f15; d[15] = f16;
}

mat4 & mat4::operator+=(const mat4 & rhs)
{
	for (int i = 0; i < 16; ++i)
	{
		d[i] += rhs.d[i]; // Add component wise
	}
	return *this;
}

mat4 & mat4::operator-=(const mat4 & rhs)
{
	for (int i = 0; i < 16; ++i)
	{
		d[i] -= rhs.d[i]; //Subtract component wise
	}
	return *this;
}

mat4 & mat4::operator*=(float c)
{
	for (int i = 0; i < 16; ++i)
	{
		d[i] *= c;
	}
	return *this;
}

mat4 & mat4::operator/=(float c)
{
	float k = 1.0f / c;
	for (int i = 0; i < 16; ++i)
	{
		d[i] *= k;
	}
	return *this;
}

float mat4::operator()(int i, int j)
{
	return d[i * 4 + j];
}

float& mat4::operator[](int i)
{
	return d[i];
}



mat4 operator+(const mat4& lhs, const mat4& rhs)
{
	mat4 tmp(lhs);
	tmp += rhs;
	return tmp;
}
mat4 operator-(const mat4& lhs, const mat4& rhs)
{
	mat4 tmp(lhs);
	tmp += rhs;
	return tmp;
}
mat4 operator*(const mat4& lhs, const mat4& rhs);
mat4 operator*(const mat4& lhs, float c)
{
	mat4 tmp(lhs);
	tmp *= c;
	return tmp;
}
mat4 operator*(float c, const mat4& rhs)
{
	mat4 tmp(rhs);
	tmp *= c;
	return tmp;
}
mat4 operator/(const mat4& lhs, float c)
{
	mat4 tmp(lhs);
	tmp /= c;
	return tmp;
}

mat4 transpose(const mat4& a)
{
	mat4 tmp;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			tmp.d[i * 4 + j] = a.d[j * 4 + i];
		}
	}
	return tmp;
}