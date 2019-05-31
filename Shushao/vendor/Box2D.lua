project 'Box2D'
    basedir ("Box2D")
    kind 'StaticLib'
    cppdialect 'C++11'

    targetdir ("%{prj.location}/lib")
    objdir ("%{prj.location}/obj")

    files { '%{prj.location}/Box2D/**' }
    includedirs { '%{prj.location}/.' }

    configuration { "gmake2" }
        targetextension (".a")

    makesettings [[
        CC = g++
    ]]

    defines { 'NDEBUG' }
        optimize 'On'

    filter 'system:windows'
        defines { '_CRT_SECURE_NO_WARNINGS' }
