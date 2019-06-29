#pragma once

#include "Core.h"

#include "Window.h"
#include "Event/Event.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/MouseEvent.h"

namespace Hazel {
 
  class HAZEL_API Application {
  public:
    Application();
    virtual ~Application();
    void Run();

    void OnEvent(Event& e);

  private:
    bool OnWindowClose(WindowCloseEvent&e);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
  };

  // To be defined in client.
  Application* CreateApplication();
}
