#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

  class OpenGLVertexBuffer : public VertexBuffer {
  public:
    OpenGLVertexBuffer(float* indices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual const BufferLayout GetBufferLayout() const override { return m_Layout; }
    virtual void SetBufferLayout(const BufferLayout& layout) override { m_Layout = layout; }

  private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
  };


  class OpenGLIndexBuffer : public IndexBuffer {
  public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual uint32_t GetCount() const override { return m_Count; };

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;
  };

}
