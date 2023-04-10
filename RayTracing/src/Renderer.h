#pragma once

#include <memory>
#include "Walnut\Image.h"
#include "glm\glm.hpp"

#include "Ray.h"
#include "Camera.h"
#include "Scene.h"
#include "Settings.h"

class Renderer {
public:
	Renderer(const Scene* scene) : mScene(scene) { }
	void Render(Camera* camera);

	uint32_t* GetRayBounces() {
		return &mRayBounces;
	}
private:
	glm::vec4 RayGen(const uint32_t& i, Camera* camera); // Per Pixel
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDist, int32_t objI);
	HitPayload Miss(const Ray& ray);

	const Scene* mScene;
	uint32_t mRayBounces = 32;
};
