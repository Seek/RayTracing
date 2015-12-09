#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#pragma once
#include "mat4.hpp"
#include "vec3.hpp"
class Transform {
public:
	Transform() {}
	Transform(const mat4& mat, const mat4& inv)
		: m(mat), minv(inv) {}
	static Transform scale(float x, float y, float z);
	static Transform translate(float x, float y, float z);
	static Transform rotateX(float theta);
	static Transform rotateY(float theta);
	static Transform rotateZ(float theta);
	static Transform rotateAxis(float theta, const vec3& axis);
	static Transform lookat();
	static Transform orthographic();
	static Transform perspective();
	friend Transform inv(const Transform& a);
	Transform operator*(const Transform& rhs) const;
private:
	mat4 m, minv;
};


#endif //TRANSFORM_HPP