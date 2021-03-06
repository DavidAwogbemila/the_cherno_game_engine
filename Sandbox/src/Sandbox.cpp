#include <Hazel.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public  Hazel::Layer {
public:
  ExampleLayer()
    : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f) {
    m_VertexArray.reset(Hazel::VertexArray::Create());

    // Set up data to draw triangle.
    float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

    vertexBuffer->SetBufferLayout({
      {Hazel::ShaderDataType::Float3, "a_Position"},
      {Hazel::ShaderDataType::Float4, "a_Color"}
      });

    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };

    Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
    indexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    // Set up data to draw square.
    float squareVertices[5 * 4] = {
       0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
       0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    m_SquareVA.reset(Hazel::VertexArray::Create());
    std::shared_ptr<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetBufferLayout(
      {
       {Hazel::ShaderDataType::Float3, "a_Position"},
       {Hazel::ShaderDataType::Float2, "a_TexCoord"}
      });

    m_SquareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Hazel::Ref<Hazel::IndexBuffer> squareIB;
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
        uniform mat4 u_Transform;

        void main() {
          v_Position = a_Position;
          v_Color = a_Color;
          gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

      m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

      std::string flatColorShaderVertexSrc = R"(
        #version 400

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        out vec3 v_Position;
        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main() {
          v_Position = a_Position;
          gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

      std::string flatColorShaderFragmentSrc = R"(
        #version 400

        out vec4 color;
        in vec3 v_Position;

        uniform vec3 u_Color;

        void main() {
          color = vec4(u_Color, 1.0f);
        }
    )";

      m_FlatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

      std::string textureShaderVertexSrc = R"(
        #version 400

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;
        
        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec2 v_TexCoord;

        void main() {
          v_TexCoord = a_TexCoord;
          gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

      std::string textureShaderFragmentSrc = R"(
        #version 400

        out vec4 color;
        in vec2 v_TexCoord;

        uniform sampler2D u_Texture;

        void main() {
          color = texture(u_Texture, v_TexCoord);
        }
    )";
      m_TextureShader.reset(Hazel::Shader::Create("assets/shaders/Texture.glsl"));
    }

    m_Texture = Hazel::Texture2D::Create("assets/Textures/Checkerboard.png");
    m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/Textures/ChernoLogo.png");

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

  }

  void OnUpdate(Hazel::Timestep ts) {

    //HZ_TRACE("Delta time: {0}, ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
    {
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

      if (Hazel::Input::IsKeyPressed(HZ_KEY_J)) {
        m_SquarePosition.x -= m_SquareMoveSpeed * ts;
      }

      else if (Hazel::Input::IsKeyPressed(HZ_KEY_L)) {
        m_SquarePosition.x += m_SquareMoveSpeed * ts;
      }

      if (Hazel::Input::IsKeyPressed(HZ_KEY_I)) {
        m_SquarePosition.y += m_SquareMoveSpeed * ts;
      }

      else if (Hazel::Input::IsKeyPressed(HZ_KEY_K)) {
        m_SquarePosition.y -= m_SquareMoveSpeed * ts;
      }
    }

    Hazel::RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1 });
    Hazel::RenderCommand::Clear();

    m_Camera.SetPosition({ m_CameraPosition });
    m_Camera.SetRotation(m_CameraRotation);

    Hazel::Renderer::BeginScene(m_Camera);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 20; j++) {
        glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

        Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
      }
    }

    m_Texture->Bind();
    Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    m_ChernoLogoTexture->Bind();
    Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    //Hazel::Renderer::Submit(m_Shader, m_VertexArray);

    Hazel::Renderer::EndScene();
  }

  void OnEvent(Hazel::Event& event) {
    Hazel::EventDispatcher dispatcher(event);

  }

  void OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
    ImGui::End();

  }
private:
  Hazel::Ref<Hazel::Shader> m_Shader;
  Hazel::Ref<Hazel::VertexArray> m_VertexArray;

  Hazel::Ref<Hazel::VertexArray> m_SquareVA;
  Hazel::Ref<Hazel::Shader> m_FlatColorShader, m_TextureShader;

  Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

  Hazel::OrthographicCamera m_Camera;
  glm::vec3 m_CameraPosition;
  float m_CameraMoveSpeed = 5.0f;
  float m_CameraRotation = 0.0f;
  float m_CameraRotationSpeed = 180.0f;

  glm::vec3 m_SquarePosition;
  float m_SquareMoveSpeed = 1.0f;

  glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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
