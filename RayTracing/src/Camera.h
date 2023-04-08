#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Walnut\Image.h"

class Camera {
public:
	Camera(float verticalFOV, float nearClip, float farClip);

	bool OnUpdate(float ts);
	void OnResize(uint32_t width, uint32_t height);

	const glm::mat4& GetProjection() const {
		return mProjection;
	}
	const glm::mat4& GetInverseProjection() const {
		return mInverseProjection;
	}
	const glm::mat4& GetView() const {
		return mView;
	}
	const glm::mat4& GetInverseView() const {
		return mInverseView;
	}
	const glm::vec3& GetPosition() const {
		return mPosition;
	}
	const glm::vec3& GetDirection() const {
		return mForwardDirection;
	}
	const std::vector<glm::vec3>& GetRayDirections() const {
		return mRayDirections;
	}
	const std::vector<uint32_t>& GetIndexes() const {
		return mIndiexes;
	}
	const std::shared_ptr<Walnut::Image>& GetImage() const {
		return mImage;
	}
	uint32_t* GetPixels() const {
		return mPixels;
	}

	float GetRotationSpeed();
private:
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();
private:
	glm::mat4 mProjection{ 1.0f };
	glm::mat4 mView{ 1.0f };
	glm::mat4 mInverseProjection{ 1.0f };
	glm::mat4 mInverseView{ 1.0f };

	float mVerticalFOV = 45.0f;
	float mNearClip = 0.1f;
	float mFarClip = 100.0f;

	glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f };
	glm::vec3 mForwardDirection{ 0.0f, 0.0f, 0.0f };

	// Cached ray directions
	std::vector<glm::vec3> mRayDirections;
	std::vector<uint32_t> mIndiexes;

	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	std::shared_ptr<Walnut::Image> mImage;
	uint32_t* mPixels = nullptr;
};
