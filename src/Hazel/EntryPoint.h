#pragma once

#include "stdio.h"

#ifdef HZ_PLATFORM_WINDOWS

Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
  printf("entered the Hazel Engine.\n");
  
  auto app = Hazel::CreateApplication();
  app->Run();
  delete app;
}

#endif
