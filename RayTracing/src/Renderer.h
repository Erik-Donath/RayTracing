#pragma once

#include <memory>
#include "Walnut\Image.h"
#include "glm\glm.hpp"

class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> GetImage() const {
		return mImage;
	}
	float* GetSkyColor() {
		return skyColor;
	}
	float* GetRayOri() {
		return (float*)&rayOri;
	}

private:
	uint32_t PerPixel(glm::vec2 coord);

	std::shared_ptr<Walnut::Image> mImage;
	uint32_t* mPixels = nullptr;

	float skyColor[3] = {0.0f, 0.0f, 0.0f};
	glm::vec3 rayOri = { 0.0f, 0.0f, -4.0f };
};