#include "vec3.hpp"

void vec3::normalize()
{
	float l = 1.0f / length();
	x *= l; y *= l; z *= l;
}

float vec3::length() const
{
	return sqrt(x*x + y*y + z*z);
}

float vec3::length_squared() const
{
	return x*x + y*y + z*z;
}

vec3& vec3::operator+=(const vec3 & rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

vec3 & vec3::operator-=(const vec3 & rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

vec3 & vec3::operator*=(float rhs)
{
	x *= rhs; y *= rhs; z *= rhs;
	return *this;
}

vec3 & vec3::operator/=(float rhs)
{
	float k = 1 / rhs;
	x *= k; y *= k; z *= k;
	return *this;
}

vec3 operator+(const vec3& lhs, const vec3& rhs)
{
	vec3 tmp(lhs);
	tmp += rhs;
	return tmp;
}

vec3 operator-(const vec3& lhs, const vec3& rhs)
{
	vec3 tmp(lhs);
	tmp -= rhs;
	return tmp;
}

vec3 operator*(const vec3& lhs, float rhs)
{
	vec3 tmp(lhs);
	tmp *= rhs;
	return tmp;
}

vec3 operator*(float lhs, const vec3& rhs)
{
	vec3 tmp(rhs);
	tmp *= lhs;
	return tmp;
}

vec3 operator/(const vec3& lhs, float rhs)
{
	vec3 tmp(lhs);
	tmp /= rhs;
	return tmp;
}

vec3 operator-(const vec3& a)
{
	vec3 tmp(a);
	tmp.x = -tmp.x; tmp.y = -tmp.y; tmp.z = -tmp.z;
	return tmp;
}

vec3 normalize(const vec3 & a)
{
	vec3 tmp(a);
	tmp.normalize();
	return tmp;
}

float dot(const vec3& a, const vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.y  * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}