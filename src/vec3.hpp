#ifndef VEC3_HPP
#define VEC3_HPP

#pragma once

#include <cmath>
#include <cassert>

class vec3 {
public:
	vec3() {}
	vec3(float xx, float yy, float zz)
		: x(xx), y(yy), z(zz) {}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	// Methods
	void normalize();
	float length() const;
	float length_squared() const;
	//Operators
	vec3& operator+=(const vec3& rhs);
	vec3& operator-=(const vec3& rhs);
	vec3& operator*=(float rhs);
	vec3& operator/=(float rhs);
};
vec3 operator+(const vec3& lhs, const vec3& rhs);
vec3 operator-(const vec3& lhs, const vec3& rhs);
vec3 operator*(const vec3& lhs, float rhs);
vec3 operator*(float lhs, const vec3& rhs);
vec3 operator/(const vec3& lhs, float rhs);
vec3 operator-(const vec3& a);
//vec3 operator==(const vec3& lhs, const vec3& rhs);
//vec3 operator!=(const vec3& lhs, const vec3& rhs);
vec3 normalize(const vec3& a);
float dot(const vec3& a, const vec3& b);
vec3 cross(const vec3& a, const vec3& b);
#endif // !VEC3_HPP
