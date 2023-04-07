#pragma once

#include <memory>
#include "Walnut\Image.h"
#include "glm\glm.hpp"

#include "Ray.h"
#include "Camera.h"

class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Camera& camera);

	std::shared_ptr<Walnut::Image> GetImage() const {
		return mImage;
	}
	float* GetSkyColor() {
		return (float*)&mSkyColor;
	}
	float* GetLightDir() {
		return (float*)&mLightDir;
	}

	float* GetSphereRadius() {
		return &mSphereRadius;
	}
	float* GetSphereColor() {
		return (float*)&mSphereColor;
	}

private:
	glm::vec4 TraceRay(const Ray& ray);

	std::shared_ptr<Walnut::Image> mImage;
	uint32_t* mPixels = nullptr;

	glm::vec4 mSkyColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 mLightDir = { -1.0f, -1.0f, -1.0f };

	float mSphereRadius = 0.5f;
	glm::vec3 mSphereColor = { 0.0f, 1.0f, 1.0f};
};
