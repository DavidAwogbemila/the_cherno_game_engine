#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include "GLFW/glfw3.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application::Application() {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
  }

  Application::~Application() {
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    HZ_CORE_TRACE("{0}", e);
  }

  void Application::Run() {

    WindowResizeEvent e(1200, 720);
    MouseMovedEvent m(1.1, 2.2);

    HZ_TRACE(e);
    HZ_TRACE(m);

    while (m_Running) {
      glClearColor(1.0f, 0.0f, 1.0f, 1);
      glClear(GL_COLOR_BUFFER_BIT);
      m_Window->OnUpdate();
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return m_Running;
  }
}
