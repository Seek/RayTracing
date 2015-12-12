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

Transform Transform::orthographic(float near, float far)
{
	return Transform::scale(1.0f, 1.0f, 1.0f / (far - near)) * Transform::translate(0.0f, 0.0f, -near);
}

Transform Transform::orthographic2(float left, float right, float top, float bottom, float near, float far)
{
	mat4 m(1.0f, 0.0f, 0.0f, -(left + right) / 2.0f,
		0.0f, 1.0f, 0.0f, -(top + bottom) / 2.0f,
		0.0f, 0.0f, -1.0f, -(far + near) / 2.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return Transform::scale(2.0f/(right-left), 2.0f/(top-bottom), 2.0f/(far-near)) *  Transform(m, inv(m));
}

Transform Transform::perspective(float fov, float near, float far)
{
	mat4 persp;
	persp.d[10] = far / (far - near);
	persp.d[11] = -((far * near) / (far - near));
	persp.d[15] = 0.0f;
	persp.d[14] = 1.0f;
	float invtan = 1.0f / tan(fov / 2.0f);
	return Transform::scale(invtan, invtan, 1.0f) * Transform(persp, inv(persp));
}

Transform Transform::screenToRaster(float imWidth, float imHeight)
{
	float width = imWidth; float height = imHeight;
	return Transform::scale(imWidth, imHeight, 1.0f) *
		Transform::translate(1.0f, 1.0f, 0.0f);
}
