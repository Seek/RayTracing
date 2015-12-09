#ifndef MAT4_HPP
#define MAT4_HPP

#pragma once

#include <cstring>
#include <exception>

// A column major 4x4 matrix implementation
class mat4 {
public:
	mat4() {}
	mat4(float data[16]);
	mat4(float f1, float f2, float f3, float f4,
		float f5, float f6, float f7, float f8,
		float f9, float f10, float f11, float f12,
		float f13, float f14, float f15, float f16);

	float d[16]{ 1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f };

	mat4& operator+=(const mat4& rhs);
	mat4& operator-=(const mat4& rhs);
	mat4& operator*=(float c);
	mat4& operator/=(float c);
	float operator()(int i, int j);
	float& operator[](int i);
};

mat4 operator+(const mat4& lhs, const mat4& rhs);
mat4 operator-(const mat4& lhs, const mat4& rhs);
mat4 operator*(const mat4& lhs, const mat4& rhs);
mat4 operator*(const mat4& lhs, float c);
mat4 operator*(float c, const mat4& rhs);
mat4 operator/(const mat4& lhs, float c);
mat4 transpose(const mat4& a);
mat4 inv(const mat4& a);
#endif // !MAT4_HPP