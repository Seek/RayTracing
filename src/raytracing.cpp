#include "Vec3.hpp"

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
	return 0;
}