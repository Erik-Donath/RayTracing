#include "Walnut/Random.h"

#include "Renderer.h"
#include "Utils.h"

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

void Renderer::Render() {
	for (uint32_t y = 0; y < mImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < mImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / mImage->GetWidth(), (float)y / mImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // 0 - 1 --> -1 - 1
			glm::vec4 color = PerPixel(coord);
			//color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			mPixels[x + y * mImage->GetWidth()] = Utils::ToRGBA(color);
		}
	}
	mImage->SetData(mPixels);
}
glm::vec4 Renderer::PerPixel(glm::vec2 coord) {
	//glm::vec3 rayOri(0.0f, 0.0f, -4.0f);
	glm::vec3 rayDir(coord.x, coord.y, -1.0f);
	//rayDir = glm::normalize(rayDir);
	float radius = mSphereRadius;

	float a = glm::dot(rayDir, rayDir);
	float b = 2.0f * glm::dot(mRayOri, rayDir);
	float c = glm::dot(mRayOri, mRayOri) - radius * radius;

	float dis = b * b - 4.0f * a * c;

	if (dis < 0.0f) { // Ray didn't hit
		return mSkyColor;
	}


	return mSphereColor;
}
