#pragma once

#include "sepch.h"
#include "Shushao/Core.h"

#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace se {

class Application {
public:
    static Application* instance;

    enum class Stage {
        INIT,
        UPDATE,
        FIXED,
        RENDER,
        EXIT
    };

    virtual ~Application();

    std::string name;

    bool Init();
    void Run();
    void Stop();

    inline static Application& Get() { return *instance; }
    inline Window& GetWindow() { return *window; }

    void OnEvent(Event& e);

protected:
    virtual void Awake(){};
    virtual void Start(){};
    virtual void GetInput(){};
    virtual void Update(){};
    virtual void FixedUpdate(){};
    virtual void Render(){};
    virtual void End(){};

    virtual bool InitResources() = 0;  // resources loading
    virtual bool InitConfig() = 0;  // config override
    virtual bool InitMapping() = 0;  // input mapping
    virtual bool InitScene() = 0;  // scene load

    bool RUNNING = true;
    bool READY = false;

    bool keys[350];

private:
    void InitScan();
    void update();
    void render();
    void fixed();
    void exit();

    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> window;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace se
