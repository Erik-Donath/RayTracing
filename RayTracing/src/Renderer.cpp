#include "Walnut/Random.h"

#include "Renderer.h"

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
	for (int y = 0; y < mImage->GetHeight(); y++) {
		for (int x = 0; x < mImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / mImage->GetWidth(), (float)y / mImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // 0 - 1 --> -1 - 1
			mPixels[x + y * mImage->GetWidth()] = PerPixel(coord);
		}
	}
	mImage->SetData(mPixels);
}
uint32_t Renderer::PerPixel(glm::vec2 coord) {
	//glm::vec3 rayOri(0.0f, 0.0f, -4.0f);
	glm::vec3 rayDir(coord.x, coord.y, -1.0f);
	float radius = 0.5f;

	float a = glm::dot(rayDir, rayDir);
	float b = 2.0f * glm::dot(rayOri, rayDir);
	float c = glm::dot(rayOri, rayOri) - radius * radius;

	float dis = b * b - 4.0f * a * c;

	if (dis < 0.0f) {
		uint32_t color;
		uint8_t r = (uint8_t)(skyColor[0] * 255.0f);
		uint8_t g = (uint8_t)(skyColor[1] * 255.0f);
		uint8_t b = (uint8_t)(skyColor[2] * 255.0f);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}
	return 0xffff00ff;
}