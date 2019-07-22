#pragma once

#include "Hazel/Input.h"

namespace Hazel {

  class WindowsInput : public Input {
  protected:
    virtual bool IsMouseButtonPressedImpl(int keyCode) override;
    virtual bool IsKeyPressedImpl(int keyCode) override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
    virtual std::pair<float, float>GetMousePositionImpl() override;
  };

}
