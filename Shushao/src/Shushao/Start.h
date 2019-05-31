#ifndef SE_START
#define SE_START

#include "Shushao/Application.h"
#include "Shushao/Debug.h"

extern se::Application* se::CreateApplication();

int main(int argc, char** argv) {
    DEBUG_INFO("Test");

    // std::cout << "test" << std::endl;

    //auto app = se::CreateApplication();
    //app->Init();
    //app->Run();
    //delete app;
}

#endif
