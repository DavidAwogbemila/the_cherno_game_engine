#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
  Hazel::Log::Init();

  HZ_CORE_INFO("[INFO] Initiating Hazel Engine.");
 
  auto app = Hazel::CreateApplication();
  app->Run();
  delete app;

  return EXIT_SUCCESS;
}

#endif
