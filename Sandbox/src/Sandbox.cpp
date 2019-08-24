#include <Hazel.h>
//#include <glm/glm/glm.hpp>
#include "imgui/imgui.h"

class ExampleLayer : public  Hazel::Layer {
public:
  ExampleLayer()
    : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
    m_VertexArray.reset(Hazel::VertexArray::Create());

    // Set up data to draw triangle.
    float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

    vertexBuffer->SetBufferLayout({
      {Hazel::ShaderDataType::Float3, "a_Position"},
      {Hazel::ShaderDataType::Float4, "a_Color"}
      });

    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };

    std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
    indexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    // Set up data to draw square.
    float squareVertices[3 * 4] = {
       0.0f, 0.0f, 0.0f,
       0.5f, 0.0f, 0.0f,
       0.5f, 0.5f, 0.0f,
       0.0f, 0.5f, 0.0f
    };

    m_SquareVA.reset(Hazel::VertexArray::Create());
    std::shared_ptr<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetBufferLayout(
      {
       {Hazel::ShaderDataType::Float3, "a_Position"},
      });

    m_SquareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<Hazel::IndexBuffer> squareIB;
    squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
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

      m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

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

      m_BlueShader.reset(new Hazel::Shader(blueShaderVertexSrc, blueShaderfragmentSrc));
    }
  }

  void OnUpdate(Hazel::Timestep ts) override {

    //HZ_TRACE("Delta time: {0}, ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

    if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) {
      m_CameraPosition.x -= m_CameraMoveSpeed * ts;
    }

    else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) {
      m_CameraPosition.x += m_CameraMoveSpeed * ts;
    }

    if (Hazel::Input::IsKeyPressed(HZ_KEY_UP)) {
      m_CameraPosition.y += m_CameraMoveSpeed * ts;
    }

    else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN)) {
      m_CameraPosition.y -= m_CameraMoveSpeed * ts;
    }

    if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
      m_CameraRotation += m_CameraRotationSpeed * ts;
    }

    else if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
      m_CameraRotation -= m_CameraRotationSpeed * ts;
    }

    Hazel::RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1 });
    Hazel::RenderCommand::Clear();
    
    m_Camera.SetPosition({ m_CameraPosition });
    m_Camera.SetRotation(m_CameraRotation);

    Hazel::Renderer::BeginScene(m_Camera);

    Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
    Hazel::Renderer::Submit(m_Shader, m_VertexArray);

    Hazel::Renderer::EndScene();
  }

  void OnEvent(Hazel::Event& event) override {
    Hazel::EventDispatcher dispatcher(event);

  }

  void OnImGuiRender() {

  }
private:
  std::shared_ptr<Hazel::Shader> m_Shader;
  std::shared_ptr<Hazel::VertexArray> m_VertexArray;

  std::shared_ptr<Hazel::VertexArray> m_SquareVA;
  std::shared_ptr<Hazel::Shader> m_BlueShader;

  Hazel::OrthographicCamera m_Camera;
  glm::vec3 m_CameraPosition;
  float m_CameraMoveSpeed = 5.0f;
  float m_CameraRotation = 0.0f;
  float m_CameraRotationSpeed = 180.0f;
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
