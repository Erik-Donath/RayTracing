#pragma once
#include <memory>
#include <iostream>
#include "Walnut\Image.h"

class RenderTarget {
public:
	virtual uint32_t GetWidth() { return 0; }
	virtual uint32_t GetHeight() { return 0; }
	virtual uint32_t* GetPixels() { return nullptr; }
	virtual bool OnRezise(uint32_t width, uint32_t height) { return false; }
	virtual void UploadPixels() { }
};

class ImageTarget : public RenderTarget {
public:
	uint32_t GetWidth() override { return mImage->GetWidth(); }
	uint32_t GetHeight() override { return mImage->GetHeight(); }
	uint32_t* GetPixels() override { return mPixels; }
	bool OnRezise(uint32_t width, uint32_t height) override {
		if (!mImage) {
			delete[] mPixels;
			mImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
			mPixels = new uint32_t[width * height];
			return true;
		}

		if (width != mImage->GetWidth() || height != mImage->GetHeight()) {
			delete[] mPixels;
			mImage->Resize(width, height);
			mPixels = new uint32_t[width * height];
			return true;
		}
		return false;
	}
	void UploadPixels() override {
		mImage->SetData(mPixels);
	}
	std::shared_ptr<Walnut::Image>& GetImage() {
		return mImage;
	}
private:
	uint32_t* mPixels = nullptr;
	std::shared_ptr<Walnut::Image> mImage = nullptr;
};
