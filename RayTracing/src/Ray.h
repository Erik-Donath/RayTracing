#pragma once

#include "glm\glm.hpp"

struct Ray {
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct HitPayload {
	float HitDistance = -1;
	glm::vec3 Position;
	glm::vec3 Normal;

	int32_t ObjectIndex = -1;
};