#include "sepch.h"

#include "WindowsWindow.h"
#include "Shushao/Core.h"
#include "Shushao/Debug.h"
#include "Shushao/Events/ApplicationEvent.h"
#include "Shushao/Events/KeyEvent.h"
#include "Shushao/Events/MouseEvent.h"
#include "Shushao/GLManager.h"

namespace se {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
    DEBUG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    wData.Title = props.Title;
    wData.Width = props.Width;
    wData.Height = props.Height;
    wData.Fullscreen = props.Fullscreen;

    DEBUG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        // TODO: glfwTerminate on system shutdown
        int success = glfwInit();
        SE_CORE_ASSERT(success, "Could not intialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow(
        (int)props.Width,
        (int)props.Height,
        wData.Title.c_str(),
        (wData.Fullscreen ? glfwGetPrimaryMonitor() : nullptr),
        nullptr  //
    );

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &wData);
    SetVSync(true);

    // init Glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    SE_CORE_ASSERT(status, "Failed to initialize Glad!");

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLManager::DESKTOP_WIDTH = mode->width;
    GLManager::DESKTOP_HEIGHT = mode->height;

    return;

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

void WindowsWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool enabled) {
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    wData.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return wData.VSync;
}

void WindowsWindow::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
}

void WindowsWindow::Clear(float r, float g, float b, float a, float depth = 1.0f) const {
    glClearColor(r, g, b, a);
    glClearDepth(depth);
}

void WindowsWindow::SetFullscreen(bool fs) {
    fullscreen = fs;

    // TODO
#if 0
    if (fullscreen) {
        SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        glViewport(0, 0, DESKTOP_WIDTH, DESKTOP_HEIGHT);
    } else {
        SDL_SetWindowFullscreen(gWindow, SDL_FALSE);
        SDL_GetWindowSize(gWindow, (int*)&WIDTH, (int*)&HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
    }
#endif
}

void WindowsWindow::ToggleFullscreen() {
    SetFullscreen(!fullscreen);
}

void WindowsWindow::Swap() const {
    glfwSwapBuffers(m_Window);
}

void WindowsWindow::Reset() const {
    Clear();
    // glLoadIdentity(); // old pipeline
}

}  // namespace se
