#include <execution>
#include "Walnut/Random.h"

#include "Renderer.h"
#include "Utils.h"
#include "Ray.h"
#include "RenderTarget.h"
#include "Log.h"

void Renderer::Render(Camera* camera) {
	// Validate Args.
	if (camera == nullptr) {
		Log("Render failed: Camera* is nullptr!");
		return;
	}
	RenderTarget* renderTarget = camera->GetRenderTarget();
	uint32_t* pixels = renderTarget->GetPixels();
	if (renderTarget == nullptr || pixels == nullptr) {
		Log("Render failed: RenderTarget* is nullptr or pixels* is nullptr!");
		return;
	}

	#if Multithreded // Multithreading RayGen
	const std::vector<uint32_t>& indexes = camera->GetIndexes();
	std::for_each(std::execution::par, indexes.begin(), indexes.end(), [this, camera, pixels](uint32_t i) {
		glm::vec4 color = RayGen(i, camera);
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		pixels[i] = Utils::ToRGBA(color);
	});

	#else // Singlethreading RayGen
	for (uint32_t i = 0; i < renderTarget->GetWidth() * renderTarget->GetHeight(); i++) {
		glm::vec4 color = RayGen(i, camera);
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		pixels[i] = Utils::ToRGBA(color);
	}
	#endif

	renderTarget->UploadPixels();
}
glm::vec4 Renderer::RayGen(const uint32_t& i, Camera* camera) {
	Ray ray;
	ray.Origin = camera->GetPosition();
	ray.Direction = camera->GetRayDirections()[i];

	glm::vec3 color(0.0f, 0.0f, 0.0f);
	float multiplier = 1.0f;

	for (int k = 0; k < mRayBounces; k++) {
		HitPayload payload = TraceRay(ray);

		if (payload.HitDistance == -1.0f) {
			color += mScene->SkyColor * multiplier;
			break;
		}

		glm::vec3 lightDir = glm::normalize(mScene->LightDir);
		float d = glm::max(glm::dot(payload.Normal, -lightDir), 0.0f);

		const Sphere& sphere = mScene->Spheres[payload.ObjectIndex];
		glm::vec3 sphereColor = sphere.Color * d;
		color += sphereColor * multiplier;
		multiplier *= 0.8f;

		ray.Origin = payload.Position + payload.Normal * 0.0001f;
		ray.Direction = glm::reflect(ray.Direction, payload.Normal);
	}
	return glm::vec4(color, 1.0f);
}
HitPayload Renderer::TraceRay(const Ray& ray) {
	if (mScene->Spheres.size() == 0)
		return MissHit(ray);

	int closestSphere = -1;
	float closestHitDist = std::numeric_limits<float>::max();

	for (size_t i = 0; i < mScene->Spheres.size(); i++) {
		const Sphere& sphere = mScene->Spheres[i];
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
		float t1 = (-b - glm::sqrt(dis)) / (2.0f * a); // nearest

		if (t1 > 0.0f && t1 < closestHitDist) {
			closestSphere = (int)i;
			closestHitDist = t1;
		}
	}

	if (closestSphere == -1)
		return MissHit(ray);
	return ClosestHit(ray, closestHitDist, closestSphere);
}
HitPayload Renderer::ClosestHit(const Ray& ray, float hitDist, int32_t objI) {
	const Sphere& closestSphere = mScene->Spheres[objI];
	glm::vec3 origin = ray.Origin - closestSphere.Position;

	HitPayload payload;
	payload.HitDistance = hitDist;
	payload.ObjectIndex = objI;
	payload.Position = origin + ray.Direction * hitDist;
	payload.Normal = glm::normalize(payload.Position);

	payload.Position += closestSphere.Position;
	return payload;
}
HitPayload Renderer::MissHit(const Ray& ray) {
	HitPayload payload;
	payload.HitDistance = -1.0f;
	payload.ObjectIndex = -1;
	return payload;
}
