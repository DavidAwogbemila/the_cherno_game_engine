#pragma once

#include "stdio.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
  printf("Entered the Hazel Engine.\n");

  Hazel::Log::Init();

  HZ_CORE_WARN("Core logger issuing a warning!");
  HZ_INFO("Core logger furnished info.");
  //int a = 5;
  //HZ_INFO("Hello Var={0}", a);
 
  auto app = Hazel::CreateApplication();
  app->Run();
  delete app;
}

#endif
