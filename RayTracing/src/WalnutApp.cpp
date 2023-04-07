#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

class AppLayer : public Walnut::Layer {
public:
	AppLayer() : mCamera(45.0f, 0.1f, 100.0f) { }

	virtual void OnUpdate(float ts) override {
		mCamera.OnUpdate(ts);
	}
	virtual void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", mLastRenderTime);
		if (ImGui::Button("Render")) {
			autoRender = false;
			Render();
		}
		ImGui::Checkbox("Auto Render", &autoRender);
		ImGui::ColorEdit4("Sky Color", mRenderer.GetSkyColor());
		ImGui::DragFloat3("Light Dir", mRenderer.GetLightDir(), 0.1f);
		ImGui::Separator();

		ImGui::DragFloat3("Cam Position",  (float*)&mCamera.GetPosition(), 0.1f);
		ImGui::DragFloat3("Cam Direction", (float*)&mCamera.GetDirection(), 0.1f);

		ImGui::Separator();
		ImGui::DragFloat("Sphere Radius", mRenderer.GetSphereRadius(), 0.1f);
		ImGui::ColorEdit3("Sphere Color", mRenderer.GetSphereColor());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		mViewWidth = ImGui::GetContentRegionAvail().x;
		mViewHeight = ImGui::GetContentRegionAvail().y;

		std::shared_ptr<Walnut::Image> view = mRenderer.GetImage();
		if(view)
			ImGui::Image(view->GetDescriptorSet(), { (float)view->GetWidth(), (float)view->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		if(autoRender)
			Render();
	}

	void Render() {
		timer.Reset();

		mRenderer.OnResize(mViewWidth, mViewHeight);
		mCamera.OnResize(mViewWidth, mViewHeight);
		mRenderer.Render(mCamera);

		mLastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer mRenderer;
	Camera mCamera;

	Walnut::Timer timer;

	float mViewWidth = 0, mViewHeight = 0;
	float mLastRenderTime = 0;
	bool autoRender = true;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<AppLayer>();

	return app;
}
