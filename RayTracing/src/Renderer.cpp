#include "Walnut/Random.h"

#include "Scene.h"
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

void Renderer::Render(const Scene& scene, const Camera& camera) {
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < mImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < mImage->GetWidth(); x++) {
			int i = x + y * mImage->GetWidth();
			ray.Direction = camera.GetRayDirections()[i];

			glm::vec4 color = TraceRay(scene, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			mPixels[i] = Utils::ToRGBA(color);
		}
	}
	mImage->SetData(mPixels);
}
glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray) {
	if (scene.Spheres.size() == 0)
		return mSkyColor;

	const Sphere* closestSphere = nullptr;
	float closestHitDist = std::numeric_limits<float>::max();
	for (const Sphere& sphere : scene.Spheres) {
		glm::vec3 origin = ray.Origin - sphere.Position;
		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		// b^2 - 4ac
		float dis = b * b - 4.0f * a * c;

		if (dis < 0.0f) { // Ray didn't hit
			continue;
		}

		// (-b +|- sqrt(dis)) / 2a
		//float t0 = -b + glm::sqrt(dis) / 2.0f * a;
		float t1 = (-b - glm::sqrt(dis)) / (2.0f * a); // nearest
		if (t1 < 0.0f) {
			continue;
		}
		if (t1 < closestHitDist) {
			closestSphere = &sphere;
			closestHitDist = t1;
		}
	}

	if (closestSphere == nullptr) {
		return mSkyColor;
	}
	glm::vec3 origin = ray.Origin - closestSphere->Position;

	//glm::vec3 h0 = ray.Origin + ray.Direction * t0;
	glm::vec3 hitPoint = origin + ray.Direction * closestHitDist;
	glm::vec3 normal = glm::normalize(hitPoint); //  * 0.5f + 0.5f
	
	float d = glm::max(glm::dot(normal, -glm::normalize(mLightDir)), 0.0f);
	
	return glm::vec4(closestSphere->Color * d, 1.0f);
}
