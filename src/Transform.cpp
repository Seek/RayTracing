#include "Transform.hpp"

Transform Transform::scale(float x, float y, float z)
{
	Transform t;
	t.m.d[0] = x; t.minv.d[0] = 1.0f / x;
	t.m.d[5] = y; t.minv.d[5] = 1.0f / y;
	t.m.d[10] = z; t.minv.d[10] = 1.0f / z;
	return t;
}

Transform Transform::translate(float x, float y, float z)
{
	Transform t;
	t.m.d[3] = x; t.minv.d[3] = -x;
	t.m.d[7] = y; t.minv.d[7] = -y;
	t.m.d[11] = z; t.minv.d[11] = -z;
	return t;
}

Transform Transform::operator*(const Transform & rhs) const
{
	return Transform(m * rhs.m, minv * rhs.minv);
}

vec3 Transform::transformVector(const vec3 & a) const
{
	float x = m.d[0] * a.x + m.d[1] * a.y + m.d[2] * a.z;
	float y = m.d[4] * a.x + m.d[5] * a.y + m.d[6] * a.z;
	float z = m.d[8] * a.x + m.d[9] * a.y + m.d[10] * a.z;
	return vec3(x, y, z);
}

vec3 Transform::transformPoint(const vec3 & a) const
{
	float x = m.d[0] * a.x + m.d[1] * a.y + m.d[2] * a.z + m.d[3];
	float y = m.d[4] * a.x + m.d[5] * a.y + m.d[6] * a.z + m.d[7];
	float z = m.d[8] * a.x + m.d[9] * a.y + m.d[10] * a.z + m.d[11];
	float w = m.d[12] * a.x + m.d[13] * a.y + m.d[14] * a.z + m.d[15];
	vec3 v(x, y, z);
	return v/w;
}

vec3 Transform::transformNormal(const vec3 & a) const
{
	float x = minv.d[0] * a.x + minv.d[4] * a.y + minv.d[8] * a.z;
	float y = minv.d[1] * a.x + minv.d[5] * a.y + minv.d[9] * a.z;
	float z = minv.d[2] * a.x + minv.d[6] * a.y + minv.d[10] * a.z;
	return vec3(x, y, z);
}

Transform inv(const Transform & a)
{
	return Transform(a.minv, a.m);
}
