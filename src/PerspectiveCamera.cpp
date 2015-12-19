#include "PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(const Transform & cam2world, const Transform & world2cam, 
	float left, float right, float top, float bottom, float near, float far, 
	float imWidth, float imHeight)
	: Camera(cam2world, world2cam)
{
	cameraToScreen = Transform::perspective(left, right, top, bottom, near, far);
	screenToCamera = inv(cameraToScreen);
	screenToRaster = Transform::screenToRaster(imWidth, imHeight);
	rasterToScreen = inv(screenToRaster);
}

Ray PerspectiveCamera::generateRay(float rasterX, float rasterY) const
{
	vec3 pRaster(rasterX, rasterY, 0.0f);
	vec3 pCamera = rasterToScreen.transformPoint(pRaster);
	pCamera = screenToCamera.transformPoint(pCamera);
	//Ray r(pCamera, vec3(0.0f, 0.0f, 1.0f));
	Ray r(vec3(0.0f, 0.0f, 0.0f), pCamera);
	r = cam2world.transformRay(r);
	r.dir = normalize(r.dir);
	return r;
}

