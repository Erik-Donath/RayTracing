#include <iostream>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"
#include "Settings.h"

class AppLayer : public Walnut::Layer {
public:
	AppLayer() : mCamera(45.0f, 0.1f, 100.0f, new ImageTarget()), mRenderer(&mScene) {
		mScene.Spheres.push_back(Sphere{ {0.0f, 0.0f, 0.0f}, 0.5f, { 0.0f, 0.0f, 0.0f } });
		mScene.Spheres.push_back(Sphere{ {1.0f, 0.0f, 0.0f}, 0.5f, { 1.0f, 0.0f, 0.0f } });
		mScene.Spheres.push_back(Sphere{ {0.0f, 1.0f, 0.0f}, 0.5f, { 0.0f, 1.0f, 0.0f } });
		mScene.Spheres.push_back(Sphere{ {1.0f, 1.0f, 0.0f}, 0.5f, { 1.0f, 1.0f, 0.0f } });
		mScene.Spheres.push_back(Sphere{ {0.0f, 0.0f, 1.0f}, 0.5f, { 0.0f, 0.0f, 1.0f } });
		mScene.Spheres.push_back(Sphere{ {1.0f, 0.0f, 1.0f}, 0.5f, { 1.0f, 0.0f, 1.0f } });
		mScene.Spheres.push_back(Sphere{ {0.0f, 1.0f, 1.0f}, 0.5f, { 0.0f, 1.0f, 1.0f } });
		mScene.Spheres.push_back(Sphere{ {1.0f, 1.0f, 1.0f}, 0.5f, { 1.0f, 1.0f, 1.0f } });
	}

	virtual void OnUpdate(float ts) override {
		mCamera.OnUpdate(ts);
	}
	virtual void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", mLastFullRenderTime);
		if (ImGui::Button("Render")) {
			autoRender = false;
			Render();
		}
		ImGui::Checkbox("Auto Render", &autoRender);
		ImGui::ColorEdit3("Sky Color", (float*)&mScene.SkyColor);
		ImGui::DragFloat3("Light Dir", (float*)&mScene.LightDir, 0.1f);
		ImGui::DragInt("Ray Bounces", (int*)mRenderer.GetRayBounces(), 1, 0, std::numeric_limits<int>::max());
		ImGui::Separator();

		ImGui::DragFloat3("Cam Position", (float*)&mCamera.GetPosition(), 0.1f);
		ImGui::DragFloat3("Cam Direction", (float*)&mCamera.GetDirection(), 0.1f);

		ImGui::End();

		ImGui::Begin("Scene");

		// Create Spheres
		if (ImGui::Button("Create Sphere")) {
			mScene.Spheres.push_back(Sphere{ mCreatePosition, mCreateRadius, mCreateColor });
			mCreatePosition = { 0.0f, 0.0f, 0.0f };
			mCreateRadius = 1.0f;
			mCreateColor = { 1.0f, 1.0f, 1.0f };
		}
		ImGui::DragFloat3("Position", (float*)&mCreatePosition, 0.1f);
		ImGui::DragFloat("Radius", &mCreateRadius, 0.1f);
		ImGui::ColorEdit3("Color", (float*)&mCreateColor);
		ImGui::Separator();

		// Delete objectes
		for (int i = 0; i < objectsToDelete.size(); i++) {
			mScene.Spheres.erase(mScene.Spheres.begin() + objectsToDelete[i]);
		}
		objectsToDelete.clear();

		// Clone objectes
		for (int i = 0; i < objectsToClone.size(); i++) {
			Sphere& toClone = mScene.Spheres[objectsToClone[i]];
			mScene.Spheres.push_back(Sphere(toClone));
		}
		objectsToClone.clear();

		// Show objects
		for (int i = 0; i < mScene.Spheres.size(); i++) {
			ImGui::PushID(i);
			ImGui::Text("Sphere %i", i + 1);
			if (ImGui::Button("Delete")) {
				objectsToDelete.push_back(i);
			}
			if (ImGui::Button("Clone")) {
				objectsToClone.push_back(i);
			}
			ImGui::DragFloat3("Position", (float*)&mScene.Spheres[i].Position, 0.1f);
			ImGui::DragFloat("Radius", &mScene.Spheres[i].Radius, 0.1f);
			ImGui::ColorEdit3("Color", (float*)&mScene.Spheres[i].Color);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport 1");
		mViewWidth = ImGui::GetContentRegionAvail().x;
		mViewHeight = ImGui::GetContentRegionAvail().y;

		ImageTarget* imageTarget = (ImageTarget*)mCamera.GetRenderTarget();
		if (imageTarget != nullptr) {
			std::shared_ptr<Walnut::Image> view = imageTarget->GetImage();
			if(view)
				ImGui::Image(view->GetDescriptorSet(), {(float)view->GetWidth(), (float)view->GetHeight()}, ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		if(autoRender)
			Render();
	}

	void Render() {
		timer.Reset();

		mCamera.OnResize(mViewWidth, mViewHeight);
		mRenderer.Render(&mCamera);

		mLastFullRenderTime = timer.ElapsedMillis();
	}
private:
	Scene mScene;
	Renderer mRenderer;
	Camera mCamera;

	glm::vec3 mCreatePosition{0.0f, 0.0f, 0.0f};
	float mCreateRadius = 1.0f;
	glm::vec3 mCreateColor{1.0f, 1.0f, 1.0f};
	std::vector<int> objectsToDelete;
	std::vector<int> objectsToClone;

	Walnut::Timer timer;

	float mViewWidth = 0, mViewHeight = 0;
	float mLastFullRenderTime = 0;
	bool autoRender = true;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<AppLayer>();

	return app;
}
