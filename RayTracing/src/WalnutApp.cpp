#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

class AppLayer : public Walnut::Layer {
public:
	virtual void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", mLastRenderTime);
		if (ImGui::Button("Render"))
			Render();
		ImGui::DragFloat3("Ray Origin", mRenderer.GetRayOri(), 0.1f);
		ImGui::ColorEdit4("Sky Color", mRenderer.GetSkyColor());

		ImGui::Separator();
		ImGui::DragFloat("Sphere Radius", mRenderer.GetSphereRadius(), 0.1f);
		ImGui::ColorEdit4("Sphere Color", mRenderer.GetSphereColor());

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

		Render();
	}

	void Render() {
		timer.Reset();

		mRenderer.OnResize(mViewWidth, mViewHeight);
		mRenderer.Render();

		mLastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer mRenderer;
	Walnut::Timer timer;

	float mViewWidth = 0, mViewHeight = 0;
	float mLastRenderTime = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<AppLayer>();

	return app;
}
