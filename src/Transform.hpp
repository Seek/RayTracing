#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#pragma once
#include "mat4.hpp"
#include "vec3.hpp"
#include "Ray.hpp"

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
	static Transform lookat(const vec3& eye, const vec3& target, const vec3& up);
	static Transform orthographic(float left, float right, float top, float bottom, float near, float far);
	static Transform perspective(float left, float right, float top, float bottom, float near, float far);
	// The pbrt implementation of this function supports subwindowing
	// we don't at this time.
	static Transform screenToRaster(float imWidth, float imHeight);
	friend Transform inv(const Transform& a);
	Transform operator*(const Transform& rhs) const;
	vec3 transformVector(const vec3& a) const;
	vec3 transformPoint(const vec3& a) const;
	vec3 transformNormal(const vec3& a) const;
	Ray transformRay(const Ray& ray);
private:
	mat4 m, minv;
};


#endif //TRANSFORM_HPP