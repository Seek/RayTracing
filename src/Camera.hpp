#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifdef _MSC_VER
#pragma once
#endif 

#include "Transform.hpp"
#include "Ray.hpp"

class Camera
{
public:
	Camera(const Transform& _cam2world, const Transform& _world2cam)
		: cam2world(_cam2world), world2cam(_world2cam) {}
	// Returns a ray in world space to be traced
	virtual Ray generateRay(float rasterX, float rasterY) const = 0;
protected:
	Transform cam2world, world2cam;
};
#endif // !CAMERA_HPP
