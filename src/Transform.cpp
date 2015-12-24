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
	return Transform(m * rhs.m, rhs.minv * minv);
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

Ray Transform::transformRay(const Ray & ray) const
{
	Ray r(ray);
	r.o = transformPoint(ray.o);
	r.dir = transformVector(ray.dir);
	return r;
}

Transform inv(const Transform & a)
{
	return Transform(a.minv, a.m);
}

Transform Transform::lookat(const vec3 & eye, const vec3 & target, const vec3 & up)
{
	mat4 tmp;
	tmp.d[3] = eye.x;
	tmp.d[7] = eye.y;
	tmp.d[11] = eye.z;
	vec3 dir = normalize(target - eye);
	vec3 left = normalize(cross(normalize(up), dir));
	vec3 newup = normalize(cross(dir, left));
	tmp.d[0] = left.x;
	tmp.d[4] = left.y;
	tmp.d[8] = left.z;
	tmp.d[1] = newup.x;
	tmp.d[5] = newup.y;
	tmp.d[9] = newup.z;
	tmp.d[2] = dir.x;
	tmp.d[6] = dir.y;
	tmp.d[10] = dir.z;
	return Transform(inv(tmp), tmp);
}

Transform Transform::orthographic(float left, float right, float top, float bottom, float near, float far)
{
	//From Real Time Rendering
	mat4 tmp(2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
		0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
		0.0f, 0.0f, 1.0f / (far - near), -near / (far - near),
		0.0f, 0.0f, 0.0f, 1.0f);
	return Transform(tmp, inv(tmp));
}


Transform Transform::perspective(float left, float right, float top, float bottom, float near, float far)
{
	mat4 tmp( (2.0f * near) / (right - left), 0.0f, -(right + left) / (right - left), 0.0f,
		0.0f, (2 * near) / (top - bottom), -(top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, (far) / (far - near), -(far*near) / (far - near),
		0.0f, 0.0f, 1.0f, 0.0f);
	return Transform(tmp, inv(tmp));
}

Transform Transform::screenToRaster(float imWidth, float imHeight)
{
	return Transform::scale(imWidth / 2.0f, imHeight / 2.0f, 1.0f) *
		Transform::translate(1.0f, 1.0f, 0.0f);
}

Transform Transform::worldToShading(const vec3 & normal, const vec3 & tangent, const vec3 & bitangent)
{
	mat4 tmp(bitangent.x, bitangent.y, bitangent.z, 0,
		tangent.x, tangent.y, tangent.z, 0,
		normal.x, normal.y, normal.z, 0,
		0, 0, 0, 1.0f);
	mat4 inv = transpose(tmp);
	return Transform(tmp, inv);
}
