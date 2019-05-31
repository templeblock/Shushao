project "SOIL2"
    basedir ("SOIL")
    kind "StaticLib"
    language "C++"

    buildoptions { "/TP" }
    defines { "_CRT_SECURE_NO_WARNINGS" }

    targetdir ("%{prj.location}/lib")
    objdir ("%{prj.location}/obj")

    files { "%{prj.location}/src/SOIL2/*.c" }

    configuration "debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "On"

    configuration "release"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"
