#pragma once

#include "glm\glm.hpp"

namespace Utils {
	uint32_t ToRGBA(glm::vec4& color) {
		uint8_t r = (uint8_t)(color.x * 255.0f);
		uint8_t g = (uint8_t)(color.y * 255.0f);
		uint8_t b = (uint8_t)(color.z * 255.0f);
		uint8_t a = (uint8_t)(color.w * 255.0f);
		return 0x00000000 | (a << 24) | (b << 16) | (g << 8) | (r << 0);
	}
}