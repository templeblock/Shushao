project "GLFW"
    basedir ("GLFW")
    kind "StaticLib"
    language "c"
    -- toolset "gcc"

    -- configuration { "gmake2" }
    --     targetextension (".a")

    -- targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    -- objdir ("obj/" .. outputdir .. "/%{prj.name}")

    targetdir ("%{prj.location}/lib")
    objdir ("%{prj.location}/obj")

    makesettings [[
        CC = gcc
    ]]

    files {
        "%{prj.location}/include/GLFW/glfw3.h",
        "%{prj.location}/include/GLFW/glfw3native.h",
        "%{prj.location}/src/glfw_config.h",
        "%{prj.location}/src/context.c",
        "%{prj.location}/src/init.c",
        "%{prj.location}/src/input.c",
        "%{prj.location}/src/monitor.c",
        "%{prj.location}/src/vulkan.c",
        "%{prj.location}/src/window.c"
    }

    links {
        "gdi32"
    }

    filter "system:windows"
        buildoptions { "-std=c11" }
        systemversion "latest"
        staticruntime "on"
        toolset "gcc"

        files {
            "%{prj.location}/src/win32_init.c",
            "%{prj.location}/src/win32_joystick.c",
            "%{prj.location}/src/win32_monitor.c",
            "%{prj.location}/src/win32_time.c",
            "%{prj.location}/src/win32_thread.c",
            "%{prj.location}/src/win32_window.c",
            "%{prj.location}/src/wgl_context.c",
            "%{prj.location}/src/egl_context.c",
            "%{prj.location}/src/osmesa_context.c"
        }

        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"