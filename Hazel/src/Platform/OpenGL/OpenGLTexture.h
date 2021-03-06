#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

  class OpenGLTexture2D : public Texture2D {
  public: 
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();

    virtual void Bind(uint32_t slot = 0) const override;

    virtual uint32_t GetWidth() const override { return m_Width; };
    virtual uint32_t GetHeight() const override { return m_Height; };
 
  private:
    uint32_t m_RendererID;
    std::string m_Path;
    uint32_t m_Width;
    uint32_t m_Height;
  };

}