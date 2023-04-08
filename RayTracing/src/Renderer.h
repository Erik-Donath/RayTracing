#pragma once

#include <memory>
#include "Walnut\Image.h"
#include "glm\glm.hpp"

#include "Ray.h"
#include "Camera.h"
#include "Scene.h"

class Renderer {
public:
	Renderer(Scene* scene) : mScene(scene) { }

	void Render(Camera& camera);

	float* GetSkyColor() {
		return (float*)&mSkyColor;
	}
	float* GetLightDir() {
		return (float*)&mLightDir;
	}

private:
	glm::vec4 TraceRay(const Ray& ray);
	Scene* mScene;
	Camera* mCurrentCamera = nullptr;

	glm::vec4 mSkyColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	glm::vec3 mLightDir = { -1.0f, -1.0f, -1.0f };
};
