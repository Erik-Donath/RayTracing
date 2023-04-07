#include "Walnut/Random.h"

#include "Renderer.h"
#include "Utils.h"
#include "Ray.h"

void Renderer::OnResize(uint32_t width, uint32_t height) {
	if (!mImage) {
		mImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		mPixels = new uint32_t[width * height];
		return;
	}

	if (mImage->GetWidth() != width || mImage->GetHeight() != height) {
		mImage->Resize(width, height);
		delete[] mPixels;
		mPixels = new uint32_t[width * height];
	}
}

void Renderer::Render(const Camera& camera) {
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < mImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < mImage->GetWidth(); x++) {
			int i = x + y * mImage->GetWidth();
			ray.Direction = camera.GetRayDirections()[i];

			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			mPixels[i] = Utils::ToRGBA(color);
		}
	}
	mImage->SetData(mPixels);
}
glm::vec4 Renderer::TraceRay(const Ray& ray) {
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - mSphereRadius * mSphereRadius;

	// b^2 - 4ac
	float dis = b * b - 4.0f * a * c;

	if (dis < 0.0f) { // Ray didn't hit
		return mSkyColor;
	}

	// (-b +|- sqrt(dis)) / 2a
	//float t0 = -b + glm::sqrt(dis) / 2.0f * a;
	float t1 = (-b - glm::sqrt(dis)) / (2.0f * a); // nearest

	//glm::vec3 h0 = mray.Origin + ray.Direction * t0;
	glm::vec3 hitPoint = ray.Origin + ray.Direction * t1;
	glm::vec3 normal = glm::normalize(hitPoint); //  * 0.5f + 0.5f
	
	float d = glm::max(glm::dot(normal, -glm::normalize(mLightDir)), 0.0f);
	
	return glm::vec4(mSphereColor * d, 1.0f);
}
