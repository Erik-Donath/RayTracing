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
	Renderer(Scene* scene) : mScene(scene) { }
	void Render(Camera& camera);
private:
	glm::vec4 TraceRay(const Ray& ray);
	Scene* mScene;
};
