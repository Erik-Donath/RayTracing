#include <iostream>
#include <execution>
#include "Walnut/Random.h"

#include "Renderer.h"
#include "Utils.h"
#include "Ray.h"
#include "RenderTarget.h"

void Renderer::Render(Camera& camera) {
	RenderTarget* renderTarget = camera.GetRenderTarget();
	if (!renderTarget) {
		std::cout << "Render Target is null" << std::endl;
		return;
	}
	uint32_t* pixels = renderTarget->GetPixels();
	if (!pixels) {
		std::cout << "Pixels is null" << std::endl;
		return;
	}
	#if Multithreded
	const std::vector<uint32_t>& indexes = camera.GetIndexes();
	std::for_each(std::execution::par, indexes.begin(), indexes.end(), [this, camera, pixels](uint32_t i) {
		Ray ray;
		ray.Origin = camera.GetPosition();
		ray.Direction = camera.GetRayDirections()[i];

		glm::vec4 color = TraceRay(ray);
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		pixels[i] = Utils::ToRGBA(color);
	});
	#else
	Ray ray;
	ray.Origin = camera.GetPosition();
	for (uint32_t y = 0; y < renderTarget->GetHeight(); y++) {
		for (uint32_t x = 0; x < renderTarget->GetWidth(); x++) {
			int i = x + y * renderTarget->GetWidth();
			ray.Direction = camera.GetRayDirections()[i];

			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			pixels[i] = Utils::ToRGBA(color);
		}
	}
	#endif
	renderTarget->UploadPixels();
}
glm::vec4 Renderer::TraceRay(const Ray& ray) {
	if (mScene->Spheres.size() == 0)
		return mScene->SkyColor;

	const Sphere* closestSphere = nullptr;
	float closestHitDist = std::numeric_limits<float>::max();
	for (const Sphere& sphere : mScene->Spheres) {
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
		return mScene->SkyColor;
	}
	glm::vec3 origin = ray.Origin - closestSphere->Position;

	//glm::vec3 h0 = ray.Origin + ray.Direction * t0;
	glm::vec3 hitPoint = origin + ray.Direction * closestHitDist;
	glm::vec3 normal = glm::normalize(hitPoint); //  * 0.5f + 0.5f
	
	float d = glm::max(glm::dot(normal, -glm::normalize(mScene->LightDir)), 0.0f);
	
	return glm::vec4(closestSphere->Color * d, 1.0f);
}
