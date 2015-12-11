#include "vec3.hpp"
#include "mat4.hpp"
#include "Transform.hpp"

int main(int argc, char* argv[]) {
	vec3 a;
	vec3 b(1.0f, 2.0f, 3.0f);
	vec3 c(2.0f, 3.0f, 1.0f);
	vec3 d = cross(b, c);
	float dd = dot(b, b);
	bool t = (dd == b.length_squared());
	vec3 e = -d;
	vec3 f = normalize(d);
	d.normalize();
	dd = d.length();
	mat4 l;
	l[2] = 1.2f;
	l[3] = 2.0f;
	mat4 m = inv(l);
	mat4 n = m * l;
	a = vec3(1.0f, 0.0f, 0.0f);
	Transform sc = Transform::scale(2.0, 2.0, 2.0);
	Transform tr = Transform::translate(1.0, 1.0, 1.0);
	vec3 at = tr.transformVector(a);
	vec3 att = tr.transformPoint(a);
	return 0;
}