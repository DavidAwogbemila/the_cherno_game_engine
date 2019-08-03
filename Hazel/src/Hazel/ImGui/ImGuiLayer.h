#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MouseEvent.h"

namespace Hazel {
  class HAZEL_API ImGuiLayer: public Layer {
  public:
    ImGuiLayer();
    ~ImGuiLayer();

   virtual void OnAttach() override;
   virtual void OnDetach() override;
   //virtual void OnUpdate() override;
   //virtual void OnEvent(Event& e) override;
   virtual void OnImGuiRender() override;

   void Begin();
   void End();

  private:
    float m_Time = 0.0f;
  };

}