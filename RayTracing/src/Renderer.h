#pragma once

#include <memory>
#include "Walnut\Image.h"
#include "glm\glm.hpp"

#include "Ray.h"
#include "Camera.h"
#include "Scene.h"

class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);

	std::shared_ptr<Walnut::Image> GetImage() const {
		return mImage;
	}
	float* GetSkyColor() {
		return (float*)&mSkyColor;
	}
	float* GetLightDir() {
		return (float*)&mLightDir;
	}

private:
	glm::vec4 TraceRay(const Scene& scene, const Ray& ray);

	std::shared_ptr<Walnut::Image> mImage;
	uint32_t* mPixels = nullptr;

	glm::vec4 mSkyColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	glm::vec3 mLightDir = { -1.0f, -1.0f, -1.0f };
};
