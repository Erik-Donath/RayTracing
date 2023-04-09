#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Walnut/Input/Input.h"

using namespace Walnut;

Camera::Camera(float verticalFOV, float nearClip, float farClip, RenderTarget* renderTarget)
	: mVerticalFOV(verticalFOV), mNearClip(nearClip), mFarClip(farClip), mRenderTarget(renderTarget) {
	mForwardDirection = glm::vec3(0, 0, -1);
	mPosition = glm::vec3(0, 0, 6);
}

bool Camera::OnUpdate(float ts) {
	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
	m_LastMousePosition = mousePos;

	if (!Input::IsMouseButtonDown(MouseButton::Right)) {
		Input::SetCursorMode(CursorMode::Normal);
		return false;
	}

	Input::SetCursorMode(CursorMode::Locked);

	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(mForwardDirection, upDirection);

	float speed = 5.0f;

	// Movement
	if (Input::IsKeyDown(KeyCode::W)) {
		mPosition += mForwardDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::S)) {
		mPosition -= mForwardDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyDown(KeyCode::A)) {
		mPosition -= rightDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::D)) {
		mPosition += rightDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyDown(KeyCode::Q)) {
		mPosition -= upDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::E)) {
		mPosition += upDirection * speed * ts;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f) {
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		mForwardDirection = glm::rotate(q, mForwardDirection);

		moved = true;
	}

	if (moved) {
		RecalculateView();
		RecalculateRayDirections();
	}

	return moved;
}

void Camera::OnResize(uint32_t width, uint32_t height) {
	bool resized = mRenderTarget->OnRezise(width, height);
	if (!resized)
		return;

	mIndiexes.resize(width * height);
	for (uint32_t i = 0; i < width * height; i++) {
		mIndiexes[i] = i;
	}

	RecalculateProjection();
	RecalculateView(); // A
	RecalculateRayDirections();
}

float Camera::GetRotationSpeed() {
	return 0.3f;
}

void Camera::RecalculateProjection() { // GH
	mProjection = glm::perspectiveFov(glm::radians(mVerticalFOV), (float)mRenderTarget->GetWidth(), (float)mRenderTarget->GetHeight(), mNearClip, mFarClip);
	mInverseProjection = glm::inverse(mProjection);
}

void Camera::RecalculateView() {
	mView = glm::lookAt(mPosition, mPosition + mForwardDirection, glm::vec3(0, 1, 0));
	mInverseView = glm::inverse(mView);
}

void Camera::RecalculateRayDirections() {
	mRayDirections.resize(mRenderTarget->GetWidth() * mRenderTarget->GetHeight());

	for (uint32_t y = 0; y < mRenderTarget->GetHeight(); y++) {
		for (uint32_t x = 0; x < mRenderTarget->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / (float)mRenderTarget->GetWidth(), (float)y / (float)mRenderTarget->GetHeight() };
			coord = coord * 2.0f - 1.0f; // -1 -> 1

			glm::vec4 target = mInverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDirection = glm::vec3(mInverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
			mRayDirections[x + y * mRenderTarget->GetWidth()] = rayDirection;
		}
	}
}
