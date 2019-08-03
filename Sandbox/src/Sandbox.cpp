#include <Hazel.h>
#include <glm/glm/glm.hpp>
#include "imgui/imgui.h"

class ExampleLayer : public  Hazel::Layer{
public:
  ExampleLayer()
    : Layer("Example") {
  }

  void OnUpdate() override {

    if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
      HZ_INFO("Tab key is pressed.");
    }
  }

  void OnEvent(Hazel::Event& event) override {
    HZ_TRACE("{0}", event);
  }

  void OnImGuiRender() {
    ImGui::Begin("app doing");
    ImGui::Text("some text for ya");
    ImGui::End();
  }
};

class Sandbox : public Hazel::Application {
public:
  Sandbox() {
    PushLayer(new ExampleLayer());
  }

  ~Sandbox() {

  }
};

Hazel::Application* Hazel::CreateApplication() {
  return new Sandbox();
}
