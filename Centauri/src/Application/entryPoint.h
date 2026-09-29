#include "application.h"
#include <iostream>

extern Centauri::Application* CreateApplication();

int main(int argc, char** argv) {
    auto app = CreateApplication();
    app -> init();
    app -> run();
    delete app;
}