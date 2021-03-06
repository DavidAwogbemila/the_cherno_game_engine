#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/Core/Timestep.h"
#include "Event/Event.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {
 
  class HAZEL_API Application {
  public:
    Application();
    virtual ~Application();
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    
    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window;  }


  private:
    bool OnWindowClose(WindowCloseEvent&e);

    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;

  private:
    static Application* s_Instance;
  };

  // To be defined in client.
  Application* CreateApplication();
}
