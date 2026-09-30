#include "application.h"
#include <iostream>

//To be defined externally
extern Centauri::Application* CreateApplication();

int main(int argc, char** argv) {
    auto app = CreateApplication();
    app -> Init();
    app -> Run();
    delete app;
}