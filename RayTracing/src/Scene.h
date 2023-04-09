#pragma once

#include <vector>
#include "glm\glm.hpp"

struct Sphere {
	glm::vec3 Position;
	float Radius = 0.5;
	glm::vec3 Color { 1.0f, 1.0f, 1.0f };
};
struct Scene {
	std::vector<Sphere> Spheres;

	glm::vec4 SkyColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	glm::vec3 LightDir = { -1.0f, -1.0f, -1.0f };
};
