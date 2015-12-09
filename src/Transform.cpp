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

Transform inv(const Transform & a)
{
	return Transform(a.minv, a.m);
}
