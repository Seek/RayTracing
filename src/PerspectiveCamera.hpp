#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Camera.hpp"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const Transform& cam2world, const Transform& world2cam,
		float left, float right, float top, float bottom, float near, float far,
		float imWidth, float imHeight);

	// Inherited via Camera
	virtual Ray generateRay(float rasterX, float rasterY) const override;
private:
	Transform screenToRaster, rasterToScreen;
	Transform screenToCamera, cameraToScreen;
};
#endif // !PERSPECTIVE_CAMERA_HPP
