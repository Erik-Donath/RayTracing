#pragma once

#include "glm\glm.hpp"

struct Ray {
	glm::vec3 Origin{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Direction{ 0.0f, 1.0f, 0.0f };
};

struct HitPayload {
	float HitDistance = -1.0f;
	glm::vec3 Position{0.0f, 0.0f, 0.0f };
	glm::vec3 Normal{ 0.0f, 1.0f, 0.0f };

	int32_t ObjectIndex = -1;
};
