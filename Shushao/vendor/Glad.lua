project "Glad"
    basedir ("Glad")
    kind "StaticLib"
    targetname "libGlad"

    targetdir ("%{prj.location}/lib")
    objdir ("%{prj.location}/obj")

    makesettings [[
        CC = gcc
    ]]

    -- configuration { "gmake2" }
    --     targetextension (".a")

    files {
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/src/glad.c"
    }

    includedirs {
        "%{prj.location}/include"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"