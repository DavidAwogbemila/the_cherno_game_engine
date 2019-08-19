#include "hzpch.h"
#include "Application.h"

#include "Input.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel {

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

  Application* Application::s_Instance = nullptr;

  Application::Application()
  : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {
    HZ_CORE_ASSERT(!s_Instance, "Application already exists.")
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    m_ImGuiLayer = new ImGuiLayer();
    
    PushOverlay(m_ImGuiLayer);

    m_VertexArray.reset(VertexArray::Create());

    // Set up data to draw triangle.
    float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    vertexBuffer->SetBufferLayout({
      {ShaderDataType::Float3, "a_Position"},
      {ShaderDataType::Float4, "a_Color"}
      });

    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, 3));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    // Set up data to draw square.
    float squareVertices[3 * 4] = {
       0.0f, 0.0f, 0.0f,
       0.5f, 0.0f, 0.0f, 
       0.5f, 0.5f, 0.0f,
       0.0f, 0.5f, 0.0f
    };

    m_SquareVA.reset(VertexArray::Create());
    std::shared_ptr<VertexBuffer> squareVB;
    squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetBufferLayout(
      {
       {ShaderDataType::Float3, "a_Position"},
      });

    m_SquareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<IndexBuffer> squareIB;
    squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
    m_SquareVA->SetIndexBuffer(squareIB);

    {
      std::string vertexSrc = R"(
        #version 400

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        out vec3 v_Position;
        out vec4 v_Color;

        uniform mat4 u_ViewProjection;

        void main() {
          v_Position = a_Position;
          v_Color = a_Color;
          gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

      std::string fragmentSrc = R"(
        #version 400

        out vec4 color;
        in vec3 v_Position;
        in vec4 v_Color;


        void main() {
          color = vec4(v_Position * 0.5 + 0.5, 1.0);
        }
    )";

      m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

      std::string blueShaderVertexSrc = R"(
        #version 400

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        out vec3 v_Position;
        uniform mat4 u_ViewProjection;

        void main() {
          v_Position = a_Position;
          gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

      std::string blueShaderfragmentSrc = R"(
        #version 400

        out vec4 color;
        in vec3 v_Position;

        void main() {
          color = vec4(0.2, 0.3, 0.8, 1.0);
        }
    )";

      m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderfragmentSrc));
    }
  }

  Application::~Application() {
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    HZ_CORE_TRACE("{0}", e);

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
      (*--it)->OnEvent(e);
      if (e.Handled)
        break;
    }
  }

  void Application::Run() {

    while (m_Running) {

      RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1 });
      RenderCommand::Clear();

      m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
      m_Camera.SetRotation(45.0f);

      Renderer::BeginScene(m_Camera);
      
      Renderer::Submit(m_BlueShader, m_SquareVA);
      Renderer::Submit(m_Shader, m_VertexArray);
      
      Renderer::EndScene();

      for (Layer* layer : m_LayerStack) {
        layer->OnUpdate();
      }
      m_ImGuiLayer->Begin();
      for (Layer* layer : m_LayerStack) {
        layer->OnImGuiRender();
      }
      m_ImGuiLayer->End();

      m_Window->OnUpdate();
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return m_Running;
  }

  void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
  }

}
