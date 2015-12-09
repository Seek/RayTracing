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
mat4 operator*(const mat4& lhs, const mat4& rhs)
{
	mat4 tmp;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			tmp[i * 4 + j] = lhs.d[i * 4] * rhs.d[j]
				+ lhs.d[i * 4 + 1] * rhs.d[4 + j]
				+ lhs.d[i * 4 + 2] * rhs.d[8 + j]
				+ lhs.d[i * 4 + 3] * rhs.d[12 + j];
		}
	}
	return tmp;
}
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

// This code was adapted from MESA
mat4 inv(const mat4& a) 
{
	float det;
	int i;
	mat4 tmp;
	float* inv = tmp.d;
	const float* m = a.d;
	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		throw std::exception("Division by zero");

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return tmp;
}