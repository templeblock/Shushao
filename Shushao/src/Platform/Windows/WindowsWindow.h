#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Shushao/Window.h"

namespace se {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return wData.Width; }
    inline unsigned int GetHeight() const override { return wData.Height; }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { wData.EventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline virtual void* GetNativeWindow() const { return m_Window; }

    virtual void Clear() const override;
    virtual void Clear(float, float, float, float, float) const override;
    virtual void SetFullscreen(bool) override;
    virtual void ToggleFullscreen() override;
    virtual void Swap() const override;
    virtual void Reset() const override;

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_Window;

    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
        bool Fullscreen;

        EventCallbackFn EventCallback;
    };

    WindowData wData;
};

}  // namespace se