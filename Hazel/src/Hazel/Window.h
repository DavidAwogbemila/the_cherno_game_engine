#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {
  struct WindowProps {
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& Title = "Hazel Engine",
                unsigned int Width = 1200,
                unsigned int Height = 720)
      : Title(Title), Width(Width), Height(Height) {

    }
  };

  class HAZEL_API Window {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
  };
}
